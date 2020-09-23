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
#include <QImage>
#include <src/module/Procedure.h>
#include <thread>
#include <future>
#include <QtWidgets/QProgressDialog>
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
using namespace calenhad::controls::globe;


ComputeService::ComputeService () :
    _computeProgram (nullptr),
    _computeShader (nullptr),
    _rasterBuffer (0),
    _heightMap (0),
    _tileSize (std::pow (2, CalenhadServices::preferences() -> calenhad_compute_tilesize)),
    _tile (new CubicSphere (CalenhadServices::preferences() -> calenhad_compute_tilesize)),
    _tiles (std::pow (2, CalenhadServices::preferences() -> calenhad_compute_gridsize - CalenhadServices::preferences() -> calenhad_compute_tilesize)) {
    // read cuda code from files into memory for use at compute time
    QFile csFile (":/shaders/compute.glsl");
    csFile.open (QIODevice::ReadOnly);
    QTextStream csTextStream (&csFile);
    _computeTemplate = csTextStream.readAll();
    csFile.close();
/*
    QFile psFile (":/shaders/process.glsl");
    psFile.open (QIODevice::ReadOnly);
    QTextStream psTextStream (&psFile);
    _processTemplate = csTextStream.readAll();
    psFile.close();

    QFile hsFile (":/shaders/histogram.glsl");
    hsFile.open (QIODevice::ReadOnly);
    QTextStream hsTextStream (&hsFile);
    _histogramCode = hsTextStream.readAll();
    hsFile.close();
*/
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    _context.setFormat(format);
    if (!_context.create()) {
        throw std::runtime_error ("context creation failed");
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
    delete _tile;
}

void ComputeService::compute (Module *module, CubicSphere *buffer) {
    std::cout << "ComputeService::compute - Module " << module -> name().toStdString() << " - buffer " << buffer << "\n";

    clock_t start = clock ();
    Graph graph (module);
    QString newCode = graph.glsl();
    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram();


    // create and allocate a buffer for any input rasters

    if (newCode != QString::null) {
    //if (_forceRender || code != newCode)) {

        _forceRender = false;
        code = newCode;
        QString ct = _computeTemplate;
        ct.detach();                 // deep copy, otherwise we overwrite the placeholder
        QString sourceCode = ct.replace("// inserted code //", code);
        if (_computeShader) {
            _computeProgram -> removeAllShaders();
            if (_computeShader -> compileSourceCode (sourceCode)) {
                _computeProgram -> addShader (_computeShader);
                _computeProgram -> link();
                _computeProgram -> bind();
                execute (buffer -> data(), graph);
            } else {
                CalenhadServices::messages() -> message ("Compute shader would not compile", code);
            }
        }
        computeStatistics (buffer);
        clock_t end = clock ();
        int time = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
        std::cout << " ... finished in " << time << " milliseconds\n\n";
        buffer -> setComputeTime (time);
        _statistics._computeTime = (int) time;

    } else {
        CalenhadServices::messages() -> message ("No code for compute shader",  code);
    }
}


void ComputeService::process (Procedure* module, CubicSphere *buffer) {
    QString newCode = module -> glsl();
    _context.makeCurrent( &_surface);
    delete _computeShader;
    delete _computeProgram;
    _computeShader = new QOpenGLShader (QOpenGLShader::Compute);
    _computeProgram = new QOpenGLShaderProgram();
    clock_t start = clock ();

    // create and allocate a buffer for any input rasters

    if (newCode != QString::null) {
        //if (_forceRender || code != newCode)) {
        _forceRender = false;
        code = newCode;
        QString ct = _processTemplate;
        ct.detach();                 // deep copy, otherwise we overwrite the placeholder
        QString sourceCode = ct.replace("// inserted code //", code);
        std::cout << "Module " << module -> name().toStdString() << " : " << "\n";
        if (_computeShader) {
            _computeProgram -> removeAllShaders();
            if (_computeShader -> compileSourceCode (sourceCode)) {
                _computeProgram -> addShader (_computeShader);
                _computeProgram -> link();
                _computeProgram -> bind();
                process (buffer -> data(), module);
            } else {
                CalenhadServices::messages() -> message ("compute(): Compute shader would not compile", code);
            }
        }
        clock_t end = clock ();
        double time = (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
        buffer -> setComputeTime (time);
        _statistics._computeTime = (int) time;
    } else {
        CalenhadServices::messages() -> message ("compute(): No code for compute shader",  code);
    }
}

uint ComputeService::setupGrid() {
    f -> glGenBuffers (1, &_heightMap);
    uint size = std::pow (2, CalenhadServices::preferences() -> calenhad_compute_gridsize);
    ulong bytes = 6 * size * size * sizeof (GLfloat);
    f -> glUseProgram (_computeProgram -> programId ());
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, _heightMap);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, bytes, nullptr, GL_DYNAMIC_READ);

    static GLint gridSizeLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "size");
    f -> glUniform1i (gridSizeLoc, size);
    return size;
}

