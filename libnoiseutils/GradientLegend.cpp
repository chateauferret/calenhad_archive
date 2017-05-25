
// Created by martin on 27/01/17.
//

#include "GradientLegend.h"

using namespace noise::utils;

GradientLegend::GradientLegend (const QString& name) : IntervalLegend (name) {


}

GradientLegend::~GradientLegend() {

}

QColor GradientLegend::lookup (const double& index) {
    std::map<double, QColor>::iterator i = std::find_if_not (_entries.begin(), _entries.end(), [&index] (std::pair<double, QColor> entry) -> bool { return entry.first <= index; } );
    QColor color = interpolateColors (i, i--, index);
    return color;
}

QColor GradientLegend::interpolateColors (std::map<double, QColor>::iterator lower, std::map<double, QColor>::iterator higher, const double& index) {
    QColor color;
    color.setRedF (interpolateValues (lower -> first, lower -> second.redF(), higher -> first, higher -> second.redF(), index));
    color.setGreenF (interpolateValues (lower -> first, lower -> second.greenF(), higher -> first, higher -> second.greenF(), index));
    color.setBlueF (interpolateValues (lower -> first, lower -> second.blueF(), higher -> first, higher -> second.blueF(), index));
    color.setAlphaF (interpolateValues (lower -> first, lower -> second.alphaF(), higher -> first, higher -> second.alphaF(), index));
    return color;
}

double GradientLegend::interpolateValues (const double& p1, const double& v1, const double& p2, const double& v2, const double& index) {
    if (p1 > p2) { return interpolateValues (p2, v2, p1, v1, index); }
    if (index < p1) return v1;
    if (index > p2) return v2;
    double delta = p2 - p1;
    double weighting = (index - p1) / delta;
    double result = v1 + ((v2 - v1) * weighting);
    return result;
}