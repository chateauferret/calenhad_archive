//
// Created by martin on 23/08/17.
//
// extract from libnoise curve.h
//
// Copyright (C) 2003, 2004 Jason Bevins
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


#ifndef CALENHAD_CUBICSPLINE_H
#define CALENHAD_CUBICSPLINE_H

#include "Curve.h"

namespace calenhad {

    namespace mapping {

        /// Noise module that maps the output value from a source module onto an
        /// arbitrary function curve.
        ///
        /// @image html modulecurve.png
        ///
        /// This noise module maps the output value from the source module onto an
        /// application-defined curve.  This curve is defined by a number of
        /// <i>control points</i>; each control point has an <i>input value</i>
        /// that maps to an <i>output value</i>.  Refer to the following
        /// illustration:
        ///
        /// @image html curve.png
        ///
        /// To add the control points to this curve, call the AddControlPoint()
        /// method.
        ///
        /// Since this curve is a cubic spline, an application must add a minimum
        /// of four control points to the curve.  If this is not done, the
        /// GetValue() method fails.  Each control point can have any input and
        /// output value, although no two control points can have the same input
        /// value.  There is no limit to the number of control points that can be
        /// added to the curve.
        ///
        /// This noise module requires one source module.
        class CubicSpline : public Curve {

        public:

            /// Constructor.
            CubicSpline ();

            /// Destructor.
            ~CubicSpline ();




            virtual float GetValue (const float& input) const override;

        protected:


        };
    }
}
#endif //CALENHAD_CUBICSPLINE_H
