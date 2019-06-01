//
// Created by martin on 09/03/17.
//

#ifndef CALENHAD_BOUNDS_H
#define CALENHAD_BOUNDS_H

#include <math.h>
#include "../geoutils.h"
namespace calenhad {
    namespace icosphere {
        class Bounds {
        public:
            Bounds (double n = M_PI_2, double s = -M_PI_2, double e = M_PI, double w = -M_PI, geoutils::Units units = geoutils::Units::Radians);

            int estimateVertexCount (const int& level) const;

            bool operator== (const Bounds& other) const;

            bool operator!= (const Bounds& other) const;

            double width () const;

            double height () const;

            double north () const;;

            double south () const;;

            double east () const;;

            double west () const;;

            bool crossesDateline ();

            geoutils::Geolocation center () const;

        protected:
            double _north, _south, _west, _east;

        };
    }
}

#endif //CALENHAD_BOUNDS_H