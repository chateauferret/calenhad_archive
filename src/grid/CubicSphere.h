//
// Created by martin on 14/10/2019.
//

#ifndef MESSAGES_CUBICSPHERE_H
#define MESSAGES_CUBICSPHERE_H

#include <array>
#include <GL/gl.h>
#include <QtGui/QImage>
#include "../geoutils.h"

namespace calenhad {
    namespace mapping {
        class Statistics;


        struct CubeCoordinates {
        public:
            int u, v;
            int face;
        };
    }
    namespace grid {
        class CubicSphere {

        public:
            explicit CubicSphere (const int& depth = 12);
            ~CubicSphere();
            long count() const;

            float* grid ();
            calenhad::mapping::Statistics statistics() const;
            float valueAt (const geoutils::Geolocation&);
            int size () const;
            void heightmap (const int& face, QImage* image);
            void exportHeightmaps (const QString& filename);
            GLfloat* data();

        protected:
            float* _grid;
            const int FACE_FRONT = 0, FACE_BACK = 3, FACE_NORTH = 1, FACE_SOUTH = 4, FACE_WEST = 2, FACE_EAST = 5;
            const double HALF_ROOT_2 = 0.70710676908493042;

            void toCartesian (const mapping::CubeCoordinates& fuv, geoutils::Cartesian& xyz) const;
            void toCubeCoordinates (mapping::CubeCoordinates& fuv, const geoutils::Cartesian& xyz);

            int _size;
            int _renderTime;

            float valueAt(const mapping::CubeCoordinates &fuv);
        };
    }
}




#endif //MESSAGES_CUBICSPHERE_H
