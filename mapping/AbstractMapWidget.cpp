#include "AbstractMapWidget.h"
#include <QIcon>

#include "../graph/graph.h"
#include "../CalenhadServices.h"
#include "../preferences/preferences.h"
#include "projection/ProjectionService.h"
#include "projection/Projection.h"
#include <QWindow>
#include <QtXml/QtXml>
#include <QtGui/QPainter>
#include <controls/globe/CalenhadGlobeConstants.h>
#include <GeographicLib/Geodesic.hpp>
#include <QtWidgets/QToolTip>
#include "Graticule.h"
#include "../module/Module.h"
#include "../nodeedit/Connection.h"
#include "../icosphere/icosphere.h"

using namespace calenhad;
using namespace geoutils;
using namespace matrices;
using namespace icosphere;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::mapping::projection;
using namespace calenhad::pipeline;
using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace GeographicLib;

AbstractMapWidget::AbstractMapWidget (const RenderMode& mode, QWidget* parent) : QOpenGLWidget (parent),
     _datumFormat (DatumFormat::Scaled),
    _zoomDrag (false),
    _graph (nullptr),
    _createHeightMap (mode == RenderMode::RenderModeGlobe),
    _sensitivity (0.5),
    _refreshHeightMap (mode == RenderMode::RenderModeGlobe),
    _mouseDoubleClickMode (mode == RenderMode::RenderModeGlobe ? CalenhadGlobeDoubleClickMode::Goto : CalenhadGlobeDoubleClickMode::NoDoubleClick),
    _source (nullptr),
    _geodesic (new Geodesic (1, 0)),
    _vertexBuffer (nullptr),
    _computeShader (nullptr),
    _computeProgram (nullptr),
    _vertexShader (nullptr),
    _fragmentShader (nullptr),
    _globeTexture (nullptr),
    _renderProgram (nullptr),
    _indexBuffer (nullptr),
    _colorMapBuffer (nullptr),
    _heightMapBuffer (nullptr),
    _projection (CalenhadServices::projections() -> fetch (mode == RenderMode::RenderModeGlobe ? "Orthographic" : "Equirectangular")),
    _scale (1.0),
    _shader (""),
    _graticule (nullptr),
    _graticuleVisible (true),
    _coordinatesFormat (CoordinatesFormat::Traditional),
    _rotation (Geolocation (0, 0)),
    _renderTime (0),
    _rasterTexture (nullptr),
    _tileSize (512),
    _mode (mode),
    _mainMap (nullptr),
    _computeVertices (mode == RenderMode::RenderModeGlobe) {

    QSurfaceFormat format;
    format.setSamples(8);
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // read shader code from files into memory for use at render time
    QFile csFile (":/shaders/map_cs.glsl");
    csFile.open (QIODevice::ReadOnly);
    QTextStream csTextStream (&csFile);
    _shaderTemplate = csTextStream.readAll ();

    QFile vsFile (":/shaders/map_vs.glsl");
    vsFile.open (QIODevice::ReadOnly);
    QTextStream vsTextStream (&vsFile);
    _vertexShaderCode = vsTextStream.readAll ();

    QFile fsFile (":/shaders/map_fs.glsl");
    fsFile.open (QIODevice::ReadOnly);
    QTextStream fsTextStream (&fsFile);
    _fragmentShaderCode = fsTextStream.readAll ();

    _graticule = new Graticule (this);
    if (_mode == RenderMode::RenderModePreview) { _graticule -> setDensity (-1); }
    if (_mode == RenderMode::RenderModeOverview) { _graticule -> setDensity (-2); }

    if (_mode == RenderMode::RenderModeOverview) {
        connect (this, &AbstractMapWidget::rendered, this, [=] () { if (_mainMap) { _mainMap -> update(); }});
    }
}

AbstractMapWidget::~AbstractMapWidget() {
    makeCurrent();
    delete _computeShader;
    delete _vertexShader;
    delete _fragmentShader;
    delete _globeTexture;
    delete _heightMapBuffer;
    delete _renderProgram;
    delete _computeProgram;
    delete _indexBuffer;
    delete _vertexBuffer;
    delete _graticule;
    delete _geodesic;
}

