//
// Created by martin on 09/03/17.
//

#ifndef CALENHAD_BOUNDS_H
#define CALENHAD_BOUNDS_H

#include <math.h>
#include "marble/GeoDataLatLonBox.h"
namespace icosphere {
    class Bounds : public Marble::GeoDataLatLonBox {
    public:
        Bounds (double n = M_PI_2, double s = -M_PI_2, double e = M_PI, double w = - M_PI);
        int estimateVertexCount (const int& level)const;

    };
}

#endif //CALENHAD_BOUNDS_H