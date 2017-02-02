//
// Created by martin on 26/01/17.
//

#include "CalenhadLayer.h"
#include <marble/GeoDataLatLonAltBox.h>
#include <marble/GeoPainter.h>
#include <marble/ViewportParams.h>
#include "../geoutils.h"
#include "../libnoiseutils/NoiseContstants.h"
#include <noise/module/module.h>
#include "../libnoiseutils/GradientLegend.h"
#include <libnoise/model/sphere.h>
#include <ctime>

using namespace geoutils;
using namespace Marble;
using namespace noise::utils;
using namespace noise::model;

CalenhadLayer::CalenhadLayer (noise::module::Module* source) : _source (source), _gradient (new GradientLegend()), _sphere (new Sphere (*source)) {

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

bool CalenhadLayer::render (GeoPainter* painter, GeoDataLatLonBox box) {
    double x, y;
    if (box.west() > box.east()) {
        return render (painter, GeoDataLatLonBox (box.north(), box.south(), box.west(), (qreal) M_PI)) &&
               render (painter, GeoDataLatLonBox (box.north(), box.south(), (qreal) -M_PI, box.east()));
    }

    double noise, latd, lond;

    QColor color;
    // see if circumpolar boxes work - if not set north to M_PI / 2 if north pole included, south to -M_PI / 2 if south pole included
    // to do - put this in a separate thread

    std::time_t start = std::clock();
    for (double lon = box.west(); lon <= box.east(); lon += _angularResolution) {
        for (double lat = box.south (); lat <= box.north (); lat += _angularResolution) {
            if (_viewport -> screenCoordinates (lon, lat, x, y)) {
                latd = lat * -180 / M_PI;
                lond = lon * 180 / M_PI;
                noise = _sphere -> GetValue (latd, lond);
                color = _gradient -> lookup (noise);
                // to do - _gradient colour mapping and normal

                painter -> setPen (color);
                painter -> drawPoint (GeoDataCoordinates ((qreal) lon, (qreal) lat));
            }
        }
    }
    std::time_t end = std::clock();
    std::cout << "Rendered in " << (end - start) / (double) (CLOCKS_PER_SEC / 1000) << " ms\n";
}


bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    GeoDataLatLonAltBox box = viewport -> viewLatLonAltBox();
    _angularResolution = viewport -> angularResolution() / 4;
    _viewport = viewport;
      return render (painter, box);
}
