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
#include "Graticule.h"


using namespace calenhad;
using namespace geoutils;
using namespace matrices;
using namespace icosphere;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::mapping::projection;

CalenhadMapWidget::CalenhadMapWidget (QWidget* parent) : QOpenGLWidget (parent),
    m_vertexBuffer (nullptr),
    m_computeShader (nullptr),
    m_computeProgram (nullptr),
    m_vertexShader (nullptr),
    m_fragmentShader (nullptr),
    _globeTexture (nullptr),
    m_renderProgram (nullptr),
    m_indexBuffer (nullptr),
    _colorMapBuffer (nullptr),
    _projection (CalenhadServices::projections() -> fetch ("Equirectangular")),
    _scale (1.0),
    _shader (""),
    _graticule (nullptr),
    _graticuleVisible (true),
    _coordinatesFormat (CoordinatesFormat::Traditional),
    _inset (false),
    _rotation (Geolocation (0, 0)),
    _insetHeight (CalenhadServices::preferences() -> calenhad_globe_inset_height),
    _insetPos (QPoint (CalenhadServices::preferences() -> calenhad_globe_inset_x, CalenhadServices::preferences() -> calenhad_globe_inset_y)) {


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
    _vertexShader = vsTextStream.readAll ();

    QFile fsFile (":/shaders/map_fs.glsl");
    fsFile.open (QIODevice::ReadOnly);
    QTextStream fsTextStream (&fsFile);
    _fragmentShader = fsTextStream.readAll ();

    _graticule = new Graticule (this);

}

CalenhadMapWidget::~CalenhadMapWidget() {
    makeCurrent();
    if (m_computeShader) { delete m_computeShader; }
    if (m_vertexShader) { delete m_vertexShader; }
    if (m_fragmentShader) { delete m_fragmentShader; }
    if (_globeTexture) { delete _globeTexture; }
    if (m_renderProgram) { delete m_renderProgram; }
    if (m_computeProgram) { delete m_computeProgram; }
    if (m_indexBuffer)  { delete m_indexBuffer; }
    if (m_vertexBuffer) { delete m_vertexBuffer; }
    if (_graticule) { delete _graticule; }
}

