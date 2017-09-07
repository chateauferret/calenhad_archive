#include "CalenhadMapWidget.h"
#include <QIcon>
#include <iostream>
#include "../graph/graph.h"
#include "../CalenhadServices.h"
#include "../preferences/preferences.h"
#include "projection/ProjectionService.h"
#include "projection/Projection.h"
#include <QWindow>

using namespace calenhad;
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
    _altitudeMapBuffer (nullptr),
    _projection (CalenhadServices::projections() -> fetch ("Equirectangular")),
    _zoom (1.0),
    _shader ("") {


    QSurfaceFormat format;
    //format.setSamples(8);
    //format.setVersion(4, 3);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    setContextMenuPolicy(Qt::CustomContextMenu);

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
    if (_altitudeMapBuffer) { delete _altitudeMapBuffer; }
}

void CalenhadMapWidget::initializeGL() {
    initializeOpenGLFunctions();
    //glEnable (GL_MULTISAMPLE);

    glClearColor(0,0,1,1);
    _colorMapBuffer = _graph -> colorMapBuffer();
    _altitudeMapBuffer = _graph -> altitudeMapBuffer();

    // create and allocate any required shared altitudeMapBuffer on the GPU and copy the contents across to them.

    GLuint altitudeMap = 0;
    glGenBuffers (1, &altitudeMap);
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, altitudeMap);
    glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (float) * _graph -> altitudeMapBufferSize(), _altitudeMapBuffer, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, altitudeMap);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    // create and allocate the colorMapBuffer on the GPU and copy the contents across to them.

    GLuint colorMap = 1;
    glGenBuffers (1, &colorMap);
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, colorMap);
    glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (float) *  _graph -> colorMapBufferSize(), _colorMapBuffer, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, colorMap);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 1); // unbind

    m_vao.create();
    if (m_vao.isCreated()){
        m_vao.bind();
        qDebug() << "VAO created!";
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
    m_vertexBuffer->allocate(g_vertex_buffer_data,sizeof(g_vertex_buffer_data));

    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_indexBuffer->create();
    m_indexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer->bind();
    m_indexBuffer->allocate(g_element_buffer_data,sizeof(g_element_buffer_data));

    glActiveTexture(GL_TEXTURE0);
    //m_texture = new QOpenGLTexture(QImage(QDir::currentPath()+"/../OpenGLComputeExample/example_image.png").mirrored());
    m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_texture->create();
    m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture->setSize (2048, 1024);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->allocateStorage();
    m_texture->bind();

    glBindImageTexture(0, m_texture->textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    qDebug() << m_texture->width() << m_texture->height();
    m_computeShader = new QOpenGLShader(QOpenGLShader::Compute);
    //m_computeShader -> compileSourceFile (":/shaders/map_cs.glsl");
    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    m_vertexShader->compileSourceFile(":/shaders/map_vs.glsl");
    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    m_fragmentShader -> compileSourceFile (":/shaders/map_fs.glsl");
    m_computeProgram = new QOpenGLShaderProgram();

    if (! m_computeShader) {
        m_computeShader = new QOpenGLShader(QOpenGLShader::Compute);
    }

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
    glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posPtr);

    m_vao.release();
}

void CalenhadMapWidget::paintGL() {

    static GLint srcLoc= glGetUniformLocation(m_renderProgram->programId(),"srcTex");
    static GLint destLoc=glGetUniformLocation(m_computeProgram->programId(),"destTex");
    static GLint ambsLoc = glGetUniformLocation (m_computeProgram -> programId(), "altitudeMapBufferSize");
    static GLint cmbsLoc = glGetUniformLocation (m_computeProgram -> programId(), "colorMapBufferSize");
    static GLint zoomLoc = glGetUniformLocation (m_computeProgram -> programId(), "zoom");
    static GLint projLoc = glGetUniformLocation (m_computeProgram -> programId(), "projection");
    static GLint resolutionLoc = glGetUniformLocation (m_computeProgram -> programId(), "resolution");
    static GLint projectionLoc = glGetUniformLocation (m_computeProgram -> programId(), "projection");

    // compute
    m_vao.bind();
    m_computeProgram->bind();
    m_texture->bind();

    glUniform1i (destLoc,0);
    glUniform1i (projLoc, _projection -> id ());
    glUniform1i (resolutionLoc, m_texture -> height());
    glUniform1i (projectionLoc, _projection -> id());

    // set the zoom factor
    if (zoomLoc != -1) {
        glUniform1f (zoomLoc, (GLfloat) _zoom);
    }

    // set the altitude map buffer size, if there is one
    if (ambsLoc != -1) {
        int ambs = 2048;
        glUniform1i (ambsLoc, ambs);
    }

    // set the color map buffer size
    if (cmbsLoc != -1) {
        int cmbs = 2048;
        glUniform1i (cmbsLoc, cmbs);
    }

    glDispatchCompute (m_texture -> width() / 16, m_texture -> height() / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // draw
    m_renderProgram->bind();
    //glClear(GL_COLOR_BUFFER_BIT);
    m_texture->bind();
    glUniform1i(srcLoc,0);
    glDrawElements (GL_TRIANGLE_STRIP,4,GL_UNSIGNED_SHORT,0);

    m_vao.release();
}

void CalenhadMapWidget::resizeGL(int width, int height)
{
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0, 1, 0, 1, 0, 1);
//    glMatrixMode(GL_MODELVIEW);
}

// Insert the given code into the compute shader to realise the noise pipeline
void CalenhadMapWidget::setGraph (Graph* g) {
    if (g != _graph) {
        // read template compute shader file
        QFile file (":/shaders/map_cs.glsl");
        file.open (QIODevice::ReadOnly);
        QTextStream textStream (&file);
        _shader = textStream.readAll ();
        QString code = g->glsl ();
        _shader.replace ("// inserted code //", code);
        _shader.replace ("// inserted projections //", CalenhadServices::projections() -> glsl());
        _graph = g;
        std::cout << _shader.toStdString () << "\n";
        if (m_computeShader) {
            m_computeShader->compileSourceCode (_shader);
            m_computeProgram->removeAllShaders ();
            m_computeProgram->addShader (m_computeShader);
            m_computeProgram->link ();
            m_computeProgram->bind ();
        }
    }
}

void CalenhadMapWidget::showEvent (QShowEvent* e) {
    update();
}


void CalenhadMapWidget::setZoom (const double& zoom) {
    _zoom = zoom;
}

double CalenhadMapWidget::zoom() {
    return _zoom;
}

void CalenhadMapWidget::setProjection (const QString& projection) {
    _projection = CalenhadServices::projections () -> fetch (projection);
    update();
}

Projection* CalenhadMapWidget::projection() {
    return _projection;
}