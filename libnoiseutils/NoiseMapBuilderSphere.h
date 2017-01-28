//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERSPHERE_H
#define CALENHAD_NOISEMAPBUILDERSPHERE_H

#include "NoiseContstants.h"
#include "NoiseMapBuilder.h"

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
        class NoiseMapBuilderSphere: public NoiseMapBuilder
        {

        public:

            /// Constructor.
            NoiseMapBuilderSphere ();

            virtual void Build ();

            /// Returns the eastern boundary of the spherical noise map.
            ///
            /// @returns The eastern boundary of the noise map, in degrees.
            double GetEastLonBound () const
            {
                return m_eastLonBound;
            }

            /// Returns the northern boundary of the spherical noise map
            ///
            /// @returns The northern boundary of the noise map, in degrees.
            double GetNorthLatBound () const
            {
                return m_northLatBound;
            }

            /// Returns the southern boundary of the spherical noise map
            ///
            /// @returns The southern boundary of the noise map, in degrees.
            double GetSouthLatBound () const
            {
                return m_southLatBound;
            }

            /// Returns the western boundary of the spherical noise map
            ///
            /// @returns The western boundary of the noise map, in degrees.
            double GetWestLonBound () const
            {
                return m_westLonBound;
            }

            /// Sets the coordinate boundaries of the noise map.
            ///
            /// @param southLatBound The southern boundary of the noise map, in
            /// degrees.
            /// @param northLatBound The northern boundary of the noise map, in
            /// degrees.
            /// @param westLonBound The western boundary of the noise map, in
            /// degrees.
            /// @param eastLonBound The eastern boundary of the noise map, in
            /// degrees.
            ///
            /// @pre The southern boundary is less than the northern boundary.
            /// @pre The western boundary is less than the eastern boundary.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            void SetBounds (double southLatBound, double northLatBound,
                            double westLonBound, double eastLonBound)
            {
                if (southLatBound >= northLatBound
                    || westLonBound >= eastLonBound) {
                    throw noise::ExceptionInvalidParam ();
                }

                m_southLatBound = southLatBound;
                m_northLatBound = northLatBound;
                m_westLonBound  = westLonBound ;
                m_eastLonBound  = eastLonBound ;
            }

        private:

            /// Eastern boundary of the spherical noise map, in degrees.
            double m_eastLonBound;

            /// Northern boundary of the spherical noise map, in degrees.
            double m_northLatBound;

            /// Southern boundary of the spherical noise map, in degrees.
            double m_southLatBound;

            /// Western boundary of the spherical noise map, in degrees.
            double m_westLonBound;

        };


    }
}


#endif //CALENHAD_NOISEMAPBUILDERSPHERE_H
