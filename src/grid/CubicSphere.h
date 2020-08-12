//
// Created by martin on 14/10/2019.
//

#ifndef MESSAGES_CUBICSPHERE_H
#define MESSAGES_CUBICSPHERE_H

#include <array>
#include <GL/gl.h>
#include <QtGui/QImage>
#include "../geoutils.h"

#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_NORTH 2
#define FACE_SOUTH 3
#define FACE_EAST 4
#define FACE_WEST 5

namespace calenhad {
    namespace mapping {
        class Statistics;


        struct CubeCoordinates {
        public:
            int u, v;
            int face;
            explicit CubeCoordinates (const int& f = 0, const int& x = 0.0, const int& y = 0.0) : u (x), v (y), face (f) { }
        };
    }
    namespace grid {
        class CubicSphere {

        public:
            //explicit CubicSphere (const int& depth);
            CubicSphere();
            ~CubicSphere();
            long count() const;

            float* grid ();
            calenhad::mapping::Statistics statistics() const;
            float valueAt (const geoutils::Geolocation&);
            int size () const;
            void heightmap (const int& face, QImage* image);
            void exportHeightmaps (const QString& filename);
            GLfloat* data();

            void toCubeCoordinates (mapping::CubeCoordinates& fuv, const geoutils::Cartesian& xyz) const;

        protected:
            float* _grid;

            const double HALF_ROOT_2 = 0.70710676908493042;

            void toCartesian (const mapping::CubeCoordinates& fuv, geoutils::Cartesian& xyz) const;
            int adjacentFace (const int& face, const int& direction);

            int _size;
            int _renderTime;

            float valueAt(const mapping::CubeCoordinates &fuv);

            void initialise();

            mapping::CubeCoordinates traverse (const mapping::CubeCoordinates& cube, const int& up, const int& right);
        };
    }
}




#endif //MESSAGES_CUBICSPHERE_H