void ComputeService::process (GLfloat* buffer,  Procedure* module) {
    uint size = setupGrid();
    extractRasters (module);
    f -> glDispatchCompute (module -> iterations() / 1024, 1, 1);
    f -> glDeleteBuffers (1, &_rasterBuffer);
    downloadBuffer (buffer, size);

}

void ComputeService::execute (GLfloat* buffer, const Graph& graph) {
    uint size = setupGrid();
    _tileSize = size / _tiles;
    QProgressDialog progress ("Computing module " + graph.module() -> name(), "Abandon", 0, _tiles * _tiles, nullptr);
    progress.setWindowModality(Qt::WindowModal);

    for (int i = 0; i < graph.rasterCount(); i++) {
        CubicSphere* cs = new CubicSphere (CalenhadServices::preferences()->calenhad_compute_gridsize);
        graph.cube (i, cs);
        _cubes.push_back (cs);
    }


    for (int i = 0; i < _tiles; i++) {
        for (int j = 0; j < _tiles; j++) {

            // upload raster tiles for this tile
            extractRasters (graph, i, j);

            // set the tile geometry on the shader
            static GLint tileIndexLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "tileIndex");
            f -> glUniform2i (tileIndexLoc, i, j);
            static GLint tileSizeLoc = f -> glGetUniformLocation (_computeProgram -> programId(), "tileSize");
            f -> glUniform1i (tileSizeLoc, _tileSize);

            // invoke the shader
            uint xp = _tileSize / 32; // the grid size divided by the number of local invocations defined in the shader which is 32 x 32 x 1
            f -> glDispatchCompute (xp, xp, 6);
            f -> glDeleteBuffers (1, &_rasterBuffer);
            progress.setValue (i * _tiles + j);
            if (progress.wasCanceled()) { break; }
        }
        if (progress.wasCanceled()) { break; }
    }

    downloadBuffer (buffer, size);
    for (CubicSphere* cs : _cubes) { delete cs; }
    _cubes.clear();
}


void ComputeService::downloadBuffer (GLfloat* buffer, const int& size) {
    f -> glMemoryBarrier (GL_SHADER_STORAGE_BARRIER_BIT);
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _heightMap);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 5, _heightMap);
    ulong bytes = 6 * size * size * sizeof (GLfloat);
    GLfloat *ptr = (GLfloat*) f -> glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, bytes, GL_MAP_READ_BIT);
    //memcpy (buffer, ptr, bytes);
    f -> glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
    f -> glGetBufferSubData (GL_SHADER_STORAGE_BUFFER, 0, bytes, buffer);
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, 5);
    f -> glDeleteBuffers (1, &_heightMap);
}

void ComputeService::extractRasters (Procedure* module) {
    int rasterCount = module -> inputs().count();
        // upload the raster data to the GPU
    // work out the size of the buffer we need for all rasters and convolutions
    ulong bytes = 0;
    for (int i = 0; i < rasterCount; i++) {
        CubicSphere* cube = module -> inputBuffer(i);
        if (cube) {
            int r = _tile -> size();
            bytes += r * r * 6 * sizeof (GLfloat);
        }
    }

    float* rasterBuffer = (float*) malloc (bytes);
    for (int i = 0; i < rasterCount; i++) {
        // unpack the raster data from the input modules
        ulong bufferIndex = 0;

        CubicSphere* cube = module->inputBuffer (i);
        int s = cube->size ();
        for (int face = 0; face < 6; face++) {
            for (int x = 0; x < cube -> size(); x++) {
                for (int y = 0; y < cube -> size(); y++) {
                    ulong index = bufferIndex + face * s * s + x * s + y;
                    float value = cube->data() [(face * s * s) + x * s + y];
                    rasterBuffer[index] = (GLfloat) value;
                }
            }
        }
        bufferIndex += s;
    }
    f -> glGenBuffers (1, &_rasterBuffer);
    f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _rasterBuffer);
    f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, _rasterBuffer);
    f -> glBufferData (GL_SHADER_STORAGE_BUFFER, bytes, rasterBuffer, GL_DYNAMIC_READ);
    free (rasterBuffer);

}

