//
// Created by martin on 21/11/2019.
//
#include <iostream>
#include <src/CalenhadServices.h>
#include "ComputeService.h"
#include "../module/Module.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"
#include <cmath>
#include "../grid/Extent.h"

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
using namespace calenhad::grid;

ComputeService::ComputeService () : _computeProgram (nullptr), _computeShader (nullptr), _heightMap (0) {
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
    f = dynamic_cast<QOpenGLFunctions_4_3_Core*> (_context.versionFunctions());
    f -> glGenBuffers (1, &_heightMap);
}

ComputeService::~ComputeService () {
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    f -> glDeleteBuffers (1, &_heightMap);
    delete _computeShader;
    delete _computeProgram;
}

void ComputeService::compute (Module *module) {

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
    Extent* extent = module -> extent();
    float* buffer = extent -> buffer();
    std::cout << "Rendering " << module -> name().toStdString() << " to buffer " << buffer << "\n";
    size_t height = extent -> rasterHeight();

    long bytes = 2 * height * height * sizeof (GLfloat);
    f -> glUseProgram (_computeProgram -> programId ());
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, _heightMap);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * height * height, nullptr, GL_DYNAMIC_READ);

    int i = 0;
    GLuint inputBuffer;
    f -> glGenBuffers (1, &inputBuffer);
    for (Port* port : module -> inputs ()) {
        QString index = QString::number (i);
        if (! port -> connections().isEmpty()) {
            Node* other = port -> connections() [0] -> otherEnd (port) -> owner();
            Module* m = dynamic_cast<Module*> (other);
            if (m)  {
                f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, inputBuffer);
                f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * height * height, m -> extent() -> buffer(), GL_DYNAMIC_COPY);
                f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, i++, inputBuffer);
            }
        }
    }
    int resolution = extent -> resolution();
    int size = std::pow (2, resolution);
    static GLint resolutionLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "resolution");
    f -> glUniform1i (resolutionLoc, size);

    f -> glDispatchCompute (32 * 2, 64 * 2, 1);
    f -> glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 5, _heightMap);

    float *ptr = (float*) f -> glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, sizeof (GLfloat) * 2 * height * height, GL_MAP_READ_BIT);
    memcpy (buffer, ptr, sizeof (GLfloat) * 2 * height * height);
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    //f -> glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, bytes, buffer);
    std::cout << "Buffer for module " << module -> name().toStdString() << ": at " << buffer << ": checksum ";
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, 5);
    f -> glDeleteBuffers (1, &inputBuffer);
}

void ComputeService::setBounds(const Bounds &bounds) {
    _bounds = bounds;
}

