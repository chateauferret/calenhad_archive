//
// Created by martin on 14/10/2019.
//

#ifndef MESSAGES_GEOMETRY_H
#define MESSAGES_GEOMETRY_H

namespace calenhad {
    namespace icosphere {
        namespace geometry {
            struct Cartesian {
            public:
                double x, y, z;
            };

            struct LatLon {
            public:
                double lat, lon, height;
            };

            struct CubeCoordinates {
            public:
                int u, v;
                int face;
            };

        }
}

};


#endif //MESSAGES_GEOMETRY_H