void AbstractMapWidget::prepareRasters () {
    // create and allocate a buffer for any input rasters
    makeCurrent ();
    int rasters = _graph->rasterCount ();
    if (_rasterTexture) {
        _rasterTexture->destroy ();
        delete _rasterTexture;
        _rasterTexture = nullptr;
    }
    if (rasters > 0) {
        glActiveTexture (GL_TEXTURE1);
        _rasterTexture = new QOpenGLTexture (QOpenGLTexture::Target2DArray);
        _rasterTexture->create ();
        _rasterTexture->setFormat (QOpenGLTexture::RGBA8_UNorm);
        _rasterTexture->setSize (CalenhadServices::preferences() -> calenhad_globe_texture_height, CalenhadServices::preferences() -> calenhad_globe_texture_height);
        _rasterTexture->setMinificationFilter (QOpenGLTexture::Linear);
        _rasterTexture->setMagnificationFilter (QOpenGLTexture::Linear);
        _rasterTexture->allocateStorage ();
        _rasterTexture->bind ();
        glBindImageTexture (1, _rasterTexture->textureId (), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
        for (int i = 0; i < rasters; i++) {
            QImage* raster = _graph->raster (i);
            _rasterTexture -> setData (0, i, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) raster -> bits ());
        }
    }
}

void AbstractMapWidget::updateParams() {
    makeCurrent();
    _computeProgram -> link ();
    _computeProgram -> bind ();
    glUseProgram (_computeProgram -> programId());
    static GLint destLoc = glGetUniformLocation (_computeProgram->programId (), "destTex");
    static GLint cmbsLoc = glGetUniformLocation (_computeProgram->programId (), "colorMapBufferSize");
    static GLint imageHeightLoc = glGetUniformLocation (_computeProgram->programId (), "imageHeight");
    static GLint insetHeightLoc = glGetUniformLocation (_computeProgram->programId (), "insetHeight");
    static GLint projectionLoc = glGetUniformLocation (_computeProgram->programId (), "projection");
    static GLint datumLoc = glGetUniformLocation (_computeProgram->programId (), "datum");
    static GLint rasterResolutionLoc = glGetUniformLocation (_computeProgram->programId (), "rasterResolution");
    static GLint vertexCountLoc = glGetUniformLocation (_computeProgram -> programId(), "vertexCount");
    static GLint renderModeLoc = glGetUniformLocation (_computeProgram -> programId(), "mode");


    glUniform1i (destLoc, 0);
    glUniform3f (datumLoc, (GLfloat) _rotation.longitude(), (GLfloat) _rotation.latitude(), (GLfloat) _mode == RenderMode::RenderModeOverview ? _mainMap -> scale() : _scale);
    glUniform1i (projectionLoc, _mode == RenderMode::RenderModeOverview ? _mainMap -> projection() -> id() : _projection -> id ());
    glUniform1i (imageHeightLoc, _mode == RenderMode::RenderModeOverview ? _mainMap -> textureHeight() : _globeTexture -> height());
    glUniform1i (insetHeightLoc, _globeTexture -> height());
    glUniform1i (cmbsLoc, 2048);
    glUniform1i (rasterResolutionLoc, CalenhadServices::preferences() -> calenhad_globe_texture_height);
    if (_computeVertices) {
        glUniform1i (vertexCountLoc, CalenhadServices::icosphere ()->vertexCount ());
    }
    glUniform1i (renderModeLoc, _mode);

}

