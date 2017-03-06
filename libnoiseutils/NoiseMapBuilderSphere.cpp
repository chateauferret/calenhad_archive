//
// Created by martin on 27/01/17.
//

#include <iostream>
#include <QtCore/QThread>
#include "NoiseMapBuilderSphere.h"
#include "libnoise/model/sphere.h"
#include "../pipeline/RenderJob.h"

using namespace noise::utils;

#include "NoiseContstants.h"
#include "NoiseMap.h"

/////////////////////////////////////////////////////////////////////////////
// NoiseMapBuilderSphere class

NoiseMapBuilderSphere::NoiseMapBuilderSphere (RenderJob* job) :
        m_eastLonBound (0.0),
        m_northLatBound (0.0),
        m_southLatBound (0.0),
        m_westLonBound (0.0),
        _job (job) {
}

void NoiseMapBuilderSphere::build () {

    prepare ();


    // Create the sphere model.
    noise::model::Sphere sphereModel;
    sphereModel.SetModule (*_source);

    // Fill every point in the noise map with the output values from the model.
    for (int y = 0; y < _destHeight; y++) {
        float* pDest = _destNoiseMap -> GetSlabPtr (y);
        _curLon = m_westLonBound;
        for (int x = 0; x < _destWidth; x++) {
            // if thread is being interrupted, terminate the loops gracefully
            if (_job -> isAbandoned()) {
                x = _destWidth;
                y = _destHeight;
            } else {
                float curValue = (float) sphereModel.GetValue (_curLat, _curLon);
                *pDest++ = curValue;
                _curLon += _xDelta;
                if (_curLon > 180) {
                    _curLon -= 360;
                }
            }
        }
        _curLat += _yDelta;
        if (m_pCallback != NULL) {
            m_pCallback (y);
        }
    }
}

void NoiseMapBuilderSphere::prepare () {
    if (
          m_northLatBound <= m_southLatBound
         || _destWidth <= 0
         || _destHeight <= 0
         || _source == NULL
         || _destNoiseMap == NULL) {
       // throw noise::ExceptionInvalidParam ();
    }

    // Resize the destination noise map so that it can store the new output
    // values from the source model.
    _destNoiseMap -> SetSize (_destWidth, _destHeight);

    double lonExtent = m_eastLonBound > m_westLonBound ? m_eastLonBound - m_westLonBound : 360 - ( m_westLonBound - m_eastLonBound );
    double latExtent = m_northLatBound - m_southLatBound;
    _xDelta = lonExtent / (double) _destWidth;
    _yDelta = latExtent / (double) _destHeight;
    _curLon = m_westLonBound;
    _curLat = m_southLatBound;

}