//
// Created by martin on 14/10/2019.
//

#include "CubicSphere.h"
#include <cmath>
#include <QtGui/QImage>
#include <src/CalenhadServices.h>
#include "../controls/globe/CalenhadStatistics.h"

using namespace geoutils;
using namespace calenhad::grid;
using namespace calenhad::mapping;
using namespace calenhad::controls::globe;

CubicSphere::CubicSphere (const int& depth) : _renderTime (0.0), _grid (nullptr), _computeTime (0.0), _depth (depth) {
    _size = std::pow (2, depth);
    initialise();
}

// copy constructor
CubicSphere::CubicSphere (CubicSphere* other) : _renderTime (0.0), _grid (nullptr), _computeTime (0.0), _depth (other -> _depth) {
    _size = other->size();
    _grid = (float*) malloc (6 * _size * _size * sizeof (float));
    for (int i = 0; i < count (); i++) {
        _grid[i] = other -> _grid[i];
    }
}


void CubicSphere::copy (CubicSphere* other) {
    if (_size == other -> size()) {
    //    for (int i = 0; i < count (); i++) {
    //        _grid [i] = other -> _grid [i];
    //    }

        memcpy (_grid, other -> _grid, count() * sizeof (GLfloat));
    }
}


void CubicSphere::makeTile (const int& x, const int& y, CubicSphere* source) {
    int xOffset = y * _size;
    int stride = source -> size();
    int yOffset = x * stride * _size;
    for (int i = 0; i < 6; i++) {
        int faceOffset = i * stride * stride;
        for (int j = 0; j < _size; j++) {
            for (int k = 0; k < _size; k++) {
                int iy = xOffset + j * stride;
                int ix = yOffset + k;
                float value = source -> data() [faceOffset + ix + iy];
                _grid [i * _size * _size + k * _size + j] = value;
                //std::cout << x << " " << y << ":  " << i << ", " << j << ", " << k << ": " << sourceIndex << " = " << value << "\n";
            }
        }
    }
}

void CubicSphere::initialise() {
    _grid = (float*) malloc (6 * _size * _size * sizeof (float));
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < _size; j++) {
            for (int k = 0; k < _size; k++) {
                _grid [(i * _size * _size) + j * _size + k] = 0.0f;
            }
        }
    }
}

CubicSphere::~CubicSphere() {
    if (_grid) {
        free (_grid);
        _grid = nullptr;
    }
}

long CubicSphere::count() const {
    return 6 * _size * _size;
}

void CubicSphere::toGeolocation (const CubeCoordinates& fuv, Geolocation& g) const {
    Cartesian c;
    toCartesian (fuv, c);
    g = geoutils::Geoutils::toGeolocation(c);
}

void CubicSphere::toCartesian (const CubeCoordinates& fuv, Cartesian& xyz) const {
    double u = ((float) fuv.u / (float) _size) * 2.0 - 1.0;
    double v = ((float) fuv.v / (float) _size) * 2.0 - 1.0;
    double x = 0.0, y = 0.0, z = 0.0;
    if (fuv.face == FACE_FRONT)  { y =  1.0; x = v; z = u; }
    if (fuv.face == FACE_BACK)  { y = -1.0; x = v; z = u; }
    if (fuv.face == FACE_EAST)   { x =  1.0; z = v; y = u; }
    if (fuv.face == FACE_WEST)   { x = -1.0; z = v; y = u; }
    if (fuv.face == FACE_NORTH)  { z =  1.0; x = v; y = u; }
    if (fuv.face == FACE_SOUTH)   { z = -1.0; x = v; y = u; }
    xyz.x = x * sqrt (1.0f - y * y * 0.5f - z * z * 0.5f + y * y * z * z / 3.0f);
    xyz.y = y * sqrt (1.0f - z * z * 0.5f - x * x * 0.5f + z * z * x * x / 3.0f);
    xyz.z = z * sqrt (1.0f - x * x * 0.5f - y * y * 0.5f + x * x * y * y / 3.0f);
}


