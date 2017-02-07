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

NoiseMapBuilderSphere::NoiseMapBuilderSphere () :
        m_eastLonBound (0.0),
        m_northLatBound (0.0),
        m_southLatBound (0.0),
        m_westLonBound (0.0) {
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
            float curValue = (float) sphereModel.GetValue (_curLat, _curLon);
            *pDest++ = curValue;
            _curLon += _xDelta;
        }
        _curLat += _yDelta;
        if (m_pCallback != NULL) {
            m_pCallback (y);
        }
    }
}

void NoiseMapBuilderSphere::prepare () {
    if (m_eastLonBound <= m_westLonBound
        || m_northLatBound <= m_southLatBound
        || _destWidth <= 0
        || _destHeight <= 0
        || _source == NULL
        || _destNoiseMap == NULL) {
        throw noise::ExceptionInvalidParam ();
    }

    // Resize the destination noise map so that it can store the new output
    // values from the source model.
    _destNoiseMap -> SetSize (_destWidth, _destHeight);

    double lonExtent = m_eastLonBound - m_westLonBound;
    double latExtent = m_northLatBound - m_southLatBound;
    _xDelta = lonExtent / (double) _destWidth;
    _yDelta = latExtent / (double) _destHeight;
    _curLon = m_westLonBound;
    _curLat = m_southLatBound;

}