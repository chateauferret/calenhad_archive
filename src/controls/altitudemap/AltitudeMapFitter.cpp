//
// Created by martin on 03/04/17.
//

#include "AltitudeMapFitter.h"
#include "src/Interpolation.h"
#include <qwt/qwt_plot.h>

using namespace calenhad::controls::altitudemap;
//! Constructor
AltitudeMapFitter::AltitudeMapFitter(): QwtSplineCurveFitter() {
}

//! Destructor
AltitudeMapFitter::~AltitudeMapFitter() {
}

/*
Fit the points to the same curve as is used by the noise owner to interpolate values
\param points Series of data points
\return Fitted Curve
\sa fitCurvePath()
*/
QPolygonF AltitudeMapFitter::fitCurve (const QPolygonF &points) const {

    QPolygonF fittedPoints (splineSize());

    const double x1 = points [0].x();
    const double x2 = points [int (points.size() - 1)].x();
    const double dx = x2 - x1;
    const double delta = dx / (splineSize() - 1);

    for (int i = 0; i < splineSize(); i++) {
        QPointF &p = fittedPoints [i];

        const double v = x1 + i * delta;
        const double w = getY (v, points);

        p.setX (qRound (v));
        p.setY (qRound (w));
    }
    return fittedPoints;

}

double AltitudeMapFitter::getY (const double& x, const QPolygonF& points) const {
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
    int index0 = noise::utils::Interpolation::ClampValue (indexPos - 2, 0, points.size() - 1);
    int index1 = noise::utils::Interpolation::ClampValue (indexPos - 1, 0, points.size() - 1);
    int index2 = noise::utils::Interpolation::ClampValue (indexPos, 0, points.size() - 1);
    int index3 = noise::utils::Interpolation::ClampValue (indexPos + 1, 0, points.size() - 1);

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

    // Now perform the cubic interpolation given the alpha value.
    return noise::utils::Interpolation::cubicInterp (
            points.at (index0).y(),
            points.at (index1).y(),
            points.at (index2).y(),
            points.at (index3).y(),
            alpha);
}