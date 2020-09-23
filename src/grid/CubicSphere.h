//
// Created by martin on 14/10/2019.
//

#ifndef MESSAGES_CUBICSPHERE_H
#define MESSAGES_CUBICSPHERE_H

#include <array>
#include <GL/gl.h>
#include <QtGui/QImage>
#include <src/CalenhadServices.h>
#include "../geoutils.h"
#include "../CalenhadServices.h"

#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_NORTH 2
#define FACE_SOUTH 3
#define FACE_EAST 4
#define FACE_WEST 5

namespace calenhad {
    class CalenhadServices;
    namespace controls {
        namespace globe {
            struct CalenhadStatistics;
        }
    }
    namespace mapping {

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
            CubicSphere (CubicSphere* other);

// Neighbouring panels for each panel, in the order +x, -x, +y, -y
            const int ADJACENT [6] [4] = { { 4, 5, 2, 3 }, { 4, 5, 2, 3, }, { 4, 5, 0, 1 }, { 4, 5, 0, 1 }, { 2, 3, 0, 1 }, { 2, 3, 0, 1 } };


            //explicit CubicSphere (const int& depth);
            CubicSphere (const int& depth);
            ~CubicSphere();
            long count() const;

            float* grid ();
            calenhad::controls::globe::CalenhadStatistics statistics (controls::globe::CalenhadStatistics& statistics) const;
            float valueAt (const geoutils::Geolocation&);
            int size () const;

            void heightmap (const int& face, QImage* image);
            void exportHeightmaps (const QString& filename);
            GLfloat* data();

            void toCubeCoordinates (mapping::CubeCoordinates& fuv, const geoutils::Cartesian& xyz) const;

            void makeTile (const int& x, const int& y, CubicSphere* source);

            float valueAt (const mapping::CubeCoordinates &fuv);

            void toGeolocation (const mapping::CubeCoordinates& fuv, geoutils::Geolocation& g) const;

            void fromRaster (QImage* image);

            void setComputeTime (double d);

            void copy (CubicSphere* other);

        protected:
            float* _grid;
            double _computeTime;
            const double HALF_ROOT_2 = 0.70710676908493042;

            void toCartesian (const mapping::CubeCoordinates& fuv, geoutils::Cartesian& xyz) const;

            int _size, _depth;
            int _renderTime;

            void initialise();
            double computeTime() const;

            void adjacent (const mapping::CubeCoordinates& fuv, mapping::CubeCoordinates* m);

            void surrounding (const mapping::CubeCoordinates& fuv, mapping::CubeCoordinates* k);
        };
    }
}




#endif //MESSAGES_CUBICSPHERE_H
