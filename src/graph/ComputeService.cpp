//
// Created by martin on 21/11/2019.
//
#include <iostream>
#include <src/CalenhadServices.h>
#include "ComputeService.h"
#include "../module/Module.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"
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
using namespace calenhad::module;
using namespace calenhad::nodeedit;

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
    if (!openglFunctions.initializeOpenGLFunctions()) {
        throw std::runtime_error ("initialization failed");
    }
    f = dynamic_cast<QOpenGLFunctions_4_3_Core*> (_context.versionFunctions ());

}

ComputeService::~ComputeService () {
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    f -> glDeleteBuffers (1, &heightMap);
    delete _computeShader;
    delete _computeProgram;
}

float* ComputeService::compute (Module* module) {

    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram ();
    clock_t start = clock ();

    QString code = module -> glsl();

    if (code != QString::null) {
        _sourceCode.replace ("// inserted code //", code);
        std::cout << "Module " << module -> name().toStdString() << " : " << code.toStdString() << "\n";
        if (_computeShader) {
            _computeProgram -> removeAllShaders ();
            if (_computeShader -> compileSourceCode (_sourceCode)) {
                _computeProgram -> addShader (_computeShader);
                _computeProgram -> link ();
                _computeProgram -> bind ();
                execute (module);
            } else {
                std::cout << "Compute shader would not compile\n";
            }
        }
    } else {
        std::cout << "No render code for compute shader\n";
    }
}

void ComputeService::execute (Module* module) {
    float* buffer = module -> buffer();
    std::cout << "Rendering " << module -> name().toStdString() << " to buffer " << buffer << "\n";
    size_t height = module -> rasterHeight();

    int bytes = 2 * height * height * sizeof (GLfloat);

    f -> glGenBuffers (1, &heightMap);
    f -> glUseProgram (_computeProgram -> programId ());
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, heightMap);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * height * height, NULL, GL_DYNAMIC_READ);


    int i = 0;
    for (Port* port : module -> inputs ()) {
        QString index = QString::number (i++);
        if (! port -> connections().isEmpty ()) {
            Node* other = port -> connections() [0] -> otherEnd (port) -> owner();
            Module* m = dynamic_cast<Module*> (other);
            if (m)  {
                GLuint inputBuffer;
                f -> glGenBuffers (1, &inputBuffer);
                f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, inputBuffer);
                f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * height * height, m -> buffer(), GL_DYNAMIC_COPY);
                f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1 + i, inputBuffer);
            }
        }
    }

    f -> glDispatchCompute (32, 64, 1);
    f -> glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, heightMap);
    float *ptr = (float*) f -> glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, sizeof (GLfloat) * 2 * height * height, GL_MAP_READ_BIT );
    memcpy (buffer, ptr, sizeof (GLfloat) * 2 * height * height);
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    //f -> glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, bytes, buffer);
    std::cout << "Buffer for module " << module -> name().toStdString() << ": at " << buffer << ": checksum ";
    float checksum = 0;
    for (int x = 0; x < 2048 * 1024; x++) {
        checksum += ptr [x];
    }
    std::cout << checksum << "\n";
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, 0);
}

