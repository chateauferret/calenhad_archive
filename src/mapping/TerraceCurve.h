

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

#ifndef CALENHAD_TERRACE_CURVE_H
#define CALENHAD_TERRACE_CURVE_H

#include "Curve.h"

namespace calenhad {
    namespace mapping {

        /// Noise module that maps the output value from a source module onto a
        /// terrace-forming curve.
        ///
        /// @image html moduleterrace.png
        ///
        /// This noise module maps the output value from the source module onto a
        /// terrace-forming curve.  The start of this curve has a slope of zero;
        /// its slope then smoothly increases.  This curve also contains
        /// <i>control points</i> which resets the slope to zero at that point,
        /// producing a "terracing" effect.  Refer to the following illustration:
        ///
        /// @image html terrace.png
        ///
        /// To add a control point to this noise module, call the
        /// AddControlPoint() method.
        ///
        /// An application must add a minimum of two control points to the curve.
        /// If this is not done, the GetValue() method fails.  The control points
        /// can have any value, although no two control points can have the same
        /// value.  There is no limit to the number of control points that can be
        /// added to the curve.
        ///
        /// This noise module clamps the output value from the source module if
        /// that value is less than the value of the lowest control point or
        /// greater than the value of the highest control point.
        ///
        /// This noise module is often used to generate terrain features such as
        /// your stereotypical desert canyon.
        ///
        /// This noise module requires one source module.
        class TerraceCurve : public Curve {

        public:

        /// Constructor.
        TerraceCurve();

        /// Destructor.
        ~TerraceCurve ();

        void InvertTerraces (bool invert = true) {
            m_invertTerraces = invert;
        }

        /// Determines if the terrace-forming curve between the control
        /// points is inverted.
        ///
        /// @returns
        /// - @a true if the curve between the control points is inverted.
        /// - @a false if the curve between the control points is not
        ///   inverted.
        bool IsTerracesInverted () const {
            return m_invertTerraces;
        }

        virtual float GetValue (const float& sourceModuleValue) const override;



        /// Determines if the terrace-forming curve between all control points
        /// is inverted.
        bool m_invertTerraces;


    };

}

}

#endif
