//
// Created by martin on 17/10/2019.
//

#include <CalenhadServices.h>
#include "CalenhadMapWidget.h"
#include "projection/ProjectionService.h"
#include "../graph/graph.h"
#include "../icosphere/icosphere.h"
#include "../module/Module.h"

using namespace calenhad::mapping;
using namespace calenhad::graph;
using namespace calenhad::controls::globe;
using namespace calenhad::module;
using namespace calenhad::legend;

CalenhadMapWidget::CalenhadMapWidget (const RenderMode& mode, QWidget* parent) : AbstractMapWidget (mode, parent) {

}

CalenhadMapWidget::~CalenhadMapWidget () {

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

    unsigned long colorMapBytes = sizeof (GLfloat) * _graph -> colorMapBufferSize ();
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



        // create and allocate the colorMapBuffer on the GPU and copy the contents across to them.
        _colorMapBuffer = (GLfloat*) _graph->colorMapBuffer ();
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
            int vertexBytes = sizeof (GLfloat) * CalenhadServices::icosphere ()->vertexCount () * 4;
            _icosphereBuffer = (GLfloat*) _source->vertexBuffer ();
            glGenBuffers (1, &icosphereBuffer);
            glBindBuffer (GL_SHADER_STORAGE_BUFFER, icosphereBuffer);
            glBufferData (GL_SHADER_STORAGE_BUFFER, vertexBytes, _icosphereBuffer, GL_DYNAMIC_COPY);
            glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 4, icosphereBuffer);
            glBindBuffer (GL_SHADER_STORAGE_BUFFER, 1); // unbind

            // compute values for the icosphere's vertices

            clock_t vertexStart = clock ();
            std::cout << "Compute icosphere vertices\n";
            static GLint renderModeLoc = glGetUniformLocation (_computeProgram->programId (), "mode");
            glUniform1i (renderModeLoc, RenderMode::RenderModeVertices);
            int n = CalenhadServices::icosphere ()->vertexCount ();
            int x = n / 256 + 1;
            glDispatchCompute (x / 32, 8, 1);
            glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

            // retrieve icosphere buffer from the GPU

            glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 4, icosphereBuffer);
            glBindBuffer (GL_SHADER_STORAGE_BUFFER, icosphereBuffer);

            glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, vertexBytes, _icosphereBuffer);
            clock_t vertexEnd = clock ();
            clock_t vertexTime = (int) (((double) vertexEnd - (double) vertexStart) / CLOCKS_PER_SEC * 1000.0);
            std::cout << "Computed " << CalenhadServices::icosphere ()->vertexCount () << " vertices in " << vertexTime
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
    connect (qm, &Node::nodeChanged, this, &AbstractMapWidget::render);
}

Legend* CalenhadMapWidget::legend () {
    return _source -> legend();
}