void CubicSphere::toCubeCoordinates (CubeCoordinates& fuv, const Cartesian& xyz) const {
    // Axes are in a different order from the GLSL so we swizzle them
    Cartesian position = Cartesian (xyz.x, xyz.y, xyz.z);

    double x = position.x, y = position.y, z = position.z;
    double fx = std::abs (x), fy = std::abs (y), fz = std::abs (z);

        if (fy >= fx && fy >= fz) {
            double a2 = x * x * 2.0;
            double b2 = z * z * 2.0;
            double inner = -a2 + b2 - 3;
            double innersqrt = -sqrt ((inner * inner) - 12.0 * a2);
            position.x = (x == 0.0 || x == -0.0) ? 0.0 : (sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2);
            position.z = (z == 0.0 || z == -0.0) ? 0.0 : (sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2);
            if (position.x > 1.0) { position.x = 1.0; }
            if (position.z > 1.0) { position.z = 1.0; }
            if (x < 0) { position.x = -position.x; }
            if (z < 0) { position.z = -position.z; }
            fuv.face = (y > 0) ? FACE_FRONT : FACE_BACK;;
            fuv.u = int ((position.x * 0.5 + 0.5) * _size);
            fuv.v = int ((position.z * 0.5 + 0.5) * _size);
        } else if (fx >= fy && fx >= fz) {
            double a2 = y * y * 2.0;
            double b2 = z * z * 2.0;
            double inner = -a2 + b2 - 3;
            double innersqrt = -sqrt ((inner * inner) - 12.0 * a2);
            position.y = (y == 0.0 || y == -0.0) ? 0.0 : (sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2);
            position.z = (z == 0.0 || z == -0.0) ? 0.0 : (sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2);
            if (position.y > 1.0) { position.y = 1.0; }
            if (position.z > 1.0) { position.z = 1.0; }
            if (y < 0) { position.y = -position.y; }
            if (z < 0) { position.z = -position.z; }
            fuv.face = (x > 0) ? FACE_EAST : FACE_WEST;
            fuv.u = int ((position.y * 0.5 + 0.5) * _size);
            fuv.v = int ((position.z * 0.5 + 0.5) * _size);
        } else {
            double a2 = x * x * 2.0;
            double b2 = y * y * 2.0;
            double inner = -a2 + b2 - 3;
            double innersqrt = -sqrt ((inner * inner) - 12.0 * a2);
            position.x = (x == 0.0 || x == -0.0) ? 0.0 : (sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2);
            position.y = (y == 0.0 || y == -0.0) ? 0.0 : (sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2);
            if (position.x > 1.0) { position.x = 1.0; }
            if (position.y > 1.0) { position.y = 1.0; }
            if (x < 0) { position.x = -position.x; }
            if (y < 0) { position.y = -position.y; }
            fuv.face = (z > 0) ? FACE_NORTH :  FACE_SOUTH;
            fuv.u = int ((position.y * 0.5 + 0.5) * _size);
            fuv.v = int ((position.x * 0.5 + 0.5) * _size);
        }
}

float* CubicSphere::grid () {
    return _grid;
}

int CubicSphere::size() const {
    return _size;
}

float CubicSphere::valueAt (const Geolocation& g) {
    Cartesian c (geoutils::Geoutils::toCartesian (g));
    CubeCoordinates fuv;
    toCubeCoordinates (fuv, c);
    return valueAt (fuv);
}

float CubicSphere::valueAt (const CubeCoordinates& fuv) {
    if (fuv.face == 0) { return _grid [fuv.face * _size * _size + fuv.v * _size + fuv.u]; }
    if (fuv.face == 1) { return _grid [fuv.face * _size * _size + fuv.v * _size + fuv.u]; }
    if (fuv.face == 2) { return _grid [fuv.face * _size * _size + fuv.u * _size + fuv.v]; }
    if (fuv.face == 3) { return _grid [fuv.face * _size * _size + fuv.u * _size + fuv.v]; }
    if (fuv.face == 4) { return _grid [fuv.face * _size * _size + fuv.u * _size + fuv.v]; }
    if (fuv.face == 5) { return _grid [fuv.face * _size * _size + fuv.u * _size + fuv.v]; }
    return 0.0f;
}

CalenhadStatistics CubicSphere::statistics (CalenhadStatistics& statistics) const {
    return CalenhadServices::compute() -> statistics();
}

GLfloat *CubicSphere::data() {
    return _grid;
}


