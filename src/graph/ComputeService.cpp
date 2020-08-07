//
// Created by martin on 21/11/2019.
//
#include <iostream>
#include <src/CalenhadServices.h>
#include "ComputeService.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"
#include <cmath>
#include "graph.h"
#include "../messages/QNotificationHost.h"
/*

calenhad::graph::ComputeService::ComputeS ervice () {
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

ComputeService::ComputeService () :  _computeProgram (nullptr), _computeShader (nullptr), _heightMap (0) {
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
    if (!_context.create()) {
        throw std::runtime_error("context creation failed");
    }
    _surface.create();
    _context.makeCurrent( &_surface);
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

void ComputeService::compute (Module *module, CubicSphere *buffer) {
    Graph graph (module);
    QString newCode = graph.glsl();
    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram();
    clock_t start = clock ();

    // create and allocate a buffer for any input rasters
    //if (!_rasterTexture) {
    //    delete _rasterTexture;
    //}
    if (graph.rasterCount() > 0) {
        // write the data in the rasters out to the raster content buffer
        int size = CalenhadServices::gridSize();
        long bytes = 6 * size * size * sizeof (GLfloat) * graph.rasterCount();
        float* rasterBuffer = (float*) malloc (bytes);
        for (int i = 0; i < graph.rasterCount(); i++) {
            QImage* image = graph.raster (i);
            image -> scaled (size * 2, size);
            for (int x = 0; x < size * 2; x++) {
                for (int y = 0; y < size; y++) {
                    QColor c = image -> pixelColor (x, size - y);
                    double value = (c.redF() + c.greenF() + c.blueF()) / 3;
                    value = (value * 2) - 1;
                    int index = (i * size * size * 2) + (y * size * 2) + x;
                    rasterBuffer [index] = (float) value;
                }
            }
        }

        f -> glGenBuffers (1, &_rasterBuffer);
        f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _rasterBuffer);
        f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, _rasterBuffer);
        f -> glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (GLfloat) * 2 * size * size * graph.rasterCount(), rasterBuffer, GL_DYNAMIC_READ);
        free (rasterBuffer);
    }

    if (newCode != QString::null) {
    //if (_forceRender || code != newCode)) {
        _forceRender = false;
        code = newCode;
        QString ct = _codeTemplate;
        ct.detach();                 // deep copy, otherwise we overwrite the placeholder
        QString sourceCode = ct.replace("// inserted code //", code);
        std::cout << "Module " << module -> name().toStdString() << " : " << "\n";
        if (_computeShader) {
            _computeProgram -> removeAllShaders();
            if (_computeShader -> compileSourceCode(sourceCode)) {
                _computeProgram -> addShader (_computeShader);
                _computeProgram -> link();
                _computeProgram -> bind();
                execute (buffer -> data());
            } else {
                CalenhadServices::messages() -> message ("Compute shader would not compile", code);
            }
        }
    //}
    } else {
        CalenhadServices::messages() -> message ("No code for compute shader",  code);
    }

    clock_t end = clock ();
    int time = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
    std::cout << " ... finished in " << time << " milliseconds\n\n";

}

void ComputeService::execute (GLfloat* buffer) {

    f -> glGenBuffers (1, &_heightMap);
    uint size = CalenhadServices::gridSize();
    ulong bytes = 6 * size * size * sizeof (GLfloat);
    f -> glUseProgram (_computeProgram -> programId ());
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, _heightMap);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, bytes, nullptr, GL_DYNAMIC_READ);

    static GLint gridSizeLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "size");
    f -> glUniform1i (gridSizeLoc, size);

    uint xp = size / 32; // the grid size divided by the number of local invocations defined in the shader which is 32 x 32 x 1
    f -> glDispatchCompute (xp, xp, 6);
    f -> glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);

    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 5, _heightMap);

    GLfloat *ptr = (GLfloat*) f -> glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, bytes, GL_MAP_READ_BIT);
    memcpy (buffer, ptr, bytes);
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    f -> glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, bytes, buffer);
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, 5);
    f -> glDeleteBuffers (1, &_heightMap);
    f -> glDeleteBuffers (1, &_rasterBuffer);
}

