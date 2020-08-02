//
// Created by martin on 14/10/2019.
//

#ifndef MESSAGES_GEOMETRY_H
#define MESSAGES_GEOMETRY_H

namespace calenhad {
    namespace grid {
        namespace geometry {
            struct Cartesian {
            public:
                double x, y, z;
            };

            struct LatLon {
            public:
                LatLon (double y, double x) : lat (y), lon (x), height (0.0) {

                }

                double lat, lon, height;
            };


        }
}

};


#endif //MESSAGES_GEOMETRY_H
