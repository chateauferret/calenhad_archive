//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERSPHERE_H
#define CALENHAD_NOISEMAPBUILDERSPHERE_H

#include "NoiseConstants.h"
#include "NoiseMapBuilder.h"
#include <marble/GeoDataLatLonBox.h>
namespace calenhad {
    namespace pipeline {
        class ImageRenderJob;
    }
}

namespace noise {
    namespace utils {


        /// Builds a spherical noise map.
        ///
        /// This class builds a noise map by filling it with coherent-noise values
        /// generated from the surface of a sphere.
        ///
        /// This class describes these input values using a (latitude, longitude)
        /// coordinate system.  After generating the coherent-noise value from the
        /// input value, it then "flattens" these coordinates onto a plane so that
        /// it can write the values into a two-dimensional noise map.
        ///
        /// The sphere model has a radius of 1.0 unit.  Its center is at the
        /// origin.
        ///
        /// The x coordinate in the noise map represents the longitude.  The y
        /// coordinate in the noise map represents the latitude.
        ///
        /// The application must provide the southern, northern, western, and
        /// eastern bounds of the noise map, in degrees.
        class NoiseMapBuilderSphere: public QObject, public noise::utils::NoiseMapBuilder {
        Q_OBJECT
        public:

            /// Constructor.
            NoiseMapBuilderSphere (calenhad::pipeline::ImageRenderJob* job);

            virtual void build ();

            /// Returns the boundary of the spherical noise map.
            ///
            /// @returns The boundary of the noise map, in degrees.
            Marble::GeoDataLatLonBox bounds () const {
                return _bounds;
            }



            /// Sets the coordinate boundaries of the noise map.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            void SetBounds (Marble::GeoDataLatLonBox bounds) {
                _bounds = bounds;
            }

        protected:

            /// Boundary of the spherical noise map, in degrees.
            Marble::GeoDataLatLonBox _bounds;

            double _curLat, _curLon, _xDelta, _yDelta;
            calenhad::pipeline::ImageRenderJob* _job;
            void prepare ();
        };


    }
}


#endif //CALENHAD_NOISEMAPBUILDERSPHERE_H
