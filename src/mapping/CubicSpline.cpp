//
// Copyright (C) 2003, 2004 Jason Bevins as adapted from libnoise.
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include <iostream>
#include "src/Interpolation.h"
#include "CubicSpline.h"


using namespace calenhad::mapping;

CubicSpline::CubicSpline() : Curve() {

}

CubicSpline::~CubicSpline() {

}


float CubicSpline::GetValue (const float& sourceModuleValue) const {
    if (count()  < 4) {
        return sourceModuleValue;
    }

    // Find the first element in the control point array that has an input value
    // larger than the output value from the source module.
    int indexPos;
    for (indexPos = 0; indexPos < count(); indexPos++) {
        if (sourceModuleValue < _mappings[indexPos].x()) {
            break;
        }
    }


    // Find the four nearest control points so that we can perform cubic
    // interpolation.
    int index0 = noise::utils::Interpolation::ClampValue (indexPos - 2, 0, count() - 1);
    int index1 = noise::utils::Interpolation::ClampValue (indexPos - 1, 0, count() - 1);
    int index2 = noise::utils::Interpolation::ClampValue (indexPos    , 0, count() - 1);
    int index3 = noise::utils::Interpolation::ClampValue (indexPos + 1, 0, count() - 1);

    // If some control points are missing (which occurs if the value from the
    // source module is greater than the largest input value or less than the
    // smallest input value of the control point array), get the corresponding
    // output value of the nearest control point and exit now.
    if (index1 == index2) {
        return _mappings[index1].y();
    }

    // Compute the alpha value used for cubic interpolation.
    float input0 = _mappings[index1].x();
    float input1 = _mappings[index2].x();
    float alpha = (sourceModuleValue - input0) / (input1 - input0);


    // Now perform the cubic interpolation given the alpha value.
    double map_out = noise::utils::Interpolation::cubicInterp (
            _mappings[index0].y(),
            _mappings[index1].y(),
            _mappings[index2].y(),
            _mappings[index3].y(),
            alpha);

    return map_out;
}
