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
#include "graph.h"

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
    _codeTemplate = csTextStream.readAll();

    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    _context.setFormat(format);
    if (!_context.create())
        throw std::runtime_error("context creation failed");

    _surface.create();
    _context.makeCurrent( &_surface);
     resolution = 11; //extent -> resolution();
     _size = std::pow (2, resolution);
    QOpenGLFunctions_4_3_Core openglFunctions;
    if (!openglFunctions.initializeOpenGLFunctions()) {
        throw std::runtime_error ("initialization failed");
    }
    f = dynamic_cast<QOpenGLFunctions_4_3_Core*> (_context.versionFunctions());

}

ComputeService::~ComputeService () {

    delete _computeShader;
    delete _computeProgram;
}

void ComputeService::compute (Module *module, GLfloat* buffer) {

    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram ();
    clock_t start = clock ();
    Graph graph (module);
    QString newCode =  graph.glsl();
    if (newCode != QString::null && code != newCode) {
        code = newCode;
        QString c = _codeTemplate;
        c.detach();                 // deep copy, otherwise we overwrite the placeholder
        QString sourceCode = c.replace ("// inserted code //", code);
        std::cout << "Module " << module -> name().toStdString() << " : " << "\n";
        if (_computeShader) {
            _computeProgram -> removeAllShaders ();
            if (_computeShader -> compileSourceCode (sourceCode)) {
                _computeProgram -> addShader (_computeShader);
                _computeProgram -> link ();
                _computeProgram -> bind ();
                execute (module, buffer);
            } else {
                std::cout << "Compute shader would not compile\n";
            }
        }
    } else {
        std::cout << "No render code for compute shader\n";
    }
    clock_t end = clock ();
    int time = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
    std::cout << " ... finished in " << time << " milliseconds\n\n";
}

void ComputeService::execute (Module* module, GLfloat* buffer) {
    f -> glGenBuffers (1, &_heightMap);
    long bytes = 2 * _size * _size * sizeof (GLfloat);
    f -> glUseProgram (_computeProgram -> programId ());
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, _heightMap);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * _size * _size, nullptr, GL_DYNAMIC_READ);

    static GLint resolutionLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "resolution");
    f -> glUniform2i (resolutionLoc, _size * 2, _size);
    //static GLint boundsLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "bounds");
    //f -> glUniform4f (boundsLoc, bounds.west(), bounds.south(), bounds.east(), bounds.north());
    int xp = std::pow (2, resolution - 5);
    f -> glDispatchCompute (xp, xp * 2, 1);
    f -> glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 5, _heightMap);

    GLfloat *ptr = (GLfloat*) f -> glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, sizeof (GLfloat) * 2 * _size * _size, GL_MAP_READ_BIT);
    memcpy (buffer, ptr, sizeof (GLfloat) * 2 * _size * _size);
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    f -> glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, bytes, buffer);
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, 5);
    f -> glDeleteBuffers (1, &_heightMap);
}

void ComputeService::setBounds (const Bounds &bounds) {
    _bounds = bounds;
}

int ComputeService::size() {
    return _size;
}