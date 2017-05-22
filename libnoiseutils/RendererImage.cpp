//
// Created by martin on 27/01/17.
//

#include <noise/mathconsts.h>
#include <libnoise/misc.h>
#include <cmath>
#include <QtGui/QColor>
#include "RendererImage.h"
#include "NoiseMap.h"
#include "Interpolation.h"
#include <QThread>
#include "../pipeline/ImageRenderJob.h"

using namespace noise::utils;
using namespace icosphere;

//////////////////////////////////////////////////////////////////////////////
// RendererImage class

RendererImage::RendererImage (ImageRenderJob* job):
        _isLightEnabled    (false),
        _isWrapEnabled     (false),
        _lightAzimuth      (45.0),
        _lightBrightness   (1.0),
        _lightColor        (255, 255, 255, 255),
        _lightContrast     (1.0),
        _lightElev         (45.0),
        _lightIntensity    (1.0),
        _pDestImage        (NULL),
        _pSourceNoiseMap   (NULL),
        _recalcLightValues (true) ,
        _legend (nullptr),
        _job (job) {
}

QColor RendererImage::findDestinationColor (const QColor& sourceColor, double lightValue) const {
    double red   = (double) sourceColor.red()   / 255.0;
    double green = (double) sourceColor.green() / 255.0;
    double blue  = (double) sourceColor.blue()  / 255.0;
    double alpha = (double) sourceColor.alpha() / 255.0;

    if (_isLightEnabled) {

        // Now calculate the light color.
        double lightRed   = lightValue * (_lightColor.red()   / 255.0);
        double lightGreen = lightValue * (_lightColor.green() / 255.0);
        double lightBlue  = lightValue * (_lightColor.blue()  / 255.0);

        // Apply the light color to the new color.
        red   *= lightRed  ;
        green *= lightGreen;
        blue  *= lightBlue ;
    }

    // Clamp the color channels to the (0..1) range.
    red   = (red   < 0.0)? 0.0: red  ;
    red   = (red   > 1.0)? 1.0: red  ;
    green = (green < 0.0)? 0.0: green;
    green = (green > 1.0)? 1.0: green;
    blue  = (blue  < 0.0)? 0.0: blue ;
    blue  = (blue  > 1.0)? 1.0: blue ;

    // Rescale the color channels to the noise::uint8 (0..255) range and return the new color.
    QColor newColor (
            (noise::uint8) ((noise::uint) (red   * 255.0) & 0xff),
            (noise::uint8) ((noise::uint) (green * 255.0) & 0xff),
            (noise::uint8) ((noise::uint) (blue  * 255.0) & 0xff),
            (noise::uint8) ((noise::uint) (alpha * 255.0) & 0xff));
    return newColor;
}

double RendererImage::findLightIntensity (double center, double left, double right, double down, double up) const {
    // Recalculate the sine and cosine of the various light values if
    // necessary so it does not have to be calculated each time this method is
    // called.
    if (_recalcLightValues) {
        _cosAzimuth = cos (_lightAzimuth * DEG_TO_RAD);
        _sinAzimuth = sin (_lightAzimuth * DEG_TO_RAD);
        _cosElev    = cos (_lightElev    * DEG_TO_RAD);
        _sinElev    = sin (_lightElev    * DEG_TO_RAD);
        _recalcLightValues = false;
    }

    // Now do the lighting calculations.
    const double I_MAX = 1.0;
    double io = I_MAX * SQRT_2 * _sinElev / 2.0;
    double ix = (I_MAX - io) * _lightContrast * SQRT_2 * _cosElev * _cosAzimuth;
    double iy = (I_MAX - io) * _lightContrast * SQRT_2 * _cosElev * _sinAzimuth;
    double intensity = (ix * (left - right) + iy * (down - up) + io);
    if (intensity < 0.0) {
        intensity = 0.0;
    }
    return intensity;
}

void RendererImage::render () {
    if ( _pSourceNoiseMap == nullptr
         || _pDestImage == nullptr
         || _pSourceNoiseMap -> GetWidth  () <= 0
         || _pSourceNoiseMap -> GetHeight () <= 0
         || ! (_legend -> isValid())) {
        throw noise::ExceptionInvalidParam ();
    }

    int width  = _pSourceNoiseMap -> GetWidth  ();
    int height = _pSourceNoiseMap -> GetHeight ();

    for (int y = 0; y < height; y++) {
        const float* pSource = _pSourceNoiseMap -> GetConstSlabPtr (y);
        for (int x = 0; x < width; x++) {

            // if thread is being interrupted, terminate the loops gracefully
            if (_job -> isAbandoned ()) {
                x = width;
                y = height;
            } else {

                // Get the color based on the value at the current point in the noise map.
                double value = _pSourceNoiseMap->GetValue (x, y);
                QColor destColor = _legend->lookup (value);

                // If lighting is enabled, calculate the light intensity based on the
                // rate of change at the current point in the noise map.
                double lightIntensity;
                if (_isLightEnabled) {

                    // Calculate the positions of the current point's four-neighbors.
                    int xLeftOffset, xRightOffset;
                    int yUpOffset, yDownOffset;
                    if (_isWrapEnabled) {
                        if (x == 0) {
                            xLeftOffset = width - 1;
                            xRightOffset = 1;
                        } else if (x == width - 1) {
                            xLeftOffset = -1;
                            xRightOffset = -(width - 1);
                        } else {
                            xLeftOffset = -1;
                            xRightOffset = 1;
                        }
                        if (y == 0) {
                            yDownOffset = height - 1;
                            yUpOffset = 1;
                        } else if (y == height - 1) {
                            yDownOffset = -1;
                            yUpOffset = -(height - 1);
                        } else {
                            yDownOffset = -1;
                            yUpOffset = 1;
                        }
                    } else {
                        if (x == 0) {
                            xLeftOffset = 0;
                            xRightOffset = 1;
                        } else if (x == width - 1) {
                            xLeftOffset = -1;
                            xRightOffset = 0;
                        } else {
                            xLeftOffset = -1;
                            xRightOffset = 1;
                        }
                        if (y == 0) {
                            yDownOffset = 0;
                            yUpOffset = 1;
                        } else if (y == height - 1) {
                            yDownOffset = -1;
                            yUpOffset = 0;
                        } else {
                            yDownOffset = -1;
                            yUpOffset = 1;
                        }
                    }
                    yDownOffset *= _pSourceNoiseMap->GetStride ();
                    yUpOffset *= _pSourceNoiseMap->GetStride ();

                    // Get the noise value of the current point in the source noise map
                    // and the noise values of its four-neighbors.
                    double nc = (double) (*pSource);
                    double nl = (double) (*(pSource + xLeftOffset));
                    double nr = (double) (*(pSource + xRightOffset));
                    double nd = (double) (*(pSource + yDownOffset));
                    double nu = (double) (*(pSource + yUpOffset));

                    // Now we can calculate the lighting intensity.
                    lightIntensity = findLightIntensity (nc, nl, nr, nd, nu);
                    lightIntensity *= _lightBrightness;

                } else {

                    // These values will apply no lighting to the destination pixmap.
                    lightIntensity = 1.0;
                }

                // Blend the destination color and the light
                // intensity together, then invalidate the destination pixmap with that color.
                QColor c = findDestinationColor (destColor, lightIntensity);
                _pDestImage->setPixelColor (x, y, c);
                // Go to the next point.
                ++pSource;
            }
        }
    }
}

void RendererImage::setLegend (Legend* legend) {
    _legend = legend;
}