void ComputeService::extractRasters (const Graph& graph, const int& xIndex, const int& yIndex) {

    if (graph.rasterCount() > 0) {

        // work out the size of the buffer we need for all rasters and convolutions
        long r = std::pow (2, CalenhadServices::preferences() -> calenhad_compute_gridsize);
        ulong bytes = r * r * 6 * sizeof (GLfloat) * graph.rasterCount();


        // unpack the raster data from the raster / convolution modules
        ulong bufferIndex = 0;
        float* rasterBuffer = (float*) malloc (bytes);

        for (int i = 0; i < graph.rasterCount(); i++) {
            _tile -> makeTile (xIndex, yIndex, _cubes.at (i) );
            if (_tile) {
                int s = _tile -> size();
                for (int face = 0; face < 6; face++) {
                    for (int x = 0; x < _tile -> size(); x++) {
                        for (int y = 0; y < _tile -> size(); y++) {
                            ulong index = bufferIndex + face * s * s + x * s + y;
                            float value = _tile -> data() [(face * s * s) + x * s + y];
                            rasterBuffer [index] = (GLfloat) value;
                        }
                    }
                }
                bufferIndex += _tile -> size();
            }
        }

        // upload the raster data to the GPU
        f -> glGenBuffers (1, &_rasterBuffer);
        f -> glBindBuffer (GL_SHADER_STORAGE_BUFFER, _rasterBuffer);
        f -> glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, _rasterBuffer);
        f -> glBufferData (GL_SHADER_STORAGE_BUFFER, bytes, rasterBuffer, GL_DYNAMIC_READ);
        free (rasterBuffer);
    }

}


void ComputeService::computeStatistics (grid::CubicSphere* buffer) {
    clock_t start = clock ();
    _tileSize = buffer->size () / _tiles;
    _statistics._maxValue = 0.0, _statistics._maxValue = 0.0;
    _statistics._valueCount = 0;
    float sum = 0.0;
    int sampleEvery = 8;
    for (int x = 0; x < buffer->size (); x += sampleEvery) {
        for (int y = 0; y < buffer->size (); y += sampleEvery) {
            for (int z = 0; z < 6; z++) {
                int i = z * buffer -> size() * buffer -> size() + y * buffer -> size() + x;
                float v = (float) buffer->data () [i];
                if (v < _statistics._minValue || x == 0) { _statistics._minValue = v; }
                if (v > _statistics._maxValue || x == 0) { _statistics._maxValue = v; }
                sum += v;
                _statistics._valueCount++;
            }
        }
    }

    _statistics._meanValue = sum / (float) _statistics._valueCount;
    _statistics._range = _statistics._maxValue - _statistics._minValue;
    for (int i = 0; i < 1000; i++) { _statistics._buckets [i] = 0; }
    for (int x = 0; x < buffer->size (); x += sampleEvery) {
        for (int y = 0; y < buffer->size (); y += sampleEvery) {
            for (int z = 0; z < 6; z++) {
                int i = z * buffer -> size() * buffer -> size() + y * buffer -> size() + x;
                float v = (float) buffer->data () [i];
                float normalised = (v - _statistics._minValue) / (_statistics._range);
                int bucket = int (normalised * 999);
                _statistics._buckets [bucket]++;
            }
        }
    }


    clock_t end = clock ();
    int time = (int) (((double) end - (double) start) / CLOCKS_PER_SEC * 1000.0);
    std::cout << " statistics fetched in " << time << " milliseconds\n\n";
}

CalenhadStatistics ComputeService::statistics () {
    return _statistics;
}


