//
// Created by martin on 27/01/17.
//

#include "NoiseMapBuilderSphere.h"
#include "libnoise/model/sphere.h"
using namespace noise::utils;
#include "NoiseContstants.h"
#include "NoiseMap.h"

/////////////////////////////////////////////////////////////////////////////
// NoiseMapBuilderSphere class

NoiseMapBuilderSphere::NoiseMapBuilderSphere ():
        m_eastLonBound  (0.0),
        m_northLatBound (0.0),
        m_southLatBound (0.0),
        m_westLonBound  (0.0)
{
}

void NoiseMapBuilderSphere::Build ()
{
    if ( m_eastLonBound <= m_westLonBound
         || m_northLatBound <= m_southLatBound
         || m_destWidth <= 0
         || m_destHeight <= 0
         || m_pSourceModule == NULL
         || m_pDestNoiseMap == NULL) {
        throw noise::ExceptionInvalidParam ();
    }

    // Resize the destination noise map so that it can store the new output
    // values from the source model.
    m_pDestNoiseMap->SetSize (m_destWidth, m_destHeight);

    // Create the plane model.
    noise::model::Sphere sphereModel;
    sphereModel.SetModule (*m_pSourceModule);

    double lonExtent = m_eastLonBound  - m_westLonBound ;
    double latExtent = m_northLatBound - m_southLatBound;
    double xDelta = lonExtent / (double)m_destWidth ;
    double yDelta = latExtent / (double)m_destHeight;
    double curLon = m_westLonBound ;
    double curLat = m_southLatBound;

    // Fill every point in the noise map with the output values from the model.
    for (int y = 0; y < m_destHeight; y++) {
        float* pDest = m_pDestNoiseMap->GetSlabPtr (y);
        curLon = m_westLonBound;
        for (int x = 0; x < m_destWidth; x++) {
            float curValue = (float)sphereModel.GetValue (curLat, curLon);
            *pDest++ = curValue;
            curLon += xDelta;
        }
        curLat += yDelta;
        if (m_pCallback != NULL) {
            m_pCallback (y);
        }
    }
}
