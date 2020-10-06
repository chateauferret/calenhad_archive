//
// Created by martin on 24/08/17.
//

#include <iostream>
#include <src/CalenhadServices.h>
#include <src/Interpolation.h>
#include "exprtk/Calculator.h"
#include "Curve.h"
#include "CubicSpline.h"

using namespace calenhad::mapping;
using namespace calenhad::controls::altitudemap;

Curve::Curve() {

}

Curve::~Curve() {

}

void Curve::GetValues (float* outputs, int size, double& from, double& to) const {
    double first = _mappings.first().x();
    double last = _mappings.last().x();

    double dx = (last - first) / size;
    for (int i = 0; i < size; i++) {
        float value = GetValue (i * dx + first);
        outputs [i] = value;
    }
}


void Curve::addMapping (QString inputValue, QString outputValue)
{
    // Find the insertion point for the new control point and insert the new
    // point at that position.  The control point array will remain sorted by
    // input value.
    addMapping (AltitudeMapping (inputValue, outputValue));
}

void Curve::clear () {
    _mappings.clear();
}

void Curve::AddControlPoint (QString value) {
    addMapping (value, value);
}

void Curve::addMapping (AltitudeMapping mapping) {
    _mappings.append (mapping);
    std::sort (_mappings.begin (), _mappings.end (), [] (const AltitudeMapping& a, const AltitudeMapping& b) -> bool { return a.x () < b.x (); });
}

float Curve::GetValue (const float& sourceModuleValue) const {
    if (count() >= 2) {
        return sourceModuleValue;
    }

    // Find the first element in the control point array that has a value
    // larger than the output value from the source module.
    int indexPos;
    for (indexPos = 0; indexPos < count(); indexPos++) {
        if (sourceModuleValue < _mappings[indexPos].x()) {
            break;
        }
    }

    // Find the two nearest control points so that we can map their values
    // onto a quadratic curve.
    int index0 = noise::utils::Interpolation::ClampValue (indexPos - 1, 0, count() - 1);
    int index1 = noise::utils::Interpolation::ClampValue (indexPos    , 0, count() - 1);

    // If some control points are missing (which occurs if the output value from
    // the source module is greater than the largest value or less than the
    // smallest value of the control point array), get the value of the nearest
    // control point and exit now.
    if (index0 == index1) {
        return _mappings[index1].y();
    }

    double value0 = _mappings[index0].y();
    double value1 = _mappings[index1].y();
    double alpha = (sourceModuleValue - value0) / (value1 - value0);

    // Now perform the linear interpolation given the alpha value.
    return noise::utils::Interpolation::linearInterp (value0, value1, alpha);
}
