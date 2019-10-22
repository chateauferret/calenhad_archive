//
// Created by martin on 14/10/2019.
//

#ifndef MESSAGES_CUBICSPHERE_H
#define MESSAGES_CUBICSPHERE_H

#include <array>
#include "Geometry.h"

namespace calenhad {
    namespace icosphere {

        class CubicSphere {

        public:
            explicit CubicSphere (const int& depth);
            ~CubicSphere();
            long count ();
        protected:
            float* _grid;
            const int FACE_FRONT = 0, FACE_BACK = 3, FACE_NORTH = 1, FACE_SOUTH = 4, FACE_WEST = 2, FACE_EAST = 5;
            const double HALF_ROOT_2 = 0.70710676908493042;

            void toCartesian (const geometry::CubeCoordinates& fuv, geometry::Cartesian& xyz);
            void toCubeCoordinates (geometry::CubeCoordinates& fuv, const geometry::Cartesian& xyz);

            int _size;

            void truncate (geometry::Cartesian& c);


        };
    }
}




#endif //MESSAGES_CUBICSPHERE_H
