//
// Created by martin on 09/03/17.
//

#include "Bounds.h"
#include <QDebug>


using namespace icosphere;
using namespace geoutils;

int Bounds::estimateVertexCount (const int& level)const {
    int n = 10 * std::pow (4, level - 1) + 2;
    double latPortion = std::abs (std::sin (_north)) - std::sin (_south);
    double portion = latPortion * width() / (4 * M_PI);
    qDebug() << "Portion: " << portion;
    return (int) (n * portion);
}

Bounds::Bounds (double n, double s, double e, double w) : _north (std::max (n, s)), _south (std::min (n, s)), _east (e), _west (w) {

}

bool Bounds::operator== (const Bounds& other) const {
    return _north == other._north && _south == other._south && _east == other._east && _west == other._west;
}


bool Bounds::operator!= (const Bounds& other) const {
    return ! (operator== (other));
}

double Bounds::width () const {
    return _east > _west ? _east - _west : M_2_PI - (_west - _east);
}

double Bounds::height() const {
    return _north - _south;
}

double Bounds::north () const {
    return _north;
}

double Bounds::south () const {
    return _south;
}

double Bounds::east () const {
    return _east;
}

double Bounds::west () const {
    return _west;
}

Geolocation Bounds::center() const {
    double lon = _east > _west ? _east + _west / 2 : M_2_PI - (_east + _west / 2);
    double lat = _north + _south / 2;
}

bool Bounds::crossesDateline () {
    return _west > _east;
}
