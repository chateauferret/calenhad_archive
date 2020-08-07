//
// Created by martin on 05/04/17.
//

#include <iostream>
#include <qwt/qwt_plot.h>
#include <cmath>
#include <src/Interpolation.h>
#include "TerraceMapFitter.h"

using namespace calenhad::controls::altitudemap;

//! Constructor
TerraceMapFitter::TerraceMapFitter() : QwtCurveFitter(), _resolution (10), _inverted (true), _plot (nullptr) {
}

//! Destructor
TerraceMapFitter::~TerraceMapFitter() {
}

void TerraceMapFitter::attachPlot (QwtPlot* plot) {
    if (! _plot) {
        _plot = plot;
    }
}

/*
Fit the points to the same curve as is used by the noise owner to interpolate values
\param points Series of data points
\return Fitted Curve
\sa fitCurvePath()
*/
QPolygonF TerraceMapFitter::fitCurve (const QPolygonF &points) const {

    QPolygonF fittedPoints (_resolution);

    const double x1 = points [0].x();
    const double x2 = points [int (points.size() - 1)].x();
    const double dx = x2 - x1;
    const double delta = dx / (_resolution - 1);

    for (int i = 0; i < _resolution; i++) {
        QPointF &p = fittedPoints [i];

        const double v = x1 + i * delta;
        const double w = getY (v, points);

        p.setX (qRound (v));
        p.setY (qRound (w));
    }
    return fittedPoints;

}

double TerraceMapFitter::getY (const double& x, const QPolygonF& points) const {
    // this stuff all comes from libnoise interp.cpp

    // Find the first element in the control point array that has an input value
    // larger than the output value from the source owner.
    int indexPos;
    for (indexPos = 0; indexPos < points.size(); indexPos++) {
        if (x < points.at (indexPos).x()) {
            break;
        }
    }

    // Find the four nearest control points so that we can perform cubic
    // interpolation.
    int index1 = noise::utils::Interpolation::ClampValue (indexPos - 1, 0, points.size() - 1);
    int index2 = noise::utils::Interpolation::ClampValue (indexPos, 0, points.size() - 1);

    // If some control points are missing (which occurs if the value from the
    // source owner is greater than the largest input value or less than the
    // smallest input value of the control point array), get the corresponding
    // output value of the nearest control point and exit now.
    if (index1 == index2) {
        return points.at (index1).y();
    }

    // Compute the alpha value used for cubic interpolation.
    double input0 = points.at (index1).x();
    double input1 = points.at (index2).x();
    double alpha = (x - input0) / (input1 - input0);

    if (_inverted) {
        alpha = 1.0 - alpha;
        int temp = index1; index1 = index2; index2 = temp;
    }

    alpha *= alpha;
    // Now perform the cubic interpolation given the alpha value.
    return  noise::utils::Interpolation::linearInterp  (
            points.at (index1).y(),
            points.at (index2).y(),
            alpha);
}
void TerraceMapFitter::setResolution (const int& resolution) {
    _resolution = resolution;
}

int TerraceMapFitter::resolution () {
    return _resolution;
}

void TerraceMapFitter::setInverted (const bool& inverted) {
    _inverted = inverted;
}

bool TerraceMapFitter::isInverted () const {
    return _inverted;
}