void AbstractMapWidget::paintGL() {
    if (_graph) {

        QPainter p (this);
        p.beginNativePainting ();
        compute ();
        static GLint srcLoc = glGetUniformLocation (_renderProgram -> programId (), "srcTex");
        glUniform1i (srcLoc, 0);


        _renderProgram->bind ();
        _globeTexture->bind ();

        glBindImageTexture (0, _globeTexture->textureId (), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
        // draw

        glDrawElements (GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        _vao.release ();

        glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

        p.endNativePainting ();

        // draw the graticule
        if (_graticule && _graticuleVisible) {
            _graticule -> drawGraticule (p);
        }
    }
}

GLfloat* AbstractMapWidget::heightMapBuffer() {
    return _heightMapBuffer;
}

QSize AbstractMapWidget::heightMapSize() const {
    return QSize (_globeTexture -> width(), _globeTexture -> height());
}

void AbstractMapWidget::resizeGL (int width, int height) {
    if (_graph) {

        glViewport (0, 0, width, height);

        glMatrixMode (GL_PROJECTION); // switch to projection matrix
        glLoadIdentity ();
        if (width >= height) {
            glOrtho (-0.5f * 2.0, 0.5f * 2.0, 0.0f, 1.0f, -1, 1);
        } else {
            glOrtho (-0.5f, 0.5f, 0.0, 1.0 / 2.0, -1, 1);
        }
        glMatrixMode (GL_MODELVIEW);
        redraw();
    }
}

void AbstractMapWidget::createTexture () {
    makeCurrent();
    glActiveTexture (GL_TEXTURE0);
    _yTiles = 1;

    if (!_globeTexture || _globeTexture -> height() != _yTiles * _tileSize || _globeTexture -> width() != _yTiles * 2 * _tileSize) {

        if (_globeTexture) { delete _globeTexture; }
        _globeTexture = new QOpenGLTexture (QOpenGLTexture::Target2D);
        _globeTexture->create();
        _globeTexture->setFormat (QOpenGLTexture::RGBA8_UNorm);
        _globeTexture->setSize (_yTiles * 2 * _tileSize, _yTiles * _tileSize);
        _globeTexture->setMinificationFilter (QOpenGLTexture::Linear);
        _globeTexture->setMagnificationFilter (QOpenGLTexture::Linear);
        _globeTexture->allocateStorage();
        _globeTexture->bind();
        glBindImageTexture (0, _globeTexture -> textureId (), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);


        // create and allocate the heightMapBuffer on the GPU. This is for downloading the heightmap from the GPU.
        int v = _globeTexture->height ();
        if (_heightMapBuffer) {
            delete _heightMapBuffer;
            _heightMapBuffer = nullptr;
        }
        _heightMapBuffer = new GLfloat[2 * v * v];
        glGenBuffers (1, &heightMap);
        glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
        glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * v * v, NULL, GL_DYNAMIC_READ);
        glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 3, heightMap);
        glBindBuffer (GL_SHADER_STORAGE_BUFFER, 1); // unbind
    }
}

void AbstractMapWidget::showEvent (QShowEvent* e) {
    //updateGL();
}


void AbstractMapWidget::setScale (const double& scale) {
    _scale = scale;
    redraw();
}

double AbstractMapWidget::scale () {
    return _scale;
}

void AbstractMapWidget::redraw() {
    update();
}

void AbstractMapWidget::setProjection (const QString& projection) {
    _projection = CalenhadServices::projections () -> fetch (projection);
    redraw();
}

Projection* AbstractMapWidget::projection() {
    return _projection;
}

void AbstractMapWidget::rotate (const Geolocation& rotation) {
    _rotation = rotation;
    redraw();
}

const Geolocation& AbstractMapWidget::rotation () {
    return _rotation;
}



bool AbstractMapWidget::isInViewport (Geolocation g) {
    QPointF s;
    if (screenCoordinates (g, s)) {
        return s.x() >= 0 && s.x() <= width() && s.y() >= 0 && s.y() < height();
    } else {
        return false;
    }
}

QImage* AbstractMapWidget::heightmap() {
    QImage* image = new QImage (_globeTexture -> width(), _globeTexture -> height(), QImage::Format_ARGB32);
    GLfloat* buffer = heightMapBuffer();
    if (buffer) {
        image->fill (Qt::red);
        int w = _globeTexture -> width();
        int h = _globeTexture -> height();
        for (int y = h - 1; y >= 0; y--) {
            for (int x = 0; x < w; x++) {
                GLfloat value = buffer [(h - y) * w + x];
                value = std::min (std::max (-1.0f, value), 1.0f);
                int k = (int) ((value + 1) / 2 * 256);
                image -> setPixelColor (x, y, QColor (k, k, k));
            }
        }
    }
    return image;
}

bool AbstractMapWidget::valueAt (const QPointF& sc, double& value) {
    QPoint tc = texCoordinates (sc);
    int index = tc.y() * _globeTexture -> width () + tc.x();
    if (index >= 0 && index < _globeTexture -> width () * _globeTexture -> height()) {
        value = (GLfloat) heightMapBuffer() [index];
        return true;
    } else {
        return false;
    }
}

