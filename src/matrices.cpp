//
// Created by martin on 09/09/17.
//

#include <cmath>
#include <iostream>
#include "matrices.h"

using namespace matrices;




Mat4::Mat4() {
    _vectors [0] = Vec4 (1.0, 0.0, 0.0, 0.0);
    _vectors [1] = Vec4 (0.0, 1.0, 0.0, 0.0);
    _vectors [2] = Vec4 (0.0, 0.0, 1.0, 0.0);
    _vectors [3] = Vec4 (0.0, 0.0, 0.0, 1.0);
}


Mat4::Mat4 (Vec4 a, Vec4 b, Vec4 c, Vec4 d) {
    _vectors [0] = a;
    _vectors [1] = b;
    _vectors [2] = c;
    _vectors [3] = d;
}

Mat4::Mat4 (Vec4 vectors [4]) {
     for (int i = 0; i < 4; i++) {
         _vectors [i] = vectors [i];
     }
}

Mat4::~Mat4() {

}

void Mat4::setValue (int i, int j, double value) {
    _vectors[i].setValue (j, value);
}


Mat4 Mat4::operator* (Mat4 other) {
    Mat4 matrix = nullMatrix();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.setValue (i, j, row (i).dotProduct (other.row (j)));
        }
    }
    return matrix;
}

Vec4 Mat4::operator* (Vec4 v) {
    Vec4 vector = Vec4::nullVector();
    for (int i = 0; i < 4; i++) {
        vector.setValue (i, column (i).dotProduct (v));
    }
    return vector;
}

Mat4 Mat4::operator* (double k) {
    Mat4 matrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.setValue (i, j, value (i, j) * k);
        }
    }
    return matrix;
}

Mat4 Mat4::operator+ (Mat4 other) {
    Mat4 matrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.setValue (i, j, value (i, j) + other.value (i, j));
        }
    }
    return matrix;
}

bool Mat4::operator== (Mat4 other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (value (i, j) != other.value (i, j)) {
                return false;
            }
        }
    }
    return true;
}

Mat4 Mat4::transpose () {
    Mat4 matrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.setValue (i, j, value (j, i));
        }
    }
    return matrix;
}

Mat4 Mat4::nullMatrix() {
    Mat4 matrix;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.setValue (i, j, 0.0);
        }
    }
    return matrix;
}


Mat4 Mat4::translationMatrix (const double& x, const double& y, const double& z) {
    return Mat4 (
            Vec4 (1.0, 0.0, 0.0, x),
            Vec4 (0.0, 1.0, 0.0, y),
            Vec4 (0.0, 0.0, 1.0, z),
            Vec4 (0.0, 0.0, 0.0, 1.0)
    );
}

Mat4 Mat4::scalingMatrix (const double& x, const double& y, const double& z) {
    return Mat4 (
            Vec4 (x, 0.0, 0.0, 0.0),
            Vec4 (0.0, y, 0.0, 0.0),
            Vec4 (0.0, 0.0, z, 0.0),
            Vec4 (0.0, 0.0, 0.0, 1.0)
    );
}

Vec4 Mat4::column (int i) {
    return transpose()._vectors [i];
}

Vec4 Mat4::row (int i) {
    return _vectors [i];
}

Vec4& Mat4::operator[] (int i) {
    return _vectors [i];
}

double Mat4::value (int i, int j) {
    return _vectors [i].value (j);
}

void Mat4::debug() {
    for (int i = 0; i < 4; i++) {
        _vectors [i].debug();
    }
}

// Vector class


Vec4::Vec4 (double x, double y, double z, double w) {
    _values [0] = x;
    _values [1] = y;
    _values [2] = z;
    _values [3] = w;
}

Vec4::Vec4() {

}

Vec4::Vec4 (double values [4]) {
    setValues (values);
}

Vec4::~Vec4 () {

}

double Vec4::x () {
    return _values [0];
}

double Vec4::y () {
    return _values [1];
}

double Vec4::z () {
    return _values [2];
}

double Vec4::w () {
    return _values [3];
}

double Vec4::setX (double x) {
    _values [0] = x;
}

double Vec4::setY (double y) {
    _values [1] = y;
}

double Vec4::setZ (double z) {
    _values [2] = z;
}

double Vec4::setW (double w) {
    _values [3] = w;
}

void Vec4::setValues (double* values) {
    for (int i = 0; i < 4; i++) {
        _values [i] = values [i];
    }
}

double Vec4::dotProduct (Vec4 other) {
    double dp = 0;
    for (int i = 0; i < 4; i++) {
        dp += _values [i] * other._values [i];
    }
    return dp;
}

double Vec4::magnitude () {
    return (std::sqrt (x() * x() + y() * y() + z() * z() + w() * w()));
}

void Vec4::normalise() {
    double l = magnitude ();
    setX (x() / l);
    setY (y() / l);
    setZ (z() / l);
    setW (w() / l);
}

double& Vec4::operator[] (int i) {
    return _values [i];
}

Vec4 Vec4::nullVector () {
    Vec4 vector;
    for (int i = 0; i < 4; i++) {
        vector._values [i] = 0;
    }
    return vector;
}

void Vec4::setValue (int i, double value) {
    _values [i] = value;
}

double Vec4::value (int i) {
    return _values [i];
}

void Vec4::debug () {
    for (int i = 0; i < 4; i++) {
        std::cout << _values [i] << " ";
    }
    std::cout << "\n";
}

Versor Versor::fromParameters (const double& theta, const double& x, const double& y, const double& z) {
    return Versor (Vec4 (std::cos (theta / 2), std::sin (theta / 2) * x, std::sin (theta / 2) * y, std::sin (theta / 2) * z));
}

Versor::Versor (Vec4 values) : Vec4 (values) {
    normalise();
}

Versor Versor::operator* (Versor other) {
    Vec4 v;
    v.setX (other.x() * x() - other.y() * y() - other.z() * z() - other.w() * w());
    v.setY (other.x() * y() + other.y() * x() - other.w() * z() + other.w() * z());
    v.setZ (other.x() * z() + other.y() * w() + other.z() * x() - other.w() * y());
    v.setW (other.x() * w() - other.y() * z() + other.z() * y() + other.w() * x());
    return Versor (v);
}

Mat4 Versor::toRotationMatrix() {
    Vec4 row0 = Vec4 (1 - 2 * y() * y() - 2 * z() * z(), 2 * x() * y() - 2 * w() * z(), 2 * x() * z() + 2 * w() * y(), 0.0);
    Vec4 row1 = Vec4 (2 * x() * y() + 2 * w() * z(), 1 - 2 * x() * x() - 2 * z() * z(), 2 * y() * z() - 2 * w() * x(), 0.0);
    Vec4 row2 = Vec4 (2 * x() * z() - 2 * w() * y(), 2 * y() * z() + 2 * w() * x(), 1 - 2 * x() * x() - 2 * y() * y(), 0.0);
    Vec4 row3 = Vec4 (0.0, 0.0, 0.0, 1.0);
    return Mat4 (row0, row1, row2, row3);
}