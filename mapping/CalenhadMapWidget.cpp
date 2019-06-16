#include "CalenhadMapWidget.h"
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
#include "../qmodule/Module.h"
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
using namespace calenhad::qmodule;
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
        connect (this, &CalenhadMapWidget::rendered, this, [=] () { if (_mainMap) { _mainMap -> update(); }});
    }
}

CalenhadMapWidget::~CalenhadMapWidget() {
    makeCurrent();
    if (_computeShader) { delete _computeShader; }
    if (_vertexShader) { delete _vertexShader; }
    if (_fragmentShader) { delete _fragmentShader; }
    if (_globeTexture) { delete _globeTexture; }
    if (_heightMapBuffer) { delete _heightMapBuffer; }
    if (_renderProgram) { delete _renderProgram; }
    if (_computeProgram) { delete _computeProgram; }
    if (_indexBuffer)  { delete _indexBuffer; }
    if (_vertexBuffer) { delete _vertexBuffer; }
    if (_graticule) { delete _graticule; }
    delete _geodesic;
}

void CalenhadMapWidget::initializeGL() {

    if (_graph) {
        initializeOpenGLFunctions ();
        glEnable (GL_MULTISAMPLE);

        glClearColor (0, 0, 1, 1);
        _colorMapBuffer = _graph->colorMapBuffer ();

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
        _vertexBuffer->create ();
        _vertexBuffer->setUsagePattern (QOpenGLBuffer::StaticDraw);
        _vertexBuffer->bind ();
        _vertexBuffer->allocate (g_vertex_buffer_data, sizeof (g_vertex_buffer_data));

        _indexBuffer = new QOpenGLBuffer (QOpenGLBuffer::IndexBuffer);
        _indexBuffer->create ();
        _indexBuffer->setUsagePattern (QOpenGLBuffer::StaticDraw);
        _indexBuffer->bind ();
        _indexBuffer->allocate (g_element_buffer_data, sizeof (g_element_buffer_data));

        _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
        _vertexShader = new QOpenGLShader (QOpenGLShader::Vertex);
        _vertexShader->compileSourceCode (_vertexShaderCode);
        _fragmentShader = new QOpenGLShader (QOpenGLShader::Fragment);
        _fragmentShader->compileSourceCode (_fragmentShaderCode);
        _computeProgram = new QOpenGLShaderProgram ();
        clock_t start = clock ();
        _computeShader->compileSourceCode (_shader);
        _computeProgram->removeAllShaders ();
        _computeProgram->addShader (_computeShader);
        _computeProgram->link ();
        _computeProgram->bind ();

        clock_t end = clock ();
        std::cout << "Compile shader " << ((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0 << " milliseconds\n";
        _renderProgram = new QOpenGLShaderProgram ();
        _renderProgram->addShader (_vertexShader);
        _renderProgram->addShader (_fragmentShader);
        _renderProgram->link ();
        _renderProgram->bind ();

        GLint posPtr = glGetAttribLocation (_renderProgram->programId (), "pos");
        glVertexAttribPointer (posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray (posPtr);

        _vao.release ();
    }
}

void CalenhadMapWidget::compute () {
    QCursor oldCursor = cursor ();
    setCursor (Qt::BusyCursor);
    //if (_interactive && _tileX > 0 && _tileY > 0) { updateParams(); return; }
    int vertexBytes = sizeof (GLfloat) * CalenhadServices::icosphere() -> vertexCount() * 4;
    int colorMapBytes = sizeof (GLfloat) * _graph -> colorMapBufferSize();
    if (_graph) {

        makeCurrent ();
        createTexture ();
        _vao.bind ();
        _computeProgram->bind ();
        _globeTexture->bind ();

        GLuint icosphereBuffer = 4;
        GLuint colorMap = 1;


            clock_t tileStart = clock ();

            start = tileStart;

            // if we want an icosphere computed, upload the vertex coordinates and buffer for results
            if (_computeVertices) {
                _icosphereBuffer = (GLfloat*) _source -> vertexBuffer();
                glGenBuffers (1, &icosphereBuffer);
                glBindBuffer (GL_SHADER_STORAGE_BUFFER, icosphereBuffer);
                glBufferData (GL_SHADER_STORAGE_BUFFER, vertexBytes, _icosphereBuffer, GL_DYNAMIC_COPY);
                glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 4, icosphereBuffer);
                glBindBuffer (GL_SHADER_STORAGE_BUFFER, 1); // unbind
            }

            // create and allocate the colorMapBuffer on the GPU and copy the contents across to them.
            _colorMapBuffer = (GLfloat*) _graph -> colorMapBuffer();
            if (_colorMapBuffer) {
                glGenBuffers (1, &colorMap);
                glBindBuffer (GL_SHADER_STORAGE_BUFFER, colorMap);
                glBufferData (GL_SHADER_STORAGE_BUFFER, colorMapBytes, _colorMapBuffer, GL_DYNAMIC_COPY);
                glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, colorMap);
                glBindBuffer (GL_SHADER_STORAGE_BUFFER, 2); // unbind
            }

            prepareRasters ();

            int xp = _tileSize / 32;
            updateParams ();
            glDispatchCompute (xp, xp * 2, 1);
            glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

            if (_computeVertices) {

                // compute values for the icosphere's vertices

                clock_t vertexStart = clock ();
                std::cout << "Compute icosphere vertices\n";
                static GLint renderModeLoc = glGetUniformLocation (_computeProgram -> programId(), "mode");
                glUniform1i (renderModeLoc, RenderMode::RenderModeVertices);
                int n = CalenhadServices::icosphere()->vertexCount ();
                int x = n / 256 + 1;
                glDispatchCompute (x / 32, 8, 1);
                glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

                // retrieve icosphere buffer from the GPU

                glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 4, icosphereBuffer);
                glBindBuffer (GL_SHADER_STORAGE_BUFFER, icosphereBuffer);

                glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, vertexBytes, _icosphereBuffer);
                clock_t vertexEnd = clock ();
                clock_t vertexTime = (int) (((double) vertexEnd - (double) vertexStart) / CLOCKS_PER_SEC * 1000.0);
                std::cout << "Computed " << vertexBytes << " data for " << CalenhadServices::icosphere ()->vertexCount () << " vertices in " << vertexTime
                          << " ms\n";
                glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
                _computeVertices = false;

            }

            // retrieve the height map data from the GPU
            // only do this after last tile because it takes about a thirtieth of a second
            if (_globeTexture && _heightMapBuffer && _createHeightMap) {
                if (_refreshHeightMap) {
                    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 3, heightMap);
                    glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
                    int h = _globeTexture->height ();
                    int heightBytes = 2 * h * h * sizeof (GLfloat);
                    glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, heightBytes, _heightMapBuffer);
                    _refreshHeightMap = false;
                    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
                }
            }

            std::cout << "Texture size " << _globeTexture->width () << " x " << _globeTexture->height () << "  -  ";
            std::cout << "Image size " << width () << " x " << height () << "\n\n";
            clock_t end = clock ();
            _renderTime = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
            std::cout << "Render fnished in " << _renderTime << " milliseconds\n\n";

        if (_mode == RenderMode::RenderModeGlobe && _mainMap) {
            emit rendered ();
        }
        _refreshHeightMap = true;
    }

    setCursor (oldCursor);
}

