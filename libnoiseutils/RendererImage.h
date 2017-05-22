//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_RENDERERIMAGE_H
#define CALENHAD_RENDERERIMAGE_H

#include <libnoise/exception.h>
#include <QtGui/QImage>
#include <memory>
#include "NoiseContstants.h"
#include "../icosphere/legend.h"


class ImageRenderJob;

namespace noise {
    namespace utils {

        class Image;
        class NoiseMap;
        class QColorGradient;
        /// Renders an image from a noise map.
        ///
        /// This class renders an image given the contents of a noise-map object.
        ///
        /// An application can configure the output of the image in three ways:
        /// - Specify the color gradient.
        /// - Specify the light source parameters.
        /// - Specify the background image.
        ///
        /// <b>Specify the color gradient</b>
        ///
        /// This class uses a color gradient to calculate the color for each pixel
        /// in the destination image according to the value from the corresponding
        /// position in the noise map.
        ///
        /// A color gradient is a list of gradually-changing colors.  A color
        /// gradient is defined by a list of <i>gradient points</i>.  Each
        /// gradient point has a position and a color.  In a color gradient, the
        /// colors between two adjacent gradient points are linearly interpolated.
        ///
        /// For example, suppose this class contains the following color gradient:
        ///
        /// - -1.0 maps to dark blue.
        /// - -0.2 maps to light blue.
        /// - -0.1 maps to tan.
        /// - 0.0 maps to green.
        /// - 1.0 maps to white.
        ///
        /// The value 0.5 maps to a greenish-white color because 0.5 is halfway
        /// between 0.0 (mapped to green) and 1.0 (mapped to white).
        ///
        /// The value -0.6 maps to a medium blue color because -0.6 is halfway
        /// between -1.0 (mapped to dark blue) and -0.2 (mapped to light blue).
        ///
        /// The color gradient requires a minimum of two gradient points.
        ///
        /// This class contains two pre-made gradients: a grayscale gradient and a
        /// color gradient suitable for terrain.  To use these pre-made gradients,
        /// call the BuildGrayscaleGradient() or BuildTerrainGradient() methods,
        /// respectively.
        ///
        /// @note The color value passed to AddGradientPoint() has an alpha
        /// channel.  This alpha channel specifies how a pixel in the background
        /// image (if specified) is blended with the calculated color.  If the
        /// alpha value is high, this class weighs the blend towards the
        /// calculated color, and if the alpha value is low, this class weighs the
        /// blend towards the color from the corresponding pixel in the background
        /// image.
        ///
        /// <b>Specify the light source parameters</b>
        ///
        /// This class contains a parallel light source that lights the image.  It
        /// interprets the noise map as a bump map.
        ///
        /// To enable or disable lighting, pass a Boolean value to the
        /// EnableLight() method.
        ///
        /// To set the position of the light source in the "sky", call the
        /// SetLightAzimuth() and SetLightElev() methods.
        ///
        /// To set the color of the light source, call the SetLightColor() method.
        ///
        /// To set the intensity of the light source, call the SetLightIntensity()
        /// method.  A good intensity value is 2.0, although that value tends to
        /// "wash out" very light colors from the image.
        ///
        /// To set the contrast amount between areas in light and areas in shadow,
        /// call the SetLightContrast() method.  Determining the correct contrast
        /// amount requires some trial and error, but if your application
        /// interprets the noise map as a height map that has its elevation values
        /// measured in meters and has a horizontal resolution of @a h meters, a
        /// good contrast amount to use is ( 1.0 / @a h ).
        ///
        /// <b>Specify the background image</b>
        ///
        /// To specify a background image, pass an Image object to the
        /// SetBackgroundImage() method.
        ///
        /// This class determines the color of a pixel in the destination image by
        /// blending the calculated color with the color of the corresponding
        /// pixel from the background image.
        ///
        /// The blend amount is determined by the alpha of the calculated color.
        /// If the alpha value is high, this class weighs the blend towards the
        /// calculated color, and if the alpha value is low, this class weighs the
        /// blend towards the color from the corresponding pixel in the background
        /// image.
        ///
        /// <b>Rendering the image</b>
        ///
        /// To render the image, perform the following steps:
        /// - Pass a NoiseMap object to the SetSourceNoiseMap() method.
        /// - Pass an Image object to the SetDestImage() method.
        /// - Pass an Image object to the SetBackgroundImage() method (optional)
        /// - Call the Render() method.
        class RendererImage : public QObject {
        Q_OBJECT
        public:

            /// Constructor.
            RendererImage (ImageRenderJob* job);

            /// Enables or disables the light source.
            ///
            /// @param enable A flag that enables or disables the light source.
            ///
            /// If the light source is enabled, this object will interpret the
            /// noise map as a bump map.
            void setLightEnabled (bool enable = true) {
                _isLightEnabled = enable;
            }

