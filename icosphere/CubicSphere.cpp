//
// Created by martin on 14/10/2019.
//

#include "CubicSphere.h"
#include <cmath>


using namespace calenhad::icosphere;
using namespace calenhad::icosphere::geometry;

CubicSphere::CubicSphere (const int& depth) {
    _size = 16384;
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
    delete _grid;
}

long CubicSphere::count() {
    return 6 * _size * _size;
}

void CubicSphere::toCartesian (const geometry::CubeCoordinates& fuv, geometry::Cartesian& xyz) {

    double x, y, z;
    if (fuv.face == FACE_NORTH)  { y =  1.0; x = fuv.u; y = fuv.v; }
    if (fuv.face == FACE_SOUTH)  { y = -1.0; x = fuv.u; y = fuv.v; }
    if (fuv.face == FACE_EAST)   { x =  1.0; z = fuv.u; y = fuv.v; }
    if (fuv.face == FACE_WEST)   { x = -1.0; z = fuv.u; y = fuv.v; }
    if (fuv.face == FACE_FRONT)  { z =  1.0; x = fuv.u; z = fuv.v; }
    if (fuv.face == FACE_BACK)   { z = -1.0; x = fuv.u; z = fuv.v; }
    xyz.x = x * sqrt (1.0f - y * y * 0.5f - z * z * 0.5f + y * y * z * z / 3.0f);
    xyz.y = y * sqrt (1.0f - z * z * 0.5f - x * x * 0.5f + z * z * x * x / 3.0f);
    xyz.z = z * sqrt (1.0f - x * x * 0.5f - y * y * 0.5f + x * x * y * y / 3.0f);
}


void CubicSphere::toCubeCoordinates (CubeCoordinates& fuv, const Cartesian& xyz) {
    Cartesian position = xyz;

    double x = position.x, y = position.y, z = position.z;
    double fx = std::abs (x), fy = std::abs (y), fz = std::abs (z);

    if (fy >= fx && fy >= fz) {
        double a2 = x * x * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 - 3;
        double innersqrt = -sqrt ((inner * inner) - 12.0 * a2);

        position.x = (x == 0.0 || x == -0.0) ? 0.0 : sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2;
        position.z = (z == 0.0 || z == -0.0) ? 0.0 : sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2;
        if (position.x > 1.0) position.x = 1.0;
        if (position.z > 1.0) position.z = 1.0;

        if (x < 0) position.x = -position.x;
        if (z < 0) position.z = -position.z;

        // top face : bottom face
        position.y = (y > 0) ? 1.0 : -1.0;;
        fuv.face = (y > 0) ? FACE_NORTH : FACE_SOUTH;;
        fuv.u = floor (0.5 + (position.x * _size));
        fuv.v = floor (0.5 + (position.y * _size));

    } else if (fx >= fy && fx >= fz) {
        double a2 = y * y * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 - 3;
        double innersqrt = -sqrt ((inner * inner) - 12.0 * a2);

        if (y == 0.0 || y == -0.0) {
            position.y = 0.0;
        } else {
            position.y = sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2;
        }

        if (z == 0.0 || z == -0.0) {
            position.z = 0.0;
        } else {
            position.z = sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2;
        }

        if (position.y > 1.0) position.y = 1.0;
        if (position.z > 1.0) position.z = 1.0;

        if (y < 0) position.y = -position.y;
        if (z < 0) position.z = -position.z;

        if (x > 0) {
            // right face
            position.x = 1.0;
            fuv.face = FACE_EAST;
        } else {
            // left face
            position.x = -1.0;
            fuv.face = FACE_WEST;
        }
        fuv.u = floor (0.5 + (position.z * _size));
        fuv.v = floor (0.5 + (position.y * _size));
    } else {
        double a2 = x * x * 2.0;
        double b2 = y * y * 2.0;
        double inner = -a2 + b2 - 3;
        double innersqrt = -sqrt ((inner * inner) - 12.0 * a2);

        if (x == 0.0 || x == -0.0) {
            position.x = 0.0;
        } else {
            position.x = sqrt (innersqrt + a2 - b2 + 3.0) * HALF_ROOT_2;
        }

        if (y == 0.0 || y == -0.0) {
            position.y = 0.0;
        } else {
            position.y = sqrt (innersqrt - a2 + b2 + 3.0) * HALF_ROOT_2;
        }

        if (position.x > 1.0) position.x = 1.0;
        if (position.y > 1.0) position.y = 1.0;

        if (x < 0) position.x = -position.x;
        if (y < 0) position.y = -position.y;

        if (z > 0) {
            // front face
            position.z = 1.0;
            fuv.face = FACE_FRONT;
        } else {
            // back face
            position.z = -1.0;
            fuv.face = FACE_BACK;
        }
        fuv.u = floor (0.5 + (position.x * _size));
        fuv.v = floor (0.5 + (position.z * _size));
    }
}
