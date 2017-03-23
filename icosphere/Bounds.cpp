//
// Created by martin on 09/03/17.
//

#include "Bounds.h"

using namespace icosphere;

Bounds::Bounds  (double n, double s, double w, double e, bool crossesDateline) : lat1 (n), lat2 (s), lon1 (w), lon2 (e), _crossesDateline (crossesDateline) { }

bool Bounds::operator== (const Bounds& other) {
    return lat1 == other.lat1 && lat2 == other.lat2 && lon2 == other.lon2 && lon1 == other.lon1;
}

bool Bounds::operator!= (const Bounds& other) {
    return ! (operator== (other));
}

int Bounds::estimateVertexCount (const int& level) {
    int n = 10 * std::pow (4, level - 1) + 2;
    double latPortion = (lat1 > 0 == lat2 > 0 ? std::abs (std::sin (lat1)) - std::abs (std::sin (lat2)) : std::abs (std::sin (lat1)) + std::abs (std::sin (lat2))) / 2;
    double lonPortion = std::abs (lon2 - lon1);
    if (_crossesDateline) {
        lonPortion = 2 * M_PI - lonPortion;
    }
    double portion = latPortion * lonPortion;
    return (int) (n * portion);
}