void CalenhadMapWidget::initializeGL() {
    initializeOpenGLFunctions();
    //glEnable (GL_MULTISAMPLE);

    glClearColor(0, 0, 1, 1);
    _colorMapBuffer = _graph -> colorMapBuffer();


    m_vao.create();
    if (m_vao.isCreated()) {
        m_vao.bind();
    }

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };
    static const GLushort g_element_buffer_data[] = { 0, 1, 2, 3 };

    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBuffer->create();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer->bind();
    m_vertexBuffer->allocate (g_vertex_buffer_data, sizeof(g_vertex_buffer_data));

    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_indexBuffer->create();
    m_indexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer->bind();
    m_indexBuffer->allocate (g_element_buffer_data, sizeof(g_element_buffer_data));

    // texture for the map
    glActiveTexture(GL_TEXTURE0);
    _globeTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    _globeTexture->create();
    _globeTexture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    _globeTexture->setSize (2048, 1024);
    _globeTexture->setMinificationFilter(QOpenGLTexture::Linear);
    _globeTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    _globeTexture->allocateStorage();
    _globeTexture->bind();
    _heightMapBuffer = new float [_globeTexture -> height() * _globeTexture -> width()];

    glBindImageTexture (0, _globeTexture -> textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    m_computeShader = new QOpenGLShader(QOpenGLShader::Compute);
    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    m_vertexShader -> compileSourceCode (_vertexShader);
    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    m_fragmentShader -> compileSourceCode (_fragmentShader);
    m_computeProgram = new QOpenGLShaderProgram();

    m_computeShader -> compileSourceCode (_shader);
    m_computeProgram -> removeAllShaders();
    m_computeProgram -> addShader (m_computeShader);
    m_computeProgram -> link();
    m_computeProgram->bind();

    m_renderProgram = new QOpenGLShaderProgram();
    m_renderProgram->addShader(m_vertexShader);
    m_renderProgram->addShader(m_fragmentShader);
    m_renderProgram->link();
    m_renderProgram->bind();

    GLint posPtr = glGetAttribLocation(m_renderProgram->programId(), "pos");
    glVertexAttribPointer (posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (posPtr);

    m_vao.release();
}

void CalenhadMapWidget::paintGL() {

    QPainter p (this);
    p.beginNativePainting();

    static GLint srcLoc= glGetUniformLocation(m_renderProgram->programId(),"srcTex");
    static GLint destLoc=glGetUniformLocation(m_computeProgram->programId(),"destTex");
    static GLint insetLoc = glGetUniformLocation (m_computeProgram -> programId(), "insetTex");
    static GLint cmbsLoc = glGetUniformLocation (m_computeProgram -> programId(), "colorMapBufferSize");
    static GLint resolutionLoc = glGetUniformLocation (m_computeProgram -> programId(), "resolution");
    static GLint projectionLoc = glGetUniformLocation (m_computeProgram -> programId(), "projection");
    static GLint scaleLoc = glGetUniformLocation (m_computeProgram -> programId (), "scale");
    static GLint datumLoc = glGetUniformLocation (m_computeProgram -> programId(), "datum");
    static GLint insetHeightLoc = glGetUniformLocation (m_computeProgram -> programId(), "insetHeight");
    static GLint insetPosLoc = glGetUniformLocation (m_computeProgram -> programId(), "insetPos");

    m_vao.bind();
    m_computeProgram->bind();
    _globeTexture->bind();

    GLubyte c = 0;
    std::vector<GLubyte> emptyData (_globeTexture -> width() * _globeTexture -> height() * 4, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _globeTexture -> width(), _globeTexture -> height(), GL_BGRA, GL_UNSIGNED_BYTE, &emptyData[0]);
    glUniform1i (destLoc, 0);
    glUniform1i (insetLoc, 1);
    glUniform2f (datumLoc, (GLfloat) _rotation.longitude(), (GLfloat) _rotation.latitude());
    glUniform1i (projectionLoc, _projection -> id ());
    glUniform1f (scaleLoc, (GLfloat) _scale);
    glUniform1i (insetHeightLoc, _inset ? _insetHeight : 0);
    glUniform1i (resolutionLoc, _globeTexture -> height());
    glUniform1i (cmbsLoc, 2048);
    glUniform2i (insetPosLoc, _insetPos.x(), _insetPos.y());

    // create and allocate the colorMapBuffer on the GPU and copy the contents across to them.
    _colorMapBuffer = _graph -> colorMapBuffer();
    if (_colorMapBuffer) {
        GLuint colorMap = 1;
        glGenBuffers (1, &colorMap);
        glBindBuffer (GL_SHADER_STORAGE_BUFFER, colorMap);
        glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (float) * _graph -> colorMapBufferSize (), _colorMapBuffer, GL_DYNAMIC_COPY);
        glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, colorMap);
        glBindBuffer (GL_SHADER_STORAGE_BUFFER, 1); // unbind
    }

    // create and allocate the heightMapBuffer on the GPU. This is for downloading the heightmap from the GPU.
    GLuint heightMap = 1;
    glGenBuffers (1, &heightMap);
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
    glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * _globeTexture -> height() * _globeTexture -> width(), NULL, GL_DYNAMIC_READ);
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 3, heightMap);
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, 1); // unbind

    glDispatchCompute (_globeTexture->width () / 16, _globeTexture->height () / 16, 1);


    // draw
    m_renderProgram -> bind();
    //glClear(GL_COLOR_BUFFER_BIT);
    _globeTexture -> bind();

    glUniform1i(srcLoc, 0);
    glDrawElements (GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    m_vao.release();

    glMemoryBarrier (GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    p.endNativePainting();

    // draw the graticule
    if (_graticule && _graticuleVisible) {
        _graticule -> drawGraticule (p);
    }

    // retrieve the height map data from the GPU
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 3, heightMap);
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
    GLfloat* heightData = (GLfloat *) glMapBuffer (GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    int zeroes = 0, outwithRange = 0;
    memcpy (_heightMapBuffer, heightData, _globeTexture -> height() * _globeTexture -> width() * sizeof (GLfloat));
    for (int i = 0; i < _globeTexture -> height () * _globeTexture -> width (); i++) {
        //std::cout << i << "      " << _heightMapBuffer [i] << "\n";
        if (_heightMapBuffer [i] < -1.0 || _heightMapBuffer [i] > 1.0) { outwithRange++; }
        if (_heightMapBuffer [i] == 0.0) { zeroes++; }
    }
    std::cout << "Out of range: " << outwithRange << "\n";
    std::cout << "Zeroes: " << zeroes << "\n";
}

float* CalenhadMapWidget::heightMapBuffer() const {
    return _heightMapBuffer;
}

QSize CalenhadMapWidget::heightMapSize() const {
    return QSize (_globeTexture -> width(), _globeTexture -> height());
}


void CalenhadMapWidget::resizeGL(int width, int height) {
    if (height == 0) height = 1;
    float aspectRatio = (float) width / (float)height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); /* switch to projection matrix */
    glLoadIdentity();
    if (width >= height) {
        glOrtho (-0.5f * aspectRatio, 0.5f * aspectRatio, 0.0f, 1.0f, -1, 1);
    }
    else {
        glOrtho (-0.5f, 0.5f, 0.0, 1.0 / aspectRatio, -1, 1);
    }
    glMatrixMode (GL_MODELVIEW);

}

// Insert the given code into the compute shader to realise the noise pipeline
void CalenhadMapWidget::setGraph (Graph* g) {
    makeCurrent();
    _shader = _shaderTemplate;
    QString code = g -> glsl ();
    std::cout << code.toStdString () << "\n";
    _graph = g;

    _shader.replace ("// inserted code //", code);
    _shader.replace ("// inserted inverse //", CalenhadServices::projections() -> glslInverse ());
    _shader.replace ("// inserted forward //", CalenhadServices::projections() -> glslForward ());

    if (m_computeShader) {
        m_computeProgram -> removeAllShaders();
        if (m_computeShader -> compileSourceCode (_shader)) {
            m_computeProgram -> addShader (m_computeShader);
            m_computeProgram -> link();
            m_computeProgram -> bind();
        } else {
            std::cout << "Compute shader would not compile\n";
        }
    } else {
        std::cout << "No compute shader\n";
    }

    update();

}

void CalenhadMapWidget::showEvent (QShowEvent* e) {
    update();
}


void CalenhadMapWidget::setScale (const double& scale) {
    _scale = scale;
}

double CalenhadMapWidget::scale () {
    return _scale;
}

void CalenhadMapWidget::setProjection (const QString& projection) {
    _projection = CalenhadServices::projections () -> fetch (projection);
    update();
}

Projection* CalenhadMapWidget::projection() {
    return _projection;
}

void CalenhadMapWidget::rotate (const Geolocation& rotation) {
    _rotation = rotation;
    update();
}

const Geolocation& CalenhadMapWidget::rotation () {
    return _rotation;
}

Bounds CalenhadMapWidget::bounds() {

}

void CalenhadMapWidget::setInset (bool inset) {
    _inset = inset;
    update();
}

bool CalenhadMapWidget::isInViewport (Geolocation g) {
    QPointF s;
    if (screenCoordinates (g, s)) {
        return s.x() >= 0 && s.x() <= width() && s.y() >= 0 && s.y() < height();
    } else {
        return false;
    }
}

QImage* CalenhadMapWidget::greyscale() {
    QImage* image = new QImage (_globeTexture -> width(), _globeTexture -> width(), QImage::Format_ARGB32);
    image -> fill (Qt::red);
    int w = _globeTexture -> width();
    int h = _globeTexture -> height();
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            float value = (float) _heightMapBuffer [i * w + j];
            value = std::min (std::max (-1.0f, value), 1.0f);
            int k = (int) ((value + 1) / 2 * 256);
            image -> setPixelColor (j, h - i, QColor (k, k, k));
        }
    }
    return image;
}

bool CalenhadMapWidget::valueAt (const QPointF& sc, double& value) {
    QPoint tc = texCoordinates (sc);
    int index = tc.y() * _globeTexture -> width () + tc.x();
    if (index >= 0 && index < _globeTexture -> width () * _globeTexture -> width()) {
        value = (double) _heightMapBuffer [index];
        return true;
    } else {
        return false;
    }
}

QPoint CalenhadMapWidget::texCoordinates (const QPointF& sc) {
    QPoint tc;
    double x = sc.x() / width ();
    double y = sc.y() / height ();
    tc.setX (x * _globeTexture -> width ());
    tc.setY ((1 - y) * _globeTexture -> height ());
    return tc;
}

bool CalenhadMapWidget::screenCoordinates (Geolocation geolocation, QPointF& sc) {
    _projection -> setDatum (_rotation);
    QPointF p;
    bool fwd = _projection -> forward (geolocation, p);
    if (fwd) {
        double x = p.x () / _scale;
        double y = (-p.y ()) / _scale;
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
    double x = ((pos.x() / width()) * M_PI * 2 - M_PI) * _scale;
    double y = ((pos.y() / height()) * M_PI - (M_PI / 2)) * _scale;
    _projection -> setDatum (_rotation);
    bool result = _projection -> inverse (QPointF (x, -y), geolocation);
    return result;
}

bool CalenhadMapWidget::inset () {
    return _inset;
}


QRectF CalenhadMapWidget::insetRect() {
    double h = (_insetHeight / (double) _globeTexture -> height()) * height();
    double x = (_insetPos.x () / (double) _globeTexture -> width()) * width();
    double y = (1 - ((_insetPos.y () + _insetHeight) / (double) _globeTexture -> height())) * height();
    return QRectF (x, y, h * 2, h);
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