            /// Enables or disables noise-map wrapping.
            ///
            /// @param enable A flag that enables or disables noise-map wrapping.
            ///
            /// This object requires five points (the initial point and its four
            /// neighbors) to calculate light shading.  If wrapping is enabled,
            /// and the initial point is on the edge of the noise map, the
            /// appropriate neighbors that lie outside of the noise map will
            /// "wrap" to the opposite side(s) of the noise map.  Otherwise, the
            /// appropriate neighbors are cropped to the edge of the noise map.
            ///
            /// Enabling wrapping is useful when creating spherical renderings and
            /// tileable textures.
            void setWrapEnabled (bool enable = true) {
                _isWrapEnabled = enable;
            }

            /// Returns the azimuth of the light source, in degrees.
            ///
            /// @returns The azimuth of the light source.
            ///
            /// The azimuth is the location of the light source around the
            /// horizon:
            /// - 0.0 degrees is east.
            /// - 90.0 degrees is north.
            /// - 180.0 degrees is west.
            /// - 270.0 degrees is south.
            double getLightAzimuth() const {
                return _lightAzimuth;
            }

            /// Returns the brightness of the light source.
            ///
            /// @returns The brightness of the light source.
            double getLightBrightness() const {
                return _lightBrightness;
            }

            /// Returns the color of the light source.
            ///
            /// @returns The color of the light source.
            QColor getLightColor() const {
                return _lightColor;
            }

            /// Returns the contrast of the light source.
            ///
            /// @returns The contrast of the light source.
            ///
            /// The contrast specifies how sharp the boundary is between the
            /// light-facing areas and the shadowed areas.
            ///
            /// The contrast determines the difference between areas in light and
            /// areas in shadow.  Determining the correct contrast amount requires
            /// some trial and error, but if your application interprets the noise
            /// map as a height map that has a spatial resolution of @a h meters
            /// and an elevation resolution of 1 meter, a good contrast amount to
            /// use is ( 1.0 / @a h ).
            double getLightContrast() const {
                return _lightContrast;
            }

            /// Returns the elevation of the light source, in degrees.
            ///
            /// @returns The elevation of the light source.
            ///
            /// The elevation is the angle above the horizon:
            /// - 0 degrees is on the horizon.
            /// - 90 degrees is straight up.
            double getLightElev() const {
                return _lightElev;
            }

            /// Returns the intensity of the light source.
            ///
            /// @returns The intensity of the light source.
            double getLightIntensity() const {
                return _lightIntensity;
            }

            /// Determines if the light source is enabled.
            ///
            /// @returns
            /// - @a true if the light source is enabled.
            /// - @a false if the light source is disabled.
            bool isLightEnabled() const {
                return _isLightEnabled;
            }

            /// Determines if noise-map wrapping is enabled.
            ///
            /// @returns
            /// - @a true if noise-map wrapping is enabled.
            /// - @a false if noise-map wrapping is disabled.
            ///
            /// This object requires five points (the initial point and its four
            /// neighbors) to calculate light shading.  If wrapping is enabled,
            /// and the initial point is on the edge of the noise map, the
            /// appropriate neighbors that lie outside of the noise map will
            /// "wrap" to the opposite side(s) of the noise map.  Otherwise, the
            /// appropriate neighbors are cropped to the edge of the noise map.
            ///
            /// Enabling wrapping is useful when creating spherical renderings and
            /// tileable textures
            bool isWrapEnabled() const {
                return _isWrapEnabled;
            }

            /// Renders the destination image using the contents of the source
            /// noise map and an optional background image.
            ///
            /// @pre SetSourceNoiseMap() has been previously called.
            /// @pre SetDestImage() has been previously called.
            /// @pre There are at least two gradient points in the color gradient.
            /// @pre No two gradient points have the same position.
            /// @pre If a background image was specified, it has the exact same
            /// size as the source height map.
            ///
            /// @post The original contents of the destination image is destroyed.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            ///
            /// The background image and the destination image can safely refer to
            /// the same image, although in this case, the destination image is
            /// irretrievably blended into the background image.
            void render ();

            /// Sets the destination image.
            ///
            /// @param destImage The destination image.
            ///
            /// The destination image will contain the rendered image after a
            /// successful call to the Render() method.
            ///
            /// The destination image must exist throughout the lifetime of this
            /// object unless another image replaces that image.
            void setDestImage (std::shared_ptr<QImage> destImage) {
                _pDestImage = destImage;
            }

            /// Sets the azimuth of the light source, in degrees.
            ///
            /// @param lightAzimuth The azimuth of the light source.
            ///
            /// The azimuth is the location of the light source around the
            /// horizon:
            /// - 0.0 degrees is east.
            /// - 90.0 degrees is north.
            /// - 180.0 degrees is west.
            /// - 270.0 degrees is south.
            ///
            /// Make sure the light source is enabled via a call to the
            /// EnableLight() method before calling the Render() method.
            void setLightAzimuth (double lightAzimuth) {
                _lightAzimuth = lightAzimuth;
                _recalcLightValues = true;
            }

            /// Sets the brightness of the light source.
            ///
            /// @param lightBrightness The brightness of the light source.
            ///
            /// Make sure the light source is enabled via a call to the
            /// EnableLight() method before calling the Render() method.
            void setLightBrightness (double lightBrightness) {
                _lightBrightness = lightBrightness;
                _recalcLightValues = true;
            }

