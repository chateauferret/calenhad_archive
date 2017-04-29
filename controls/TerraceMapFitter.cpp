//
// Created by martin on 05/04/17.
//

#include <libnoise/misc.h>
#include <libnoise/interp.h>
#include <iostream>
#include <qwt/qwt_plot.h>
#include "TerraceMapFitter.h"


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
    // this stuff all comes from libnoise terrace.cpp and interp.cpp

    // Find the first element in the control point array that has an input value
    // larger than the output value from the source owner.
    // Find the first element in the control point array that has a value
    // larger than the output value from the source owner.

    // y-axis is oriented inversely on the screen from on the logical canvas so we need to do some maths.

    int indexPos;
    for (indexPos = 0; indexPos < points.size(); indexPos++) {
        if (x < points.at (indexPos).x()) {
            break;
        }
    }

    // Find the two nearest control points so that we can map their values
    // onto a quadratic curve.
    int index0 = noise::ClampValue (indexPos - 1, 0, points.size() - 2);
    int index1 = noise::ClampValue (indexPos, 0, points.size() - 1);

    // If some control points are missing (which occurs if the output value from
    // the source owner is greater than the largest value or less than the
    // smallest value of the control point array), get the value of the nearest
    // control point and exit now.
    if (index0 == index1) {
        return points.at (index1).x();
    }

    // Compute the alpha value used for linear interpolation.
    double value0 = points.at (index0).x();
    double value1 = points.at (index1).x();

    double v0 = _plot -> invTransform (QwtPlot::xBottom, value0);
    double v1 = _plot -> invTransform (QwtPlot::xBottom, value1);
    double xValue = _plot -> invTransform (QwtPlot::xBottom, x);
    double alpha = (xValue - v0) / (v1 - v0);
    if (_inverted) {
        alpha = 1.0 - alpha;
        noise::SwapValues (v0, v1);
    }

    // Squaring the alpha produces the terrace effect.
    alpha *= alpha;

    // Now perform the linear interpolation given the alpha value.

    double yValue = ((1.0 - alpha) * v0) + (alpha * v1);
    return _plot -> transform (QwtPlot::xBottom, - yValue);
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
