//
// Created by martin on 21/11/2019.
//
#include <iostream>
#include <src/CalenhadServices.h>
#include "ComputeService.h"
/*

calenhad::graph::ComputeService::ComputeService () {
    // read cuda code from files into memory for use at compute time
    QDirIterator it(":/cuda");
    while (it.hasNext()) {
        QFile csFile (it.next ());
        csFile.open (QIODevice::ReadOnly);
        QTextStream csTextStream (&csFile);
        _templates.insert (csFile.fileName(), csTextStream.readAll());
    }

}
*/

using namespace calenhad::graph;

ComputeService::ComputeService () : _computeProgram (nullptr), _computeShader (nullptr) {
    // read cuda code from files into memory for use at compute time
    QFile csFile (":/shaders/compute.glsl");
    csFile.open (QIODevice::ReadOnly);
    QTextStream csTextStream (&csFile);
    _sourceCode = csTextStream.readAll();

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);


    _context.setFormat(format);
    if (!_context.create())
        throw std::runtime_error("context creation failed");


    _surface.create();
    _context.makeCurrent( &_surface);

    QOpenGLFunctions_4_3_Core openglFunctions;
    if (!openglFunctions.initializeOpenGLFunctions())
        throw std::runtime_error("initialization failed");

}

ComputeService::~ComputeService () {
    delete _computeShader;
    delete _computeProgram;
}

float* ComputeService::compute (Graph* g, float* buffer, int height) {
    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram ();
    clock_t start = clock ();

    QString code = g -> glsl ();
    if (code != QString::null) {
        _sourceCode.replace ("// inserted code //", code);

        if (_computeShader) {
            _computeProgram -> removeAllShaders ();
            if (_computeShader -> compileSourceCode (_sourceCode)) {
                _computeProgram -> addShader (_computeShader);
                _computeProgram -> link ();
                _computeProgram -> bind ();
                std::cout << "created shader for " << g -> glsl ().toStdString () << "\n";

                execute (buffer, height);
            } else {
                std::cout << "Compute shader would not compile\n";
            }
        }
    } else {
        std::cout << "No render code for compute shader\n";
    }
}

void ComputeService::execute (float* buffer, int height) {
    _context.makeCurrent (&_surface);
    GLuint heightMap;
    int heightBytes = 2 * height * height * sizeof (GLfloat);

    QOpenGLFunctions_4_3_Core* f = dynamic_cast<QOpenGLFunctions_4_3_Core*> (_context.versionFunctions ());
    f -> glGenBuffers (1, &heightMap);
    f -> glUseProgram (_computeProgram -> programId ());
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * height * height, NULL, GL_DYNAMIC_READ);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, heightMap);
    f -> glDispatchCompute (32, 64, 1);
    f -> glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);
    f -> glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, heightBytes, buffer);
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);

}


