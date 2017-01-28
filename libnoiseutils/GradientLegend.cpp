
// Created by martin on 27/01/17.
//

#include "GradientLegend.h"
#include "Interpolation.h"

using namespace noise::utils;

QColor GradientLegend::lookup (const int& value) {
    std::map<int, QColor>::iterator i = std::find_if_not (intervals.begin(), intervals.end(), [&value] (std::pair<int, QColor> entry) -> bool { return entry.first <= value; } );
    QColor c2 = i -> second;
    double p2 = i -> first;
    QColor c1 = (--i) -> second;
    double p1 = i -> first;
    QColor color = interpolate (c1, c2, p1 + ((value - p1) / (p2 - p1)));
    return color;
}

QColor GradientLegend::interpolate (const QColor& c1, const QColor& c2, const double& mix) {
    QColor color;
    color.setRedF (c1.redF() * mix + c2.redF() * (1 - mix));
}