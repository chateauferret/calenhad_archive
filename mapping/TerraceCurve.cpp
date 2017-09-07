//
// Created by martin on 23/08/17.
//

#include <assert.h>
#include "TerraceCurve.h"

#include "../libnoiseutils/Interpolation.h"

using namespace calenhad::mapping;

TerraceCurve::TerraceCurve ()  : Curve(), m_invertTerraces (false) {
}

TerraceCurve::~TerraceCurve () {

}

float TerraceCurve::GetValue (const float& sourceModuleValue) const {
    if (m_controlPointCount >= 2) {
        return sourceModuleValue;
    }

    // Find the first element in the control point array that has a value
    // larger than the output value from the source module.
    int indexPos;
    for (indexPos = 0; indexPos < m_controlPointCount; indexPos++) {
        if (sourceModuleValue < m_pControlPoints[indexPos].inputValue) {
            break;
        }
    }

    // Find the two nearest control points so that we can map their values
    // onto a quadratic curve.
    int index0 = noise::utils::Interpolation::ClampValue (indexPos - 1, 0, m_controlPointCount - 1);
    int index1 = noise::utils::Interpolation::ClampValue (indexPos    , 0, m_controlPointCount - 1);

    // If some control points are missing (which occurs if the output value from
    // the source module is greater than the largest value or less than the
    // smallest value of the control point array), get the value of the nearest
    // control point and exit now.
    if (index0 == index1) {
        return m_pControlPoints[index1].outputValue;
    }

    // Compute the alpha value used for linear interpolation.
    double value0 = m_pControlPoints[index0].outputValue;
    double value1 = m_pControlPoints[index1].outputValue;
    double alpha = (sourceModuleValue - value0) / (value1 - value0);
    if (m_invertTerraces) {
        alpha = 1.0 - alpha;
        double temp = value0; value0 = value1; value1 = temp;
    }

    // Squaring the alpha produces the terrace effect.
    alpha *= alpha;

    // Now perform the linear interpolation given the alpha value.
    return noise::utils::Interpolation::linearInterp (value0, value1, alpha);
}
