//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_RENDERERNORMALMAP_H
#define CALENHAD_RENDERERNORMALMAP_H

#include "NoiseContstants.h"
#include <QImage>

namespace noise {
    namespace utils {

        /// Renders a normal map from a noise map.
        ///
        /// This class renders an image containing the normal vectors from a noise
        /// map object.  This image can then be used as a bump map for a 3D
        /// application or game.
        ///
        /// This class encodes the (x, y, z) components of the normal vector into
        /// the (red, green, blue) channels of the image.  Like any 24-bit
        /// true-color image, the channel values range from 0 to 255.  0
        /// represents a normal coordinate of -1.0 and 255 represents a normal
        /// coordinate of +1.0.
        ///
        /// You should also specify the <i>bump height</i> before rendering the
        /// normal map.  The bump height specifies the ratio of spatial resolution
        /// to elevation resolution.  For example, if your noise map has a spatial
        /// resolution of 30 meters and an elevation resolution of one meter, set
        /// the bump height to 1.0 / 30.0.
        ///
        /// <b>Rendering the normal map</b>
        ///
        /// To render the image containing the normal map, perform the following
        /// steps:
        /// - Pass a NoiseMap object to the SetSourceNoiseMap() method.
        /// - Pass an Image object to the SetDestImage() method.
        /// - Call the Render() method.

        class NoiseMap;

        class RendererNormalMap {

        public:

            /// Constructor.
            RendererNormalMap ();

            /// Enables or disables noise-map wrapping.
            ///
            /// @param enable A flag that enables or disables noise-map wrapping.
            ///
            /// This object requires three points (the initial point and the right
            /// and up neighbors) to calculate the normal vector at that point.
            /// If wrapping is/ enabled, and the initial point is on the edge of
            /// the noise map, the appropriate neighbors that lie outside of the
            /// noise map will "wrap" to the opposite side(s) of the noise map.
            /// Otherwise, the appropriate neighbors are cropped to the edge of
            /// the noise map.
            ///
            /// Enabling wrapping is useful when creating spherical and tileable
            /// normal maps.
            void EnableWrap (bool enable = true) {
                m_isWrapEnabled = enable;
            }

            /// Returns the bump height.
            ///
            /// @returns The bump height.
            ///
            /// The bump height specifies the ratio of spatial resolution to
            /// elevation resolution.  For example, if your noise map has a
            /// spatial resolution of 30 meters and an elevation resolution of one
            /// meter, set the bump height to 1.0 / 30.0.
            ///
            /// The spatial resolution and elevation resolution are determined by
            /// the application.
            double GetBumpHeight () const {
                return m_bumpHeight;
            }

            /// Determines if noise-map wrapping is enabled.
            ///
            /// @returns
            /// - @a true if noise-map wrapping is enabled.
            /// - @a false if noise-map wrapping is disabled.
            ///
            /// This object requires three points (the initial point and the right
            /// and up neighbors) to calculate the normal vector at that point.
            /// If wrapping is/ enabled, and the initial point is on the edge of
            /// the noise map, the appropriate neighbors that lie outside of the
            /// noise map will "wrap" to the opposite side(s) of the noise map.
            /// Otherwise, the appropriate neighbors are cropped to the edge of
            /// the noise map.
            ///
            /// Enabling wrapping is useful when creating spherical and tileable
            /// normal maps.
            bool IsWrapEnabled () const {
                return m_isWrapEnabled;
            }

            /// Renders the noise map to the destination image.
            ///
            /// @pre SetSourceNoiseMap() has been previously called.
            /// @pre SetDestImage() has been previously called.
            ///
            /// @post The original contents of the destination image is destroyed.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            void Render ();

            /// Sets the bump height.
            ///
            /// @param bumpHeight The bump height.
            ///
            /// The bump height specifies the ratio of spatial resolution to
            /// elevation resolution.  For example, if your noise map has a
            /// spatial resolution of 30 meters and an elevation resolution of one
            /// meter, set the bump height to 1.0 / 30.0.
            ///
            /// The spatial resolution and elevation resolution are determined by
            /// the application.
            void SetBumpHeight (double bumpHeight) {
                m_bumpHeight = bumpHeight;
            }

            /// Sets the destination image.
            ///
            /// @param destImage The destination image.
            ///
            /// The destination image will contain the normal map after a
            /// successful call to the Render() method.
            ///
            /// The destination image must exist throughout the lifetime of this
            /// object unless another image replaces that image.
            void SetDestImage (QImage& destImage) {
                m_pDestImage = &destImage;
            }

            /// Sets the source noise map.
            ///
            /// @param sourceNoiseMap The source noise map.
            ///
            /// The destination image must exist throughout the lifetime of this
            /// object unless another image replaces that image.
            void SetSourceNoiseMap (const NoiseMap& sourceNoiseMap) {
                m_pSourceNoiseMap = &sourceNoiseMap;
            }

        private:

            /// Calculates the normal vector at a given point on the noise map.
            ///
            /// @param nc The height of the given point in the noise map.
            /// @param nr The height of the left neighbor.
            /// @param nu The height of the up neighbor.
            /// @param bumpHeight The bump height.
            ///
            /// @returns The normal vector represented as a color.
            ///
            /// This method encodes the (x, y, z) components of the normal vector
            /// into the (red, green, blue) channels of the returned color.  In
            /// order to represent the vector as a color, each coordinate of the
            /// normal is mapped from the -1.0 to 1.0 range to the 0 to 255 range.
            ///
            /// The bump height specifies the ratio of spatial resolution to
            /// elevation resolution.  For example, if your noise map has a
            /// spatial resolution of 30 meters and an elevation resolution of one
            /// meter, set the bump height to 1.0 / 30.0.
            ///
            /// The spatial resolution and elevation resolution are determined by
            /// the application.
            QColor CalcNormalColor (double nc, double nr, double nu, double bumpHeight) const;

            /// The bump height for the normal map.
            double m_bumpHeight;

            /// A flag specifying whether wrapping is enabled.
            bool m_isWrapEnabled;

            /// A pointer to the destination image.
            QImage* m_pDestImage;

            /// A pointer to the source noise map.
            const NoiseMap* m_pSourceNoiseMap;

        };

    }
}

#endif //CALENHAD_RENDERERNORMALMAP_H