void CubicSphere::fromRaster (QImage* image) {

    Geolocation g;
    int units = _size * 6;
    int progress = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < _size; j++) {
            for (int k = 0; k < _size; k++) {
                CubeCoordinates fuv (i, j, k);
                toGeolocation (fuv, g);
                double dx = g.longitude() / (M_PI * 2) + 0.5;
                double dy = g.latitude() / M_PI + 0.5;
                int ix = (int) (dx * (image -> width() - 1));
                int iy = (int) ((1 - dy) * (image -> height() - 1));

                QColor c = image -> pixelColor (ix, iy);
                double value = c.valueF() * 2.0 - 1.0;
                _grid [i * _size * _size + j * _size + k] = (float) value;
            }
            progress++;

        }
    }
}

void CubicSphere::heightmap (const int& face, QImage* image) {

    for (int i = 0; i < image -> height(); i++) {
        for (int j = 0; j < image -> width(); j++) {
            if (i < _size && j < _size) {
                CubeCoordinates fuv;
                fuv.face = face;
                fuv.u = i;
                fuv.v = j;
                double v = (valueAt (fuv) + 1.0) / 2.0;
                //int index = face * _size * _size + i * _size + j;
                //float v = (_grid [index] + 1) / 2;
                v =  std::max (0.0, (double) std::min ((double) v, 1.0));
                QColor c;
                c.setBlueF (v);
                c.setRedF (v);
                c.setGreenF (v);
                c.setAlphaF (1.0);
                image -> setPixelColor (i, j, c);
            }
        }
    }
}

void CubicSphere::exportHeightmaps (const QString &filename) {
    for (int f = 0; f < 6; f++) {
        QImage* image = new QImage (_size, _size, QImage::Format_RGB32);
        heightmap (f, image);
        image -> save (filename  + QString::number (f) + ".png");
        delete image;
    }
}


void CubicSphere::adjacent (const struct CubeCoordinates& fuv, struct CubeCoordinates* m) {

    m [0] = CubeCoordinates (fuv.face, fuv.u + 1, fuv.v);
    m [1] = CubeCoordinates (fuv.face, fuv.u - 1, fuv.v);
    m [2] = CubeCoordinates (fuv.face, fuv.u, fuv.v + 1);
    m [3] = CubeCoordinates (fuv.face, fuv.u, fuv.v - 1);

    if (m [0].u < 0) {
        m [0].u = 0;
        m [0].face = ADJACENT [fuv.face] [0];
    }
    if (m [1].u > _size - 1) {
        m [1].u = _size - 1;
        m [0].face = ADJACENT [fuv.face] [1];
    }
    if (m [2].v < 0) {
        m [2].v = 0;
        m [2].face = ADJACENT [fuv.face] [2];
    }
    if (m [3].v > _size - 1) {
        m [3].v = _size - 1;
        m [3].face = ADJACENT [fuv.face] [3];
    }
}

void CubicSphere::surrounding (const CubeCoordinates& fuv, CubeCoordinates* k) {
    CubeCoordinates adj [4];
    adjacent (fuv, adj);

    k[0] = CubeCoordinates (adj [1].face == fuv.face ? (adj [3].face == fuv.face ? fuv.face : adj [3].face) : adj [1].face, adj [1].u, adj [3].v);
    k[1] = CubeCoordinates (adj [1].face, adj [1].u, fuv.v);
    k[2] = CubeCoordinates (adj [1].face == fuv.face ? (adj [2].face == fuv.face ? fuv.face : adj [2].face) : adj [1].face, adj [1].u, adj [2].v);
    k[3] = CubeCoordinates (adj [3].face, fuv.u, adj [3].v);
    k[4] = fuv;
    k[5] = CubeCoordinates (adj [2].face, fuv.u, adj [2].v);
    k[6] = CubeCoordinates (adj [0].face == fuv.face ? (adj [3].face == fuv.face ? fuv.face : adj [3].face) : adj [0].face, adj [0].u, adj [3].v);
    k[7] = CubeCoordinates (adj[0].face, adj[0].u, fuv.v);
    k[8] = CubeCoordinates (adj [0].face == fuv.face ? (adj [2].face == fuv.face ? fuv.face : adj [2].face) : adj [0].face, adj [0].u, adj [2].v);

}

double CubicSphere::computeTime() const {
    return _computeTime;
}

void CubicSphere::setComputeTime (double time) {
    _computeTime = time;
}