#include "../grid/CubicSphere.h"
#include "CalenhadMapWidget.h"
#include "CalenhadMapWidget.h"
#include <QIcon>

#include "../graph/graph.h"
#include "../mapping/Statistics.h"
#include "src/CalenhadServices.h"
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
#include "../grid/icosphere.h"
#include "../CalenhadServices.h"
#include "../legend//LegendService.h"

using namespace calenhad;
using namespace geoutils;
using namespace matrices;
using namespace grid;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::mapping::projection;
using namespace calenhad::pipeline;
using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace GeographicLib;

CalenhadMapWidget::CalenhadMapWidget (const RenderMode& mode, QWidget* parent) : QOpenGLWidget (parent),
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
                                                                                 _buffer (nullptr),
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
                                                                                 _legend (nullptr),
                                                                                 _computeVertices (mode == RenderMode::RenderModeGlobe) {

    QSurfaceFormat format;
    format.setSamples(8);
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    setContextMenuPolicy(Qt::CustomContextMenu);

    QFile vsFile (":/shaders/map_vs.glsl");
    vsFile.open (QIODevice::ReadOnly);
    QTextStream vsTextStream (&vsFile);
    _vertexShaderCode = vsTextStream.readAll();
    _legend = CalenhadServices::legends() -> lastUsed();
    QFile fsFile (":/shaders/map_fs.glsl");
    fsFile.open (QIODevice::ReadOnly);
    QTextStream fsTextStream (&fsFile);
    _fragmentShaderCode = fsTextStream.readAll ();

    _graticule = new Graticule (this);
    if (_mode == RenderMode::RenderModePreview) { _graticule -> setDensity (-1); }
    if (_mode == RenderMode::RenderModeOverview) { _graticule -> setDensity (-2); }

    if (_mode == RenderMode::RenderModeOverview) {
        connect (this, &CalenhadMapWidget::rendered, this, [=] () { if (_mainMap) { _mainMap -> update(); }});
    }

    // read shader code from files into memory for use at render time
    QFile csFile (":/shaders/render.glsl");
    csFile.open (QIODevice::ReadOnly);
    QTextStream csTextStream (&csFile);
    _shader = csTextStream.readAll ();
}

CalenhadMapWidget::~CalenhadMapWidget() {
    delete _vertexBuffer;
    delete _computeShader;
    delete _vertexShader;
    delete _fragmentShader;
    delete _globeTexture;
    delete _renderProgram;
    delete _computeProgram;
    delete _indexBuffer;

    delete _graticule;
    delete _geodesic;
    if (_buffer) free (_buffer);

}


