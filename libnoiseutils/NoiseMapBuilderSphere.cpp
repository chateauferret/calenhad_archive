//
// Created by martin on 27/01/17.
//

#include <iostream>
#include <QtCore/QThread>
#include "NoiseMapBuilderSphere.h"
#include "libnoise/model/sphere.h"
#include "../pipeline/ImageRenderJob.h"

using namespace noise::utils;
using namespace calenhad::pipeline;
using namespace Marble;

#include "NoiseConstants.h"
#include "NoiseMap.h"

/////////////////////////////////////////////////////////////////////////////
// NoiseMapBuilderSphere class

NoiseMapBuilderSphere::NoiseMapBuilderSphere (ImageRenderJob* job) :
        _job (job) {

}

void NoiseMapBuilderSphere::build () {
    if (_source) {
        prepare ();

        // Create the sphere model.
        noise::model::Sphere sphereModel;
        sphereModel.SetModule (*_source);
        _curLat = RAD2DEG * (_bounds.south());
        // Fill every point in the noise map with the output values from the model.
        for (int y = 0; y < _destHeight; y++) {
            float* pDest = _destNoiseMap -> GetSlabPtr (y);
            _curLon = RAD2DEG * (_bounds.west());

            for (int x = 0; x < _destWidth; x++) {
                //if thread is being interrupted, terminate the loops gracefully
                if (_job -> isAbandoned ()) {
                    x = _destWidth;
                    y = _destHeight;
                } else {

                    float curValue = (float) sphereModel.GetValue (_curLat, _curLon);
                    *pDest++ = curValue;
                    _curLon += _xDelta;
                    if (_curLon >= 180) {
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
}

void NoiseMapBuilderSphere::prepare () {

    // Resize the destination noise map so that it can store the new output
    // values from the source model.
    _destNoiseMap -> SetSize (_destWidth, _destHeight);

    _xDelta = RAD2DEG * _bounds.width() / (double) _destWidth;
    _yDelta = RAD2DEG * _bounds.height() / (double) _destHeight;

    // GeoDataLatLonBox thinks 180 degrees West is equal to 180 degrees East; so we need to remember that the actual
    // longitudinal extent around the whole globe starting and ending on this meridian is 180 degrees (2 * M_PI radians), not 0.
    if (_xDelta == 0) { _xDelta = 360 / (double) _destWidth; }


}