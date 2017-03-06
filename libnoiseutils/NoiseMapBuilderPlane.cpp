//
// Created by martin on 27/01/17.
//

#include <QtCore/QThread>
#include "NoiseMapBuilderPlane.h"
#include "NoiseMap.h"
#include "libnoise/model/plane.h"
#include "Interpolation.h"


using namespace noise::utils;


/////////////////////////////////////////////////////////////////////////////
// NoiseMapBuilderPlane class

NoiseMapBuilderPlane::NoiseMapBuilderPlane ():
        m_isSeamlessEnabled (false),
        m_lowerXBound  (0.0),
        m_lowerZBound  (0.0),
        m_upperXBound  (0.0),
        m_upperZBound  (0.0)
{
}

void NoiseMapBuilderPlane::Build ()
{
    if ( m_upperXBound <= m_lowerXBound
         || m_upperZBound <= m_lowerZBound
         || _destWidth <= 0
         || _destHeight <= 0
         || _source == NULL
         || _destNoiseMap == NULL) {
        throw noise::ExceptionInvalidParam ();
    }

    // Resize the destination noise map so that it can store the new output
    // values from the source model.
    _destNoiseMap->SetSize (_destWidth, _destHeight);

    // Create the plane model.
    noise::model::Plane planeModel;
    planeModel.SetModule (*_source);

    double xExtent = m_upperXBound - m_lowerXBound;
    double zExtent = m_upperZBound - m_lowerZBound;
    double xDelta  = xExtent / (double)_destWidth ;
    double zDelta  = zExtent / (double)_destHeight;
    double xCur    = m_lowerXBound;
    double zCur    = m_lowerZBound;

    // Fill every point in the noise map with the output values from the model.
    for (int z = 0; z < _destHeight; z++) {
        float* pDest = _destNoiseMap->GetSlabPtr (z);
        xCur = m_lowerXBound;
        for (int x = 0; x < _destWidth; x++) {
            if (QThread::currentThread ()->isInterruptionRequested ()) {
                x = _destWidth;
                z = _destHeight;
            } else {
                float finalValue;
                if (!m_isSeamlessEnabled) {
                    finalValue = (float) planeModel.GetValue (xCur, zCur);
                } else {
                    double swValue, seValue, nwValue, neValue;
                    swValue = planeModel.GetValue (xCur, zCur);
                    seValue = planeModel.GetValue (xCur + xExtent, zCur);
                    nwValue = planeModel.GetValue (xCur, zCur + zExtent);
                    neValue = planeModel.GetValue (xCur + xExtent, zCur + zExtent);
                    double xBlend = 1.0 - ((xCur - m_lowerXBound) / xExtent);
                    double zBlend = 1.0 - ((zCur - m_lowerZBound) / zExtent);
                    double z0 = LinearInterp (swValue, seValue, xBlend);
                    double z1 = LinearInterp (nwValue, neValue, xBlend);
                    finalValue = (float) LinearInterp (z0, z1, zBlend);
                }
                *pDest++ = finalValue;
                xCur += xDelta;
            }
            zCur += zDelta;
            if (m_pCallback != NULL) {
                m_pCallback (z);
            }
        }
    }
}