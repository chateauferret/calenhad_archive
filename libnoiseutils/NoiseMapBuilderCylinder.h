//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERCYLINDER_H
#define CALENHAD_NOISEMAPBUILDERCYLINDER_H

#include "NoiseContstants.h"
#include "NoiseMapBuilder.h"
namespace noise {
    namespace utils {

/// Builds a cylindrical noise map.
///
/// This class builds a noise map by filling it with coherent-noise values
/// generated from the surface of a cylinder.
///
/// This class describes these input values using an (angle, height)
/// coordinate system.  After generating the coherent-noise value from the
/// input value, it then "flattens" these coordinates onto a plane so that
/// it can write the values into a two-dimensional noise map.
///
/// The cylinder model has a radius of 1.0 unit and has infinite height.
/// The cylinder is oriented along the @a y axis.  Its center is at the
/// origin.
///
/// The x coordinate in the noise map represents the angle around the
/// cylinder's y axis.  The y coordinate in the noise map represents the
/// height above the x-z plane.
///
/// The application must provide the lower and upper angle bounds of the
/// noise map, in degrees, and the lower and upper height bounds of the
/// noise map, in units.
        class NoiseMapBuilderCylinder : public NoiseMapBuilder {

        public:

            /// Constructor.
            NoiseMapBuilderCylinder ();

            virtual void Build ();

            /// Returns the lower angle boundary of the cylindrical noise map.
            ///
            /// @returns The lower angle boundary of the noise map, in degrees.
            double GetLowerAngleBound () const {
                return m_lowerAngleBound;
            }

            /// Returns the lower height boundary of the cylindrical noise map.
            ///
            /// @returns The lower height boundary of the noise map, in units.
            ///
            /// One unit is equal to the radius of the cylinder.
            double GetLowerHeightBound () const {
                return m_lowerHeightBound;
            }

            /// Returns the upper angle boundary of the cylindrical noise map.
            ///
            /// @returns The upper angle boundary of the noise map, in degrees.
            double GetUpperAngleBound () const {
                return m_upperAngleBound;
            }

            /// Returns the upper height boundary of the cylindrical noise map.
            ///
            /// @returns The upper height boundary of the noise map, in units.
            ///
            /// One unit is equal to the radius of the cylinder.
            double GetUpperHeightBound () const {
                return m_upperHeightBound;
            }

            /// Sets the coordinate boundaries of the noise map.
            ///
            /// @param lowerAngleBound The lower angle boundary of the noise map,
            /// in degrees.
            /// @param upperAngleBound The upper angle boundary of the noise map,
            /// in degrees.
            /// @param lowerHeightBound The lower height boundary of the noise
            /// map, in units.
            /// @param upperHeightBound The upper height boundary of the noise
            /// map, in units.
            ///
            /// @pre The lower angle boundary is less than the upper angle
            /// boundary.
            /// @pre The lower height boundary is less than the upper height
            /// boundary.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            ///
            /// One unit is equal to the radius of the cylinder.
            void SetBounds (double lowerAngleBound, double upperAngleBound,
                            double lowerHeightBound, double upperHeightBound) {
                if (lowerAngleBound >= upperAngleBound
                    || lowerHeightBound >= upperHeightBound) {
                    throw noise::ExceptionInvalidParam ();
                }

                m_lowerAngleBound = lowerAngleBound;
                m_upperAngleBound = upperAngleBound;
                m_lowerHeightBound = lowerHeightBound;
                m_upperHeightBound = upperHeightBound;
            }

        private:

            /// Lower angle boundary of the cylindrical noise map, in degrees.
            double m_lowerAngleBound;

            /// Lower height boundary of the cylindrical noise map, in units.
            double m_lowerHeightBound;

            /// Upper angle boundary of the cylindrical noise map, in degrees.
            double m_upperAngleBound;

            /// Upper height boundary of the cylindrical noise map, in units.
            double m_upperHeightBound;

        };
    }
}

#endif //CALENHAD_NOISEMAPBUILDERCYLINDER_H