            /// Sets the color of the light source.
            ///
            /// @param lightColor The light color.
            ///
            /// Make sure the light source is enabled via a call to the
            /// EnableLight() method before calling the Render() method.
            void setLightColor (const QColor& lightColor) {
                _lightColor = lightColor;
            }

            /// Sets the contrast of the light source.
            ///
            /// @param lightContrast The contrast of the light source.
            ///
            /// @pre The specified light contrast is positive.
            ///
            /// @throw noise::ExceptionInvalidParam See the preconditions.
            ///
            /// The contrast specifies how sharp the boundary is between the
            /// light-facing areas and the shadowed areas.
            ///
            /// The contrast determines the difference between areas in light and
            /// areas in shadow.  Determining the correct contrast amount requires
            /// some trial and error, but if your application interprets the noise
            /// map as a height map that has a spatial resolution of @a h meters
            /// and an elevation resolution of 1 meter, a good contrast amount to
            /// use is ( 1.0 / @a h ).
            ///
            /// Make sure the light source is enabled via a call to the
            /// EnableLight() method before calling the Render() method.
            void setLightContrast (double lightContrast) {
                if (lightContrast <= 0.0) {
                    throw noise::ExceptionInvalidParam ();
                }

                _lightContrast = lightContrast;
                _recalcLightValues = true;
            }

            /// Sets the elevation of the light source, in degrees.
            ///
            /// @param lightElev The elevation of the light source.
            ///
            /// The elevation is the angle above the horizon:
            /// - 0 degrees is on the horizon.
            /// - 90 degrees is straight up.
            ///
            /// Make sure the light source is enabled via a call to the
            /// EnableLight() method before calling the Render() method.
            void setLightElev (double lightElev) {
                _lightElev = lightElev;
                _recalcLightValues = true;
            }

            /// Returns the intensity of the light source.
            ///
            /// @returns The intensity of the light source.
            ///
            /// A good value for intensity is 2.0.
            ///
            /// Make sure the light source is enabled via a call to the
            /// EnableLight() method before calling the Render() method.
            void setLightIntensity (double lightIntensity) {
                if (lightIntensity < 0.0) {
                    throw noise::ExceptionInvalidParam ();
                }

                _lightIntensity = lightIntensity;
                _recalcLightValues = true;
            }

            /// Sets the source noise map.
            ///
            /// @param sourceNoiseMap The source noise map.
            ///
            /// The destination image must exist throughout the lifetime of this
            /// object unless another image replaces that image.
            void setSourceNoiseMap (const NoiseMap& sourceNoiseMap) {
                _pSourceNoiseMap = &sourceNoiseMap;
            }

            void setLegend (icosphere::Legend* legend);

        private:

            /// Calculates the destination color.
            ///
            /// @param sourceColor The source color generated from the color
            /// gradient.
            /// @param backgroundColor The color from the background image at the
            /// corresponding position.
            /// @param lightValue The intensity of the light at that position.
            ///
            /// @returns The destination color.
            QColor findDestinationColor (const QColor& sourceColor, double lightValue) const;

            /// Calculates the intensity of the light given some elevation values.
            ///
            /// @param center Elevation of the center point.
            /// @param left Elevation of the point directly left of the center
            /// point.
            /// @param right Elevation of the point directly right of the center
            /// point.
            /// @param down Elevation of the point directly below the center
            /// point.
            /// @param up Elevation of the point directly above the center point.
            ///
            /// These values come directly from the noise map.
            double findLightIntensity (double center, double left, double right, double down, double up) const;

            /// The cosine of the azimuth of the light source.
            mutable double _cosAzimuth;

            /// The cosine of the elevation of the light source.
            mutable double _cosElev;

            /// The color gradient used to specify the image colors.
            QColorGradient* _gradient;

            /// A flag specifying whether lighting is enabled.
            bool _isLightEnabled;

            /// A flag specifying whether wrapping is enabled.
            bool _isWrapEnabled;

            /// The azimuth of the light source, in degrees.
            double _lightAzimuth;

            /// The brightness of the light source.
            double _lightBrightness;

            /// The color of the light source.
            QColor _lightColor;

            /// The contrast between areas in light and areas in shadow.
            double _lightContrast;

            /// The elevation of the light source, in degrees.
            double _lightElev;

            /// The intensity of the light source.
            double _lightIntensity;

            /// A pointer to the destination image.
            std::shared_ptr<QImage> _pDestImage;

            /// A pointer to the source noise map.
            const NoiseMap* _pSourceNoiseMap;

            /// Used by the CalcLightIntensity() method to recalculate the light
            /// values only if the light parameters change.
            ///
            /// When the light parameters change, this value is set to True.  When
            /// the CalcLightIntensity() method is called, this value is set to
            /// false.
            mutable bool _recalcLightValues;

            /// The sine of the azimuth of the light source.
            mutable double _sinAzimuth;

            /// The sine of the elevation of the light source.
            mutable double _sinElev;

            mutable icosphere::Legend* _legend;

            ImageRenderJob* _job;

        };

    }
}
#endif //CALENHAD_RENDERERIMAGE_H
