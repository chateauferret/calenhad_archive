//
// Created by martin on 24/08/17.
//

#include <iostream>
#include <CalenhadServices.h>
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
