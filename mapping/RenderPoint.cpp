//
// Created by martin on 18/07/17.
//

#include "RenderPoint.h"

using namespace Marble;
using namespace calenhad::mapping;

RenderPoint::RenderPoint (const QPoint& p, Marble::ViewportParams* viewport) : _point (p), _value (0.0), _color (Qt::black), _viewport (viewport), _ready (false) {
    double lat, lon;
    _valid = (viewport -> geoCoordinates (p.x(), p.y(), lon, lat, GeoDataCoordinates::Radian));
    if (_valid) {
        _latRadians = lat;
        _lonRadians = lon;

        // precompute the geolocation in degrees so that libnoise Sphere modeul can use it without doing sums first
        _latDegrees = lat * RAD2DEG;
        _lonDegrees = lon * RAD2DEG;
    }
}

const QPoint& RenderPoint::point() {
    return _point;
}

void RenderPoint::populate (const double& value, const QColor& color) {
    _value = value;
    _color = color;
    _ready = true;

}

const QColor& RenderPoint::getColor () const {
    return _color;
}

double RenderPoint::getValue () const {
    return _value;
}

bool RenderPoint::isValid () const {
    return _valid;
}

bool RenderPoint::isReady () const {
    return _ready;
}
