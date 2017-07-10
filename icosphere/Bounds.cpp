//
// Created by martin on 09/03/17.
//

#include "Bounds.h"
#include <QDebug>

using namespace icosphere;

int Bounds::estimateVertexCount (const int& level)const {
    int n = 10 * std::pow (4, level - 1) + 2;
    double latPortion = std::abs (std::sin (north())) - std::sin (south());
    double portion = latPortion * width() / (4 * M_PI);
    qDebug() << "Portion: " << portion;
    return (int) (n * portion);
}

Bounds::Bounds (double n, double s, double e, double w) : GeoDataLatLonBox (n, s, e, w){

}