void CalenhadMapWidget::prepareRasters () {
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
        _rasterTexture->setSize (CalenhadServices::preferences ()->calenhad_globe_texture_height, CalenhadServices::preferences ()->calenhad_globe_texture_height);
        _rasterTexture->setMinificationFilter (QOpenGLTexture::Linear);
        _rasterTexture->setMagnificationFilter (QOpenGLTexture::Linear);
        _rasterTexture->allocateStorage ();
        _rasterTexture->bind ();
        glBindImageTexture (1, _rasterTexture->textureId (), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
        for (int i = 0; i < rasters; i++) {
            QImage* raster = _graph->raster (i);
            _rasterTexture->setData (0, i, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, (void*) raster->bits ());
        }
    }
}

void CalenhadMapWidget::updateParams() {
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

void CalenhadMapWidget::paintGL() {
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

GLfloat* CalenhadMapWidget::heightMapBuffer() {
    return _heightMapBuffer;
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

// Insert the given code into the compute shader to realise the noise pipeline
void CalenhadMapWidget::setGraph (Graph* g) {
    if (_graph != g) {
        _graph = g;
        makeCurrent ();
        _shader = _shaderTemplate;
        QString code = g->glsl ();
        if (code != QString::null) {
            _code = code;
            _shader.replace ("// inserted code //", _code);
            _shader.replace ("// inserted inverse //", CalenhadServices::projections() -> glslInverse ());
            _shader.replace ("// inserted forward //", CalenhadServices::projections() -> glslForward ());
            //std::cout << _shader.toStdString () << "\n";
            if (_computeShader) {
                _computeProgram -> removeAllShaders ();
                if (_computeShader -> compileSourceCode (_shader)) {
                    _computeProgram -> addShader (_computeShader);
                } else {
                    std::cout << "Compute shader would not compile\n";
                }
            }
        } else {
            std::cout << "No render code for compute shader\n";
        }
    }
    _computeVertices = true;
    redraw();
}

void CalenhadMapWidget::showEvent (QShowEvent* e) {
    //updateGL();
}


void CalenhadMapWidget::setScale (const double& scale) {
    _scale = scale;
    redraw();
}

double CalenhadMapWidget::scale () {
    return _scale;
}

void CalenhadMapWidget::redraw() {
    update();
}

void CalenhadMapWidget::setProjection (const QString& projection) {
    _projection = CalenhadServices::projections () -> fetch (projection);
    redraw();
}

Projection* CalenhadMapWidget::projection() {
    return _projection;
}

void CalenhadMapWidget::rotate (const Geolocation& rotation) {
    _rotation = rotation;
    redraw();
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

QImage* CalenhadMapWidget::heightmap() {
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

bool CalenhadMapWidget::valueAt (const QPointF& sc, double& value) {
    QPoint tc = texCoordinates (sc);
    int index = tc.y() * _globeTexture -> width () + tc.x();
    if (index >= 0 && index < _globeTexture -> width () * _globeTexture -> height()) {
        value = (GLfloat) heightMapBuffer() [index];
        return true;
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

bool CalenhadMapWidget::screenCoordinates (Geolocation geolocation, QPointF& sc) {
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

bool CalenhadMapWidget::isGraticuleVisible() {
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

void CalenhadMapWidget::paintEvent (QPaintEvent* e) {
    QOpenGLWidget::paintEvent (e);
}

void CalenhadMapWidget::render() {
    // if the name isn't set, module is still being built, so don't render it
    if (! _source -> name().isNull()) {
        if (_source->isComplete () && !_source->renderSuppressed ()) {
            Graph* g = new Graph (_source);
            setGraph (g);
        }
    }
}

Module* CalenhadMapWidget::source() {
    return _source;
}

void CalenhadMapWidget::setSource (Module* qm) {
    _source = qm;

    render();
    connect (qm, &Node::nodeChanged, this, &CalenhadMapWidget::render);
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

double CalenhadMapWidget::sensitivity () {
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

void CalenhadMapWidget::wheelEvent (QWheelEvent* event) {
    double dz =  - event -> delta() * _sensitivity / 1200;
    emit zoomRequested (_scale + dz);
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
