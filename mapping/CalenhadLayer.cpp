//
// Created by martin on 26/01/17.
//

#include "CalenhadLayer.h"
#include <marble/GeoPainter.h>
#include <marble/ViewportParams.h>
#include "../libnoiseutils/NoiseContstants.h"
#include "../libnoiseutils/GradientLegend.h"
#include <libnoise/model/sphere.h>
#include <ctime>
#include <QThread>

using namespace geoutils;
using namespace Marble;
using namespace noise::utils;
using namespace noise::model;

CalenhadLayer::CalenhadLayer (QModule* source) : _source (source), _gradient (new GradientLegend()), _sphere (new Sphere (*source -> module ())), _step (INITIAL_STEP) {

}

CalenhadLayer::~CalenhadLayer() {
    if (_gradient) { delete _gradient; }
}

void CalenhadLayer::setGradient (GradientLegend* gradient) {
    _gradient = gradient;
}

QStringList CalenhadLayer::renderPosition() const {
    QStringList list;
    list << QString ("SURFACE");
    return list;
}

int CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport) {
       return render (painter, viewport, 0);
   }


int CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const int& offset) {
    GeoDataLatLonAltBox box = viewport -> viewLatLonAltBox();
    QColor color;
    std::time_t start = std::clock();
        double lat;
        double lon = box.west();
        bool finished = false;
        while (! finished) {
            lon += _angularResolution * _step;
            if (box.east() > box.west()) {
                if (lon > box.east()) {
                    finished = true;
                }
            } else {
                if (lon > box.east() && lon < box.west()) {
                    finished = true;
                }
            }
            if (lon > M_PI) {
                lon -= 2 * M_PI;
            }
            lat = box.south();
            while (lat < box.north()) {
                lat += _angularResolution * _step;
                double value = _sphere -> GetValue (radiansToDegrees (lat), radiansToDegrees (lon));
                color = _gradient -> lookup (value);
                painter -> setPen (color);
                painter -> drawPoint (GeoDataCoordinates ((qreal) lon, (qreal) lat));
            }
        }
    if (_step > 1) {
        _step /= 2;
    }
    emit imageRefreshed();
    return 0;
}


bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    _angularResolution = viewport -> angularResolution() / 4;
    return render (painter, viewport) == 0;
}

void CalenhadLayer::refresh (const GeoDataLatLonAltBox& box) {
    _step = INITIAL_STEP;
}

void CalenhadLayer::rescale (const QSize&) {
    _step = INITIAL_STEP;
}