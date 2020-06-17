//
// Created by martin on 17/10/2019.
//

#include <src/CalenhadServices.h>
#include "CalenhadMapWidget.h"
#include "projection/ProjectionService.h"
#include "../graph/graph.h"
#include "../icosphere/icosphere.h"
#include "../icosphere/CubicSphere.h"
#include "projection/Projection.h"
#include "../module/Module.h"

using namespace calenhad::mapping;
using namespace calenhad::graph;
using namespace calenhad::controls::globe;
using namespace calenhad::module;
using namespace calenhad::legend;
using namespace calenhad::grid;

CalenhadMapWidget::CalenhadMapWidget (const RenderMode& mode, QWidget* parent) : AbstractMapWidget (mode, parent),
    _computeCubeMap (false) {
    // read shader code from files into memory for use at render time
    QFile csFile (":/shaders/render.glsl");
    csFile.open (QIODevice::ReadOnly);
    QTextStream csTextStream (&csFile);
    _shader = csTextStream.readAll ();
}

CalenhadMapWidget::~CalenhadMapWidget () {

}


void CalenhadMapWidget::initializeGL() {

        initializeOpenGLFunctions ();
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
    std::cout << "Display " << _source -> name().toStdString () << "\n";
    unsigned long colorMapBytes = CalenhadServices::preferences() -> calenhad_colormap_buffersize * sizeof (float) * 4;
    unsigned long heightMapBytes = sizeof (GLfloat) * _source -> rasterHeight() * _source -> rasterHeight() * 2;

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

    GLuint icosphereBuffer = 4;
    GLuint gridBuffer = 5;
    GLuint colorMap = 1;

    clock_t tileStart = clock ();
    start = tileStart;

    glGenBuffers (1, &colorMap);
    glGenBuffers (1, &heightMap);

    // create and allocate the colorMapBuffer on the GPU and copy the contents across to them.
    _colorMapBuffer = (GLfloat*) _source -> colorMapBuffer ();
    if (_colorMapBuffer) {

        glBindBuffer (GL_SHADER_STORAGE_BUFFER, colorMap);
        glBufferData (GL_SHADER_STORAGE_BUFFER, colorMapBytes, _colorMapBuffer, GL_DYNAMIC_COPY);
        glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, colorMap);
        glBindBuffer (GL_SHADER_STORAGE_BUFFER, 2); // unbind
    }

    // create and allocate the heightMapBuffer on the GPU and copy the contents across to them.
    _heightMapBuffer = (GLfloat*) _source -> buffer();
    if (_heightMapBuffer) {

        glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
        glBufferData (GL_SHADER_STORAGE_BUFFER, heightMapBytes, _heightMapBuffer, GL_DYNAMIC_COPY);
        glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, heightMap);
        glBindBuffer (GL_SHADER_STORAGE_BUFFER, 1); // unbind
    }

    int xp = _tileSize / 32;
    updateParams();
    glDispatchCompute (xp, xp * 2, 1);
    glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);
    //std::cout << "Texture size " << _globeTexture -> width () << " x " << _globeTexture->height () << "  -  ";
    //std::cout << "Image size " << width () << " x " << height () << "\n\n";
    clock_t end = clock ();
    _renderTime = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
    //std::cout << "Render fnished in " << _renderTime << " milliseconds\n\n";

    if (_mode == RenderMode::RenderModeGlobe && _mainMap) {
        emit rendered ();
    }
    _refreshHeightMap = true;
    glDeleteBuffers (1, &colorMap);
    glDeleteBuffers (1, &heightMap);
}

void CalenhadMapWidget::render() {
    // if the name isn't set, module is still being built, so don't render it
    //if (! _source -> name().isNull()) {
        if (_source -> isComplete () && !_source -> renderSuppressed ()) {
            redraw();
        }
   // }
}

Module* CalenhadMapWidget::source() {
    return _source;
}

void CalenhadMapWidget::setSource (Module* qm) {
    _source = qm;
    render();
    connect (qm, &Node::nodeChanged, this, &AbstractMapWidget::render);
}

Legend* CalenhadMapWidget::legend () {
    return _source -> legend();
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
    //static GLint vertexCountLoc = glGetUniformLocation (_computeProgram -> programId(), "vertexCount");
    static GLint renderModeLoc = glGetUniformLocation (_computeProgram -> programId(), "mode");

    glUniform1i (destLoc, 0);
    glUniform3f (datumLoc, (GLfloat) _rotation.longitude(), (GLfloat) _rotation.latitude(), (GLfloat) _mode == RenderMode::RenderModeOverview ? _mainMap -> scale() : _scale);
    glUniform1i (projectionLoc, _mode == RenderMode::RenderModeOverview ? _mainMap -> projection() -> id() : _projection -> id ());
    glUniform1i (imageHeightLoc, _mode == RenderMode::RenderModeOverview ? _mainMap -> textureHeight() : _globeTexture -> height());
    glUniform1i (insetHeightLoc, _globeTexture -> height());
    glUniform1i (cmbsLoc, 2048);
    glUniform1i (rasterResolutionLoc, _source -> rasterHeight());
    //glUniform1i (vertexCountLoc, CalenhadServices::icosphere ()->vertexCount ());
    glUniform1i (renderModeLoc, _mode);
}

void CalenhadMapWidget::showEvent(QShowEvent *event) {
    AbstractMapWidget::showEvent(event);
    update();
}
