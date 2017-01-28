//
// Created by martin on 26/01/17.
//

#include "CalenhadLayer.h"
#include <marble/GeoDataLatLonAltBox.h>
#include <marble/GeoPainter.h>
#include <marble/ViewportParams.h>
#include "../geoutils.h"
#include "../libnoiseutils/NoiseContstants.h"
#include "../pipeline/QColorGradient.h"
#include <noise/module/module.h>


using namespace geoutils;
using namespace Marble;
using namespace noise::utils;

CalenhadLayer::CalenhadLayer (noise::module::Module* source) : _source (source), _gradient (new QColorGradient()) {

}

CalenhadLayer::~CalenhadLayer() {
    if (_gradient) { delete _gradient; }
}

void CalenhadLayer::setGradient (QColorGradient* gradient) {
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

    double noise;
    Geolocation geolocation;
    Cartesian cartesian;
    Color color;
    QColor qc;
    int r, g, b;

    // see if circumpolar boxes work - if not set north to M_PI / 2 if north pole included, south to -M_PI / 2 if south pole included
    for (double lat = box.west(); lat <= box.east(); lat += _angularResolution) {
        for (double lon = box.south (); lon <= box.north (); lon += _angularResolution) {
            if (_viewport->screenCoordinates (lat, lon, x, y)) {
                geolocation = Geolocation (lat, lon, Geolocation::RADS);
                cartesian = Math::toCartesian (geolocation);
                noise = _source -> GetValue (cartesian.x, cartesian.y, cartesian.z);
                color = _gradient -> GetColor (noise);
                // to do - gradient colour mapping and normal
                qc = QColor (color.red, color.green, color.blue);
                painter -> setPen (qc);
                painter -> drawPoint ((GeoDataCoordinates ((qreal) geolocation.latitude, (qreal) geolocation.longitude)));
            }
        }
    }
}


bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    GeoDataLatLonAltBox box = viewport -> viewLatLonAltBox();
    _angularResolution = viewport -> angularResolution();
    _viewport = viewport;
    return render (painter, box);
}
