//
// Created by martin on 27/01/17.
//

#include <cmath>
#include <libnoise/exception.h>
#include "RendererNormalMap.h"
#include "Color.h"
#include "NoiseMap.h"
#include "Image.h"

using namespace noise::utils;

//////////////////////////////////////////////////////////////////////////////
// RendererNormalMap class

RendererNormalMap::RendererNormalMap ():
        m_bumpHeight      (1.0),
        m_isWrapEnabled   (false),
        m_pDestImage      (NULL),
        m_pSourceNoiseMap (NULL)
{
}

Color RendererNormalMap::CalcNormalColor (double nc, double nr, double nu,
                                          double bumpHeight) const
{
    // Calculate the surface normal.
    nc *= bumpHeight;
    nr *= bumpHeight;
    nu *= bumpHeight;
    double ncr = (nc - nr);
    double ncu = (nc - nu);
    double d = sqrt ((ncu * ncu) + (ncr * ncr) + 1);
    double vxc = (nc - nr) / d;
    double vyc = (nc - nu) / d;
    double vzc = 1.0 / d;

    // Map the normal range from the (-1.0 .. +1.0) range to the (0 .. 255)
    // range.
    noise::uint8 xc, yc, zc;
    xc = (noise::uint8)((noise::uint)((floor)((vxc + 1.0) * 127.5)) & 0xff);
    yc = (noise::uint8)((noise::uint)((floor)((vyc + 1.0) * 127.5)) & 0xff);
    zc = (noise::uint8)((noise::uint)((floor)((vzc + 1.0) * 127.5)) & 0xff);

    return Color (xc, yc, zc, 0);
}

void RendererNormalMap::Render ()
{
    if ( m_pSourceNoiseMap == NULL
         || m_pDestImage == NULL
         || m_pSourceNoiseMap->GetWidth  () <= 0
         || m_pSourceNoiseMap->GetHeight () <= 0) {
        throw noise::ExceptionInvalidParam ();
    }

    int width  = m_pSourceNoiseMap->GetWidth  ();
    int height = m_pSourceNoiseMap->GetHeight ();

    for (int y = 0; y < height; y++) {
        const float* pSource = m_pSourceNoiseMap->GetConstSlabPtr (y);
        Color* pDest = m_pDestImage->GetSlabPtr (y);
        for (int x = 0; x < width; x++) {

            // Calculate the positions of the current point's right and up
            // neighbors.
            int xRightOffset, yUpOffset;
            if (m_isWrapEnabled) {
                if (x == (int)width - 1) {
                    xRightOffset = -((int)width - 1);
                } else {
                    xRightOffset = 1;
                }
                if (y == (int)height - 1) {
                    yUpOffset = -((int)height - 1);
                } else {
                    yUpOffset = 1;
                }
            } else {
                if (x == (int)width - 1) {
                    xRightOffset = 0;
                } else {
                    xRightOffset = 1;
                }
                if (y == (int)height - 1) {
                    yUpOffset = 0;
                } else {
                    yUpOffset = 1;
                }
            }
            yUpOffset *= m_pSourceNoiseMap->GetStride ();

            // Get the noise value of the current point in the source noise map
            // and the noise values of its right and up neighbors.
            double nc = (double)(*pSource);
            double nr = (double)(*(pSource + xRightOffset));
            double nu = (double)(*(pSource + yUpOffset   ));

            // Calculate the normal product.
            *pDest = CalcNormalColor (nc, nr, nu, m_bumpHeight);

            // Go to the next point.
            ++pSource;
            ++pDest;
        }
    }
}