QPoint AbstractMapWidget::texCoordinates (const QPointF& sc) {
    if (_globeTexture) {
        QPoint tc;

        double x = sc.x () / width ();// * xp;
        double y = sc.y () / height ();// * xp;
        tc.setX (x * _globeTexture -> width ());
        tc.setY ((1 - y) * _globeTexture -> height ());
        return tc;
    } else {
        return QPoint();
    }
}

bool AbstractMapWidget::screenCoordinates (Geolocation geolocation, QPointF& sc) {
    Projection* projection = _mode == RenderMode::RenderModeOverview ? CalenhadServices::projections () -> fetch ("Equirectangular") : _projection;
    projection -> setDatum (_mode == RenderMode::RenderModeOverview ? Geolocation (0, 0, Degrees) : _rotation);
    QPointF p;
    bool fwd = projection -> forward (geolocation, p);
    if (fwd) {
        double x = p.x () / (_mode == RenderMode::RenderModeOverview ? 1.0 :_scale);
        double y = (-p.y ()) / (_mode == RenderMode::RenderModeOverview ? 1.0 : _scale);
        x += M_PI;
        y += M_PI / 2;
        x /= M_PI * 2;
        y /= M_PI;
        sc.setX (x * width ());
        sc.setY (y * height ());
    }
    return fwd;
}

bool AbstractMapWidget::geoCoordinates (QPointF pos, Geolocation& geolocation) {
    Projection* projection = _mode == RenderMode::RenderModeOverview ? CalenhadServices::projections () -> fetch ("Equirectangular") : _projection;
    double x = ((pos.x() / width()) * M_PI * 2 - M_PI) * _scale;
    double y = ((pos.y() / height()) * M_PI - (M_PI / 2)) * _scale;
    projection -> setDatum (_rotation);
    bool result = projection -> inverse (QPointF (x, -y), geolocation);
    return result;
}



void AbstractMapWidget::setGraticuleVisible (const bool& visible) {
    _graticuleVisible = visible;
    update();
}

bool AbstractMapWidget::isGraticuleVisible() {
    return _graticuleVisible;
}

Graticule* AbstractMapWidget::graticule() {
    return _graticule;
}

void AbstractMapWidget::setCoordinatesFormat (CoordinatesFormat format) {
    _coordinatesFormat = format;
}

geoutils::CoordinatesFormat AbstractMapWidget::coordinatesFormat () {
    return _coordinatesFormat;
}

Statistics AbstractMapWidget::statistics() {
    if (! _globeTexture) { return Statistics (0.0, 0.0, 0.0, 0, 0, 0); }
    double _min = 0, _max = 0, _sum = 0;
    int count = 0;
    GLfloat* buffer = heightMapBuffer();
    if (buffer) {
        for (int i = 0; i < _globeTexture->height () * _globeTexture->width (); i++) {
            if (!isnan (buffer[i])) {
                if (buffer[i] < _min) { _min = buffer[i]; }
                if (buffer[i] > _max) { _max = buffer[i]; }
                _sum += buffer[i];
                count++;
            }
        }
    }
    Statistics statistics = Statistics (_min, _max, _sum, count, _renderTime, _globeTexture -> height());
    return statistics;
}

void AbstractMapWidget::paintEvent (QPaintEvent* e) {
    QOpenGLWidget::paintEvent (e);
}



void AbstractMapWidget::setDatumFormat (DatumFormat format) {
    _datumFormat = format;
}

void AbstractMapWidget::setSensitivity (double sensitivity) {
    _sensitivity = sensitivity;
}

DatumFormat AbstractMapWidget::datumFormat () {
    return _datumFormat;
}

double AbstractMapWidget::sensitivity () {
    return _sensitivity;
}

void AbstractMapWidget::goTo (const Geolocation& geolocation) {
    rotate (geolocation);
}

void AbstractMapWidget::mousePressEvent (QMouseEvent* e) {
    Geolocation loc;
    if (e -> button () == Qt::LeftButton) {
        if (geoCoordinates (e -> pos(), loc)) {
            _moveFrom = e -> pos ();
            setCursor (Qt::OpenHandCursor);
        }
    }
}

