//
// Created by martin on 11/01/17.
//


#include <osgEarthUtil/Common>
#include "TileProducer.h"
#include "../httpserver/CalenhadRequestHandler.h"
#include "RenderJob.h"

using namespace geoutils;

TileProducer::TileProducer()  {

}

TileProducer::~TileProducer() { }

RenderJob* TileProducer::makeRenderJob (const unsigned& x, const unsigned& y, const unsigned& z) {
    // change the zoom level if requested different from last time
    if (z != _zoom) {
        _zoom = z > MAX_ZOOM ? MAX_ZOOM : z;
        _width_rads = (TWO_PI) / powers (z);
        _height_rads = PI / z == 0 ? 1 : powers (z - 1);
    }

    Geolocation nw = getNWCorner (x, y);
    Geolocation se = getSECorner (x, y);
    GeoQuad bounds = GeoQuad (nw, se);
    RenderJob* job = new RenderJob (bounds, _module);
    return job;
}

bool TileProducer::fetchTile (std::shared_ptr<QImage> tile, const unsigned& x, const unsigned& y, const unsigned& z) {
    RenderJob* job = makeRenderJob (x, y, z);
    job -> setImage (tile);
    job -> render();
    delete job;
    return tile != nullptr;
}

Geolocation TileProducer::getNWCorner (const unsigned& x, const unsigned& y) const {
    if (_zoom >= 2) {
        double latitude = (y % _ycount) * (y - powers (_zoom - 1)) * TWO_PI;
        while (latitude > PI) { latitude -= PI; }
        while (latitude < -PI)  { latitude += PI; }
        double longitude = (x % _xcount) * (x - powers (_zoom - 2)) * PI;
        while (longitude > HALF_PI) { longitude -= HALF_PI; }
        while (longitude < HALF_PI) { longitude += HALF_PI; }
        return Geolocation (latitude, longitude, Geolocation::RADS);
    } else {
        // z == 1; x in { 0, 1 }
        return Geolocation (((x == 0) ? -PI : 0), PI / 2, Geolocation::RADS);
    }
}

Geolocation TileProducer::getSECorner (const unsigned& x, const unsigned& y) const {
    if (_zoom >= 2) {
        return getNWCorner (x + 1, y + 1);
    } else {
        // z == 1; x in { 0, 1 }
        return Geolocation (((x == 0) ? 0 : PI), -HALF_PI, Geolocation::RADS);
    }
}

void TileProducer::setModule (noise::module::Module* module) {
    _module = module;
}

void TileProducer::setImageSize (int size) {
    _size = size;
}

// return 2 to the power of the given index
int TileProducer::powers (int index) {
    if (index < 0) { return 0; }
    int result = 1;
    result <<= (index - 1);
    return result;
}