void CalenhadMapWidget::paintGL() {
    if (_legend && _source) {
        QPainter p (this);
        p.beginNativePainting();
        compute();
        static GLint srcLoc = glGetUniformLocation(_renderProgram->programId(), "srcTex");
        glUniform1i(srcLoc, 0);

        _renderProgram -> bind();
        _globeTexture -> bind();
        glBindImageTexture (0, _globeTexture->textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

        glDrawElements (GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        _vao.release();

        glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

        p.endNativePainting();

        // draw the graticule
        if (_graticule && _graticuleVisible) {
            _graticule -> drawGraticule(p);
        }
    }
}

GLfloat* CalenhadMapWidget::heightMapBuffer() {
    return _buffer;
}

QSize CalenhadMapWidget::heightMapSize() const {
    return QSize (_globeTexture -> width(), _globeTexture -> height());
}

void CalenhadMapWidget::resizeGL (int width, int height) {
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

void CalenhadMapWidget::createTexture () {
    makeCurrent();
//    glActiveTexture (GL_TEXTURE0);
    _yTiles = 1;

    if (!_globeTexture || _globeTexture -> height() != _size || _globeTexture -> width() != 2 * _size) {
        delete _globeTexture;
        _globeTexture = new QOpenGLTexture (QOpenGLTexture::Target2D);
        _globeTexture-> create ();
        _globeTexture-> setFormat (QOpenGLTexture::RGBA8_UNorm);
        _globeTexture-> setSize (_size * 2, _size);
        _globeTexture -> setMinificationFilter (QOpenGLTexture::Linear);
        _globeTexture -> setMagnificationFilter (QOpenGLTexture::Linear);
        _globeTexture -> allocateStorage ();
        _globeTexture -> bind ();
        glBindImageTexture (7, _globeTexture -> textureId (), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    }
}

void CalenhadMapWidget::setScale (const double& scale) {
    if (_mode == RenderModeGlobe) {
        _scale = scale;
        redraw();
    }
}

double CalenhadMapWidget::scale () {
    return _scale;
}

void CalenhadMapWidget::redraw() {
    update();
}

void CalenhadMapWidget::setProjection (const QString& projection) {
    if (_mode == RenderModeGlobe) {
        _projection = CalenhadServices::projections()->fetch(projection);
        redraw();
    }
}

Projection* CalenhadMapWidget::projection() {
    return _projection;
}

void CalenhadMapWidget::rotate (const Geolocation& rotation) {
    if (_mode == RenderModeGlobe) {
        _rotation = rotation;
        redraw();
    }
}

const Geolocation& CalenhadMapWidget::rotation () {
    return _rotation;
}



bool CalenhadMapWidget::isInViewport (Geolocation g) {
    QPointF s;
    if (screenCoordinates (g, s)) {
        return s.x() >= 0 && s.x() <= width() && s.y() >= 0 && s.y() < height();
    } else {
        return false;
    }
}

QPoint CalenhadMapWidget::texCoordinates (const QPointF& sc) {
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

bool CalenhadMapWidget::screenCoordinates (const Geolocation& geolocation, QPointF& sc) {
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

bool CalenhadMapWidget::geoCoordinates (QPointF pos, Geolocation& geolocation) {
    Projection* projection = _mode == RenderMode::RenderModeOverview ? CalenhadServices::projections () -> fetch ("Equirectangular") : _projection;
    double x = ((pos.x() / width()) * M_PI * 2 - M_PI) * _scale;
    double y = ((pos.y() / height()) * M_PI - (M_PI / 2)) * _scale;
    projection -> setDatum (_rotation);
    bool result = projection -> inverse (QPointF (x, -y), geolocation);
    return result;
}

void CalenhadMapWidget::setGraticuleVisible (const bool& visible) {
    _graticuleVisible = visible;
    update();
}

bool CalenhadMapWidget::isGraticuleVisible() const {
    return _graticuleVisible;
}

Graticule* CalenhadMapWidget::graticule() {
    return _graticule;
}

void CalenhadMapWidget::setCoordinatesFormat (CoordinatesFormat format) {
    _coordinatesFormat = format;
}

geoutils::CoordinatesFormat CalenhadMapWidget::coordinatesFormat () {
    return _coordinatesFormat;
}

Statistics CalenhadMapWidget::statistics() {
    if (! _globeTexture) { return {0.0, 0.0, 0.0, 0, 0, 0}; }
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

void CalenhadMapWidget::paintEvent (QPaintEvent* e) {
    QOpenGLWidget::paintEvent (e);
}



void CalenhadMapWidget::setDatumFormat (DatumFormat format) {
    _datumFormat = format;
}

void CalenhadMapWidget::setSensitivity (double sensitivity) {
    _sensitivity = sensitivity;
}

DatumFormat CalenhadMapWidget::datumFormat () {
    return _datumFormat;
}

double CalenhadMapWidget::sensitivity () const {
    return _sensitivity;
}

void CalenhadMapWidget::goTo (const Geolocation& geolocation) {
    rotate (geolocation);
}

void CalenhadMapWidget::mousePressEvent (QMouseEvent* e) {
    Geolocation loc;
    if (e -> button () == Qt::LeftButton) {
        if (geoCoordinates (e -> pos(), loc)) {
            _moveFrom = e -> pos ();
            setCursor (Qt::OpenHandCursor);
        }
    }
}

void CalenhadMapWidget::mouseDoubleClickEvent (QMouseEvent* e) {
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

void CalenhadMapWidget::mouseMoveEvent (QMouseEvent* e) {
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
            //double dz = dy * _sensitivity / 10;
            emit dy > 0 ? zoomInRequested() : zoomOutRequested();
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
                    if (isInViewport (loc)) {
                        double ix = (loc.longitude() / (M_PI * 2)) + 0.5;
                        double iy = (loc.latitude() / M_PI) + 0.5;
                        iy *= _size;
                        ix *= _size * 2;
                        int index =  ((int) iy * _size * 2 + (int) ix);
                        value = _buffer [index];
                        text += ": " + QString::number (ix) + ", " + QString::number (iy);
                        text += ": " + QString::number (index) + " : " + QString::number (value);
                        QToolTip::showText (e->globalPos (), text, this);
                    }
                }
            }
        }
    }
}

void CalenhadMapWidget::wheelEvent (QWheelEvent* event) {
    //double dz =  - event -> delta() * _sensitivity / 12000;
    emit event -> delta() > 0 ? zoomInRequested() : zoomOutRequested();
    update();
}

void CalenhadMapWidget::mouseReleaseEvent (QMouseEvent* e) {
    _zoomDrag = false;
    Geolocation loc;

    // rollback cursor for mouse move without buttons
    if (geoCoordinates (e -> pos(), loc)) {
        setCursor (Qt::CrossCursor);
    } else {
        setCursor (Qt::ArrowCursor);
    }
}


void CalenhadMapWidget::setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mode) {
    _mouseDoubleClickMode = mode;
}

void CalenhadMapWidget::setMouseDragMode (const CalenhadGlobeDragMode& mode) {
    _mouseDragMode = mode;
}


// move the view centre along a given azimuth
void CalenhadMapWidget::navigate (const NavigationEvent& e) {
    // move the viewport centre in the chosen direction by the distance multiplied by the current scale
    double lat, lon;
    double distance = e.distance() * scale();
    _geodesic -> Direct (rotation().latitude (Units::Degrees), rotation().longitude (Units::Degrees), e.azimuth(), distance, lat, lon);
    goTo (Geolocation (lat, lon, Units::Degrees));
}

CalenhadGlobeDoubleClickMode CalenhadMapWidget::mouseDoubleClickMode () {
    return _mouseDoubleClickMode;
}

CalenhadGlobeDragMode CalenhadMapWidget::mouseDragMode () {
    return _mouseDragMode;
}

void CalenhadMapWidget::setCreateHeightMap (const bool& createHeightMap) {
    _createHeightMap = createHeightMap;
}

int CalenhadMapWidget::textureHeight () {
    return _globeTexture -> height();
}

RenderMode CalenhadMapWidget::renderMode () {
    return _mode;
}

void CalenhadMapWidget::setMainMap (CalenhadMapWidget* mainMap) {
    if (_mode == RenderMode::RenderModeOverview) {
        _mainMap = mainMap;
    }
}


void CalenhadMapWidget::initializeGL() {

        initializeOpenGLFunctions();
        glEnable (GL_MULTISAMPLE);
        glClearColor (0, 0, 1, 1);

        _vao.create ();
        if (_vao.isCreated ()) {
            _vao.bind ();
        }

        static const GLfloat g_vertex_buffer_data[] = {
                -1.0f, -1.0f,
                -1.0f, 1.0f,
                1.0f, -1.0f,
                1.0f, 1.0f
        };
        static const GLushort g_element_buffer_data[] = {0, 1, 2, 3};

        _vertexBuffer = new QOpenGLBuffer (QOpenGLBuffer::VertexBuffer);
        _vertexBuffer -> create ();
        _vertexBuffer -> setUsagePattern (QOpenGLBuffer::StaticDraw);
        _vertexBuffer -> bind ();
        _vertexBuffer -> allocate (g_vertex_buffer_data, sizeof (g_vertex_buffer_data));

        _indexBuffer = new QOpenGLBuffer (QOpenGLBuffer::IndexBuffer);
        _indexBuffer -> create ();
        _indexBuffer -> setUsagePattern (QOpenGLBuffer::StaticDraw);
        _indexBuffer -> bind ();
        _indexBuffer -> allocate (g_element_buffer_data, sizeof (g_element_buffer_data));

        _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
        _vertexShader = new QOpenGLShader (QOpenGLShader::Vertex);
        _vertexShader -> compileSourceCode (_vertexShaderCode);
        _fragmentShader = new QOpenGLShader (QOpenGLShader::Fragment);
        _fragmentShader -> compileSourceCode (_fragmentShaderCode);
        _computeProgram = new QOpenGLShaderProgram ();
        clock_t start = clock ();
        _computeShader -> compileSourceCode (_shader);
        _computeProgram -> removeAllShaders ();
        _computeProgram -> addShader (_computeShader);
        _computeProgram -> link ();
        _computeProgram -> bind ();

        clock_t end = clock ();
        //std::cout << "Compile shader " << ((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0 << " milliseconds\n";
        _renderProgram = new QOpenGLShaderProgram ();
        _renderProgram -> addShader (_vertexShader);
        _renderProgram -> addShader (_fragmentShader);
        _renderProgram -> link ();
        _renderProgram -> bind ();

        GLint posPtr = glGetAttribLocation (_renderProgram -> programId (), "pos");
        glVertexAttribPointer (posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray (posPtr);

        _vao.release ();
}

void CalenhadMapWidget::compute () {
    std::cout << "Mode "<< _mode << "\n";
    unsigned long colorMapBytes = CalenhadServices::preferences() -> calenhad_colormap_buffersize * sizeof (float) * 4;
    if (! context()) { std::cout << "No context"; }

    makeCurrent ();
    createTexture ();
    _vao.bind ();
    if (_computeShader) {
        _computeProgram -> removeAllShaders ();
        if (_computeShader -> compileSourceCode (_shader)) {
            _computeProgram -> addShader (_computeShader);
        } else {
            std::cout << "Compute shader would not compile\n";
        }
    }
    _computeProgram -> bind ();
    _globeTexture -> bind ();

    //GLuint icosphereBuffer = 4;
    //GLuint gridBuffer = 5;

    clock_t tileStart = clock ();
    start = tileStart;
    GLuint inBuffer = 0;
    GLuint colBuffer = 0;
    // create and allocate the colorMapBuffer on the GPU and copy the contents across to them.

    _colorMapBuffer = (GLfloat *) _legend -> colorMapBuffer();
    glGenBuffers (1, &colBuffer);
    if (_colorMapBuffer) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, colBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, colorMapBytes, _colorMapBuffer, GL_DYNAMIC_COPY);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, colBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 6); // unbind
    }

    // checksum for debugging
    float checksum = 0;
    for (int i = 0; i < 2 * _size * _size; i++) {
        checksum += _buffer [i];
    }
    std::cout << "Checksum: " << checksum << "\n";

    // create and allocate the heightMapBuffer on the GPU and copy the contents across to them.
    if (_buffer) {
        long bytes = 2 * _size * _size * sizeof (GLfloat);
        glGenBuffers (1, &inBuffer);
        if (_buffer) {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, inBuffer);
            glBufferData(GL_SHADER_STORAGE_BUFFER, bytes, _buffer, GL_DYNAMIC_COPY);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, inBuffer);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 5); // unbind
        }
    }

    // update paramaters
    _computeProgram -> link();
    _computeProgram -> bind();
    glUseProgram (_computeProgram -> programId());
    static GLint destLoc = glGetUniformLocation (_computeProgram -> programId(), "destTex");
    static GLint cmbsLoc = glGetUniformLocation (_computeProgram -> programId(), "colorMapBufferSize");
    static GLint imageHeightLoc = glGetUniformLocation (_computeProgram -> programId(), "imageHeight");
    static GLint projectionLoc = glGetUniformLocation (_computeProgram -> programId(), "projection");
    static GLint datumLoc = glGetUniformLocation (_computeProgram -> programId(), "datum");
    static GLint sizeLoc = glGetUniformLocation (_computeProgram -> programId(), "size");
    //static GLint vertexCountLoc = glGetUniformLocation (_computeProgram -> programId(), "vertexCount");
    static GLint renderModeLoc = glGetUniformLocation (_computeProgram -> programId(), "mode");
    glUniform1i (destLoc, 0);
    glUniform3f (datumLoc, (GLfloat) _rotation.longitude(), (GLfloat) _rotation.latitude(), (GLfloat) _mode == RenderModeOverview ? _mainMap -> scale() : _scale);
    glUniform1i (projectionLoc, _mode == RenderModeOverview ? _mainMap -> projection() -> id() : _projection -> id ());
    glUniform1i (imageHeightLoc, _mode == RenderModeOverview ? _mainMap -> textureHeight() : _size);
    glUniform1i (cmbsLoc, CalenhadServices::preferences() -> calenhad_colormap_buffersize);
    std::cout << "Source size " << _size * 2 << " x " << _size << "\n";
    glUniform2i (sizeLoc, _size * 2, _size);
    //glUniform1i (vertexCountLoc, CalenhadServices::grid() -> vertexCount ());
    glUniform1i (renderModeLoc, _mode);

    // compute the projection
    int xp = _size / 32;
    glDispatchCompute (xp, xp * 2, 1);
    glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);
    std::cout << "Texture size " << _globeTexture -> width () << " x " << _globeTexture->height () << "  -  ";
    std::cout << "Image size " << width () << " x " << height () << "\n\n";
    clock_t end = clock ();
    _renderTime = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
    std::cout << "Render finished in " << _renderTime << " milliseconds\n\n";
    glDeleteBuffers (1, &inBuffer);
    glDeleteBuffers (1, &colBuffer);
    if (_mode == RenderModeGlobe && _mainMap) {
        emit rendered ();
    }
    _refreshHeightMap = true;
}

void CalenhadMapWidget::render() {
    if (_source && _source -> isComplete ()) {
        if (! _source -> name().isNull()) {
            ComputeService* c = CalenhadServices::compute();
            _size = c -> size();
           if (_buffer) { free (_buffer); }
           _buffer = (GLfloat*) malloc (_size * _size * 2 * sizeof (GLfloat));
           c -> compute (_source, _buffer);
        }

        redraw();
    }
}

Module* CalenhadMapWidget::source() {
    return _source;
}

void CalenhadMapWidget::setSource (Module* qm) {
    if (_source) {
        disconnect(_source, &Node::nodeChanged, this, &CalenhadMapWidget::render);
    }
    _source = qm;
    if (_source) {
        render();
        connect(_source, &Node::nodeChanged, this, &CalenhadMapWidget::render);
    }
    update();
}

Legend* CalenhadMapWidget::legend () {
    return _legend;
}

void CalenhadMapWidget::showEvent (QShowEvent* event) {
    update();
}