void AbstractMapWidget::mouseDoubleClickEvent (QMouseEvent* e) {
    if (_mouseDoubleClickMode == CalenhadGlobeDoubleClickMode::Goto) {
        Geolocation loc;
        if (geoCoordinates (e -> pos(), loc)) {
            goTo (loc);
        }
    }
    if (_mouseDoubleClickMode == CalenhadGlobeDoubleClickMode::Place) {
        // to do
    }
}

void AbstractMapWidget::mouseMoveEvent (QMouseEvent* e) {
    Geolocation se, nw;

    if (e -> buttons() & Qt::LeftButton) {
        if (cursor().shape () != Qt::ClosedHandCursor) {
            setCursor (Qt::ClosedHandCursor);
        }

        double dx = e -> pos().x() - _moveFrom.x();
        double dy = e -> pos().y() - _moveFrom.y();

        if (_mouseDragMode == CalenhadGlobeDragMode::Pan) {
            double dLon = 180.0 * _scale * _sensitivity * dx / 50;
            double dLat = 180.0 * _scale * _sensitivity * dy / 50;
            _moveFrom = e -> pos();
            goTo (Geolocation (_rotation.latitude() + qDegreesToRadians (dLat), _rotation.longitude() - qDegreesToRadians (dLon)));
        }

        if (_mouseDragMode == CalenhadGlobeDragMode::Zoom) {
            double dz = dy * _sensitivity / 10;
            emit zoomRequested (_scale + dz);
            _moveFrom = e -> pos();
            update();
        }
    } else {
        setCursor (Qt::CrossCursor);
        if  ( _coordinatesFormat != CoordinatesFormat::NoCoordinates) {
            Geolocation loc;
            QPointF point;
            point.setX ((double) e -> pos().x());
            point.setY ((double) e -> pos().y());

            if (geoCoordinates (point, loc)) {
                QString text = geoutils::Math::geoLocationString (loc, _coordinatesFormat);
                double value;
                if (_globeTexture) {
                    QPoint tc = texCoordinates (point);
                    if (isInViewport (loc)) {
                        text += ": " + QString::number (tc.x()) + ", " + QString::number (tc.y());
                        text += ": " + QString::number (tc.y() * _globeTexture -> width() + tc.x()) + " ";
                        if (valueAt (point, value)) {
                            text += ": " + QString::number (value);
                        }
                        QToolTip::showText (e->globalPos (), text, this);
                    }
                }
            }
        }
    }
}

void AbstractMapWidget::wheelEvent (QWheelEvent* event) {
    double dz =  - event -> delta() * _sensitivity / 1200;
    emit zoomRequested (_scale + dz);
    update();
}

void AbstractMapWidget::mouseReleaseEvent (QMouseEvent* e) {
    _zoomDrag = false;

    Geolocation loc;

    // rollback cursor for mouse move without buttons
    if (geoCoordinates (e -> pos(), loc)) {
        setCursor (Qt::CrossCursor);
    } else {
        setCursor (Qt::ArrowCursor);
    }
}


void AbstractMapWidget::setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mode) {
    _mouseDoubleClickMode = mode;
}

void AbstractMapWidget::setMouseDragMode (const CalenhadGlobeDragMode& mode) {
    _mouseDragMode = mode;
}


// move the view centre along a given azimuth
void AbstractMapWidget::navigate (const NavigationEvent& e) {
    // move the viewport centre in the chosen direction by the distance multiplied by the current scale
    double lat, lon;
    double distance = e.distance() * scale();
    _geodesic -> Direct (rotation().latitude (Units::Degrees), rotation().longitude (Units::Degrees), e.azimuth(), distance, lat, lon);
    goTo (Geolocation (lat, lon, Units::Degrees));
}

CalenhadGlobeDoubleClickMode AbstractMapWidget::mouseDoubleClickMode () {
    return _mouseDoubleClickMode;
}

CalenhadGlobeDragMode AbstractMapWidget::mouseDragMode () {
    return _mouseDragMode;
}

void AbstractMapWidget::setCreateHeightMap (const bool& createHeightMap) {
    _createHeightMap = createHeightMap;
}

int AbstractMapWidget::textureHeight () {
    return _globeTexture -> height();
}

RenderMode AbstractMapWidget::renderMode () {
    return _mode;
}

void AbstractMapWidget::setMainMap (AbstractMapWidget* mainMap) {
    if (_mode == RenderMode::RenderModeOverview) {
        _mainMap = mainMap;
    }
}
