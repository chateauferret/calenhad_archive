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
#include "../grid/CubicSphere.h"
#include <QImage>
#include "../module/StructuredGrid.h"
#include "../module/RasterModule.h"
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

        // work out the size of the buffer we need for all rasters and convolutions
        ulong bytes = 0;
        for (int i = 0; i < graph.rasterCount(); i++) {
            QImage* image = dynamic_cast<QImage*> (graph.raster (i));
            if (image) {
                bytes += image -> height() * image -> width() * sizeof (GLfloat);
            }
            CubicSphere* cube = dynamic_cast<CubicSphere*> (graph.cube (i));
            if (cube) {
                int r = cube -> size();
                bytes += r * r * 6 * sizeof (GLfloat);
            }
        }
        // unpack the raster data from the raster / convolution modules
        ulong bufferIndex = 0;
        float* rasterBuffer = (float*) malloc (bytes);
        for (int i = 0; i < graph.rasterCount(); i++) {
            QImage* image = graph.raster (i);
            if (image) {
                for (int x = 0; x < image -> width(); x++) {
                    for (int y = 0; y < image -> height(); y++) {
                        QColor c = image -> pixelColor (x, image -> height() - y - 1);
                        double value = (c.redF() + c.greenF() + c.blueF()) / 3;
                        value = (value * 2) - 1;
                        ulong index = bufferIndex + (y * image -> width()) + x;
                        rasterBuffer [index] = (GLfloat) value;
                    }
                }
            }

            CubicSphere* cm = graph.cube (i);
            if (cm) {
                for (int face = 0; face < 6; face++) {
                    for (int x = 0; x < cm -> size(); x++) {
                        for (int y = 0; y < cm -> size(); y++) {
                            int s = cm -> size();
                            ulong index = bufferIndex + (face * s * s + y * s + x);
                            double value = cm -> data() [index];
                            rasterBuffer [index] = (GLfloat) value;
                        }
                    }
                }
            }
        }

        // upload the raster data to the GPU
        f -> glGenBuffers (1, &_rasterBuffer);
        std::cout << "Generate buffer " << _rasterBuffer << " of " << bytes << " bytes\n";
        f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _rasterBuffer);
        f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, _rasterBuffer);
        f -> glBufferData (GL_SHADER_STORAGE_BUFFER, bytes, rasterBuffer, GL_DYNAMIC_READ);
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

/*
void ComputeService::computeDetail (Module *module, CubicSphere *buffer) {
    if (! module -> )

    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram();
    clock_t start = clock ();
*/

    // create and allocate a buffer for any input rasters
    //if (!_rasterTexture) {
    //    delete _rasterTexture;
    //}
    /*if (graph.rasterCount() > 0) {

        // work out the size of the buffer we need for all rasters and convolutions
        ulong bytes = 0;
        for (int i = 0; i < graph.rasterCount(); i++) {
            QImage* image = dynamic_cast<QImage*> (graph.raster (i));
            if (image) {
                bytes += image -> height() * image -> width() * sizeof (GLfloat);
            }
            CubicSphere* cube = dynamic_cast<CubicSphere*> (graph.cube (i));
            if (cube) {
                int r = cube -> size();
                bytes += r * r * 6 * sizeof (GLfloat);
            }
        }
        // unpack the raster data from the raster / convolution modules
        ulong bufferIndex = 0;
        float* rasterBuffer = (float*) malloc (bytes);
        for (int i = 0; i < graph.rasterCount(); i++) {
            QImage* image = graph.raster (i);
            if (image) {
                for (int x = 0; x < image -> width(); x++) {
                    for (int y = 0; y < image -> height(); y++) {
                        QColor c = image -> pixelColor (x, image -> height() - y - 1);
                        double value = (c.redF() + c.greenF() + c.blueF()) / 3;
                        value = (value * 2) - 1;
                        ulong index = bufferIndex + (y * image -> width()) + x;
                        rasterBuffer [index] = (GLfloat) value;
                    }
                }
            }

            CubicSphere* cm = graph.cube (i);
            if (cm) {
                for (int face = 0; face < 6; face++) {
                    for (int x = 0; x < cm -> size(); x++) {
                        for (int y = 0; y < cm -> size(); y++) {
                            int s = cm -> size();
                            ulong index = bufferIndex + (face * s * s + y * s + x);
                            double value = cm -> data() [index];
                            rasterBuffer [index] = (GLfloat) value;
                        }
                    }
                }
            }
        }

        // upload the raster data to the GPU
        f -> glGenBuffers (1, &_rasterBuffer);
        std::cout << "Generate buffer " << _rasterBuffer << " of " << bytes << " bytes\n";
        f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _rasterBuffer);
        f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, _rasterBuffer);
        f -> glBufferData (GL_SHADER_STORAGE_BUFFER, bytes, rasterBuffer, GL_DYNAMIC_READ);
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

}*/
