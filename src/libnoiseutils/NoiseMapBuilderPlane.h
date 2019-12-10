//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERPLANE_H
#define CALENHAD_NOISEMAPBUILDERPLANE_H



/// Builds a planar noise map.
///
/// This class builds a noise map by filling it with coherent-noise values
/// generated from the surface of a plane.
///
/// This class describes these input values using (x, z) coordinates.
/// Their y coordinates are always 0.0.
///
/// The application must provide the lower and upper x coordinate bounds
/// of the noise map, in units, and the lower and upper z coordinate
/// bounds of the noise map, in units.
///
/// To make a tileable noise map with no seams at the edges, call the
/// EnableSeamless() method.

#include "NoiseConstants.h"
#include "NoiseMapBuilder.h"

namespace noise {
    namespace utils {


        class NoiseMapBuilderPlane : public NoiseMapBuilder {

        public:

            /// Constructor.
            NoiseMapBuilderPlane ();

            virtual void Build ();

            /// Enables or disables seamless tiling.
            ///
            /// @param enable A flag that enables or disables seamless tiling.
            ///
            /// Enabling seamless tiling builds a noise map with no seams at the
            /// edges.  This allows the noise map to be tileable.
            void EnableSeamless (bool enable = true) {
                m_isSeamlessEnabled = enable;
            }

            /// Returns the lower x boundary of the planar noise map.
            ///
            /// @returns The lower x boundary of the planar noise map, in units.
            double GetLowerXBound () const {
                return m_lowerXBound;
            }

            /// Returns the lower z boundary of the planar noise map.
            ///
            /// @returns The lower z boundary of the noise map, in units.
            double GetLowerZBound () const {
                return m_lowerZBound;
            }

            /// Returns the upper x boundary of the planar noise map.
            ///
            /// @returns The upper x boundary of the noise map, in units.
            double GetUpperXBound () const {
                return m_upperXBound;
            }

            /// Returns the upper z boundary of the planar noise map.
            ///
            /// @returns The upper z boundary of the noise map, in units.
            double GetUpperZBound () const {
                return m_upperZBound;
            }

            /// Determines if seamless tiling is enabled.
            ///
            /// @returns
            /// - @a true if seamless tiling is enabled.
            /// - @a false if seamless tiling is disabled.
            ///
            /// Enabling seamless tiling builds a noise map with no seams at the
            /// edges.  This allows the noise map to be tileable.
            bool IsSeamlessEnabled () const {
                return m_isSeamlessEnabled;
            }

            /// Sets the boundaries of the planar noise map.
            ///
            /// @param lowerXBound The lower x boundary of the noise map, in
            /// units.
            /// @param upperXBound The upper x boundary of the noise map, in
            /// units.
            /// @param lowerZBound The lower z boundary of the noise map, in
            /// units.
            /// @param upperZBound The upper z boundary of the noise map, in
            /// units.
            ///
            /// @pre The lower x boundary is less than the upper x boundary.
            /// @pre The lower z boundary is less than the upper z boundary.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            void SetBounds (double lowerXBound, double upperXBound,
                            double lowerZBound, double upperZBound) {
                if (lowerXBound >= upperXBound
                    || lowerZBound >= upperZBound) {
                    throw noise::ExceptionInvalidParam ();
                }

                m_lowerXBound = lowerXBound;
                m_upperXBound = upperXBound;
                m_lowerZBound = lowerZBound;
                m_upperZBound = upperZBound;
            }

        private:

            /// A flag specifying whether seamless tiling is enabled.
            bool m_isSeamlessEnabled;

            /// Lower x boundary of the planar noise map, in units.
            double m_lowerXBound;

            /// Lower z boundary of the planar noise map, in units.
            double m_lowerZBound;

            /// Upper x boundary of the planar noise map, in units.
            double m_upperXBound;

            /// Upper z boundary of the planar noise map, in units.
            double m_upperZBound;

        };
    }
}

#endif //CALENHAD_NOISEMAPBUILDERPLANE_H
