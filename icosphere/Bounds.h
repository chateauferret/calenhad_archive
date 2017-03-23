//
// Created by martin on 09/03/17.
//

#ifndef CALENHAD_BOUNDS_H
#define CALENHAD_BOUNDS_H

#include <math.h>
namespace icosphere {
    class Bounds {
    public:
        // in radians
        Bounds (double n = M_PI, double s = -M_PI, double w = -M_PI * 2, double e = M_PI * 2, bool crossesDateline = false);
        double lat1, lat2, lon1, lon2;
        bool _crossesDateline;
        bool operator== (const Bounds& other);
        bool operator!= (const Bounds& other);
        int estimateVertexCount (const int& level);
    };
}

#endif //CALENHAD_BOUNDS_H
