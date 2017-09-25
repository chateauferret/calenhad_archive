#include "CalenhadMapWidget.h"
#include <QIcon>
#include "../icosphere/Bounds.h"
#include "../graph/graph.h"
#include "../CalenhadServices.h"
#include "../preferences/preferences.h"
#include "projection/ProjectionService.h"
#include "projection/Projection.h"
#include <QWindow>
#include <QtXml/QtXml>
#include <QtGui/QPainter>

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
    m_texture (nullptr),
    m_renderProgram (nullptr),
    m_indexBuffer (nullptr),
    _colorMapBuffer (nullptr),
    _projection (CalenhadServices::projections() -> fetch ("Equirectangular")),
    _scale (1.0),
    _shader (""),
    _graticule (true),
    _inset (false) {


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
    
}

CalenhadMapWidget::~CalenhadMapWidget() {
    makeCurrent();
    if (m_computeShader) { delete m_computeShader; }
    if (m_vertexShader) { delete m_vertexShader; }
    if (m_fragmentShader) { delete m_fragmentShader; }
    if (m_texture) { delete m_texture; }
    if (m_renderProgram) { delete m_renderProgram; }
    if (m_computeProgram) { delete m_computeProgram; }
    if (m_indexBuffer)  { delete m_indexBuffer; }
    if (m_vertexBuffer) { delete m_vertexBuffer; }
}

void CalenhadMapWidget::initializeGL() {
    initializeOpenGLFunctions();
    //glEnable (GL_MULTISAMPLE);

    glClearColor(0, 0, 1, 1);
    _colorMapBuffer = _graph -> colorMapBuffer();


    m_vao.create();
    if (m_vao.isCreated()){
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
    m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_texture->create();
    m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture->setSize (2048, 1024);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->allocateStorage();
    m_texture->bind();

    glBindImageTexture (0, m_texture -> textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

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

    m_vao.bind();
    m_computeProgram->bind();
    m_texture->bind();

    GLubyte c = 0;
    std::vector<GLubyte> emptyData (m_texture -> width() * m_texture -> height() * 4, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_texture -> width(), m_texture -> height(), GL_BGRA, GL_UNSIGNED_BYTE, &emptyData[0]);

    std::cout << "Repaint with projection " << _projection -> name().toStdString() << "\n";

    glUniform1i (destLoc, 0);
    glUniform1i (insetLoc, 1);
    glUniform2f (datumLoc, (GLfloat) _rotation.longitude(), (GLfloat) _rotation.latitude());
    glUniform1i (projectionLoc, _projection -> id ());
    glUniform1f (scaleLoc, (GLfloat) _scale);
    glUniform1i (insetHeightLoc, _inset ? 192 : 0);
    glUniform1i (resolutionLoc, m_texture -> height());
    glUniform1i (cmbsLoc, 2048);

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

    glDispatchCompute (m_texture->width () / 16, m_texture->height () / 16, 1);
    glMemoryBarrier (GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // draw
    m_renderProgram -> bind();
    //glClear(GL_COLOR_BUFFER_BIT);
    m_texture->bind();

    glUniform1i(srcLoc, 0);
    glDrawElements (GL_TRIANGLE_STRIP,4,GL_UNSIGNED_SHORT,0);
    m_vao.release();

    p.endNativePainting();

    if (_graticule) {
        drawGraticule (p);
    }
}

void CalenhadMapWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
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
            m_computeProgram -> removeAllShaders ();
            if (m_computeShader -> compileSourceCode (_shader)) {
                m_computeProgram->addShader (m_computeShader);
                m_computeProgram->link ();
                m_computeProgram->bind ();
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

Geolocation CalenhadMapWidget::rotation() {
    return _rotation;
}

QPointF CalenhadMapWidget::translation() {
    return _translation;
}

void CalenhadMapWidget::setTranslation (const QPointF& translation) {
    _translation = translation;
}


Bounds CalenhadMapWidget::bounds() {

}

void CalenhadMapWidget::setInset (bool inset) {
    _inset = inset;
}


QList<double> CalenhadMapWidget::graticules() {
    QList<double> list;
    list.append ({ 30, 15, 5, 1, 0.5, 0.25, 0.1, 0.05, 0.025, 0.01, 0.005, 0.0025, 0.001 });
    return list;
}

void CalenhadMapWidget::drawGraticule (QPainter& p) {
    p.setPen(Qt::yellow);
    Geolocation centre = _rotation;
    QList<double> g = graticules();
    int dLat = 10, dLon = 10;
    double resLat = 0, resLon = 0;

}

void CalenhadMapWidget::screenCoordinates (Geolocation geolocation, QPointF& screenCoordinates) {
    // to do
}

bool CalenhadMapWidget::geoCoordinates (QPointF pos, Geolocation& geolocation) {
    double x = ((pos.x() / width()) * M_PI * 2 - M_PI) * _scale;
    double y = ((pos.y() / height()) * M_PI - (M_PI / 2)) * _scale;
    _projection -> setDatum (_rotation);
    bool result = _projection -> inverse (QPointF (x, -y), geolocation);
    return result;
}