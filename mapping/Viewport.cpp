//
// Created by martin on 06/09/17.
//

#include "Viewport.h"
#include "projection/Projection.h"
#include "projection/ProjectionService.h"
#include "../CalenhadServices.h"
#include "../messages/QNotificationHost.h"


using namespace geoutils;
using namespace calenhad::mapping;
using namespace calenhad::mapping::projection;
using namespace icosphere;
using namespace calenhad::notification;


Viewport::Viewport () : _projection (CalenhadServices::projections() -> fetch ("Equirectangular")), _size (QSize()), _datum (Geolocation (0.0, 0.0)), _zoom (1.0)  {

}

Viewport::~Viewport () {

}

void Viewport::setSize (const QSize& size) {
    _size = size;
}

QSize Viewport::size() {
    return _size;
}

void Viewport::setProjection (const QString& projection) {
    _projection = CalenhadServices::projections () -> fetch (projection);
    if (! _projection) {
        _projection = CalenhadServices::projections() -> fetch ("Equirectangular");
        CalenhadServices::messages() -> message ("Projection not found", "Couldn't find projection " + projection, "Using Equirectangular projection instead", NotificationStyle::WarningNotification);
    }
    emit changed();
}

Projection* Viewport::projection() {
    return _projection;
}

void Viewport::setDatum (const geoutils::Geolocation& datum) {
    _datum = datum;
    emit changed();
}

Geolocation Viewport::datum() {
    return _datum;
}

void Viewport::setZoom (const double& zoom) {
    _zoom = zoom;
    emit changed();
}

double Viewport::zoom() {
    return _zoom;
}

QPointF Viewport::flatMapCoordinates (const QPoint& screenCoordinates) {
    double x = ((screenCoordinates.x() / _size.width()) - 0.5) * 2 * _projection -> range().x();
    double y = ((screenCoordinates.y() / _size.height()) - 0.5) * 2 * _projection -> range().y();
    return QPointF (x, -y);
}

icosphere::Bounds calenhad::mapping::Viewport::bounds () {
    return Bounds();

}

bool calenhad::mapping::Viewport::screenCoordinates (geoutils::Geolocation geolocation, QPointF& screenCoordinates) {
    return false;
}

bool Viewport::geoCoordinates (const QPoint& pos, geoutils::Geolocation& geolocation) {
    return _projection -> inverse (flatMapCoordinates (pos), geolocation);
}
