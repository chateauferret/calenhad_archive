//
// Created by martin on 04/09/17.
//

#include <boost/concept_check.hpp>
#include "OrthographicProjection.h"

using namespace calenhad::mapping::projection;
using namespace geoutils;

OrthographicProjection::OrthographicProjection () : Projection(), _datum (Geolocation (0.0, 0.0)) {

}

OrthographicProjection::~OrthographicProjection () {

}

void OrthographicProjection::setDatum (const Geolocation& datum) {
    _datum.setLatitude (datum.latitude());
    _datum.setLongitude (datum.longitude());
};

bool OrthographicProjection::inverse (const QPointF& point, Geolocation& geolocation) {
    double x = point.x() / 2;
    double y = point.y();
    double rho = std::sqrt (x * x + y * y);
    double c = std::asin (rho);
    double lat = asin (cos (c) * sin (_datum.latitude()) + (((y * sin (c)) * cos (_datum.latitude()) / rho)));
    double lon = _datum.longitude() + std::atan2 ((x * sin (c)), (rho * cos (_datum.latitude()) * cos (c) - y * sin (_datum.latitude()) * sin (c)));
    geolocation.setLatitude (lat);
    geolocation.setLongitude (lon);
    return (isVisible (geolocation));
}

bool OrthographicProjection::isVisible (const Geolocation& geolocation) {
    return sin (geolocation.latitude()) * sin (_datum.latitude()) + cos (geolocation.latitude()) * cos (_datum.latitude()) * cos (geolocation.longitude() - _datum.longitude()) >= 0;
}

bool OrthographicProjection::forward (const geoutils::Geolocation& geolocation, QPointF& point) {
    point.setX ((geolocation.longitude() - _datum.longitude()) * cos (_datum.latitude()));
    point.setY (geolocation.latitude() - _datum.latitude());
    bool valid = isVisible (geolocation);
    return valid;
}

QString OrthographicProjection::name () {
    return "Mercator projection";
}

QString OrthographicProjection::notes () {
    return "";
}

int OrthographicProjection::id () {
    return 2; // see map_cs.glsl
}

QString OrthographicProjection::glsl() {
    QString code = "if (projection == PROJ_ORTHOGRAPHIC) {\n";
    code += "    float x = i.x / 2;\n";
    code += "    float y = i.y;\n";
    code += "    float w = sqrt (x * x + y * y);\n";
    code += "    float z = sqrt (1.0 - x * x - y * y);\n";
    code += "    float x1 = cos (datum.x) * x + sin (datum.x) * sin (datum.y) * y + sin (datum.x) * cos (datum.y) * z;\n";
    code += "    float y1 = cos (datum.y) * y - sin (datum.y) * z;\n";
    code += "    float z1 = - sin (datum.x) * x + cos (datum.x) * sin (datum.y) * y + cos (datum.x) * cos (datum.y) * z;\n";
    code += "    return vec4 (z1, x1, y1, w);\n";
    code += "}\n";
    return code;
}

QPointF OrthographicProjection::range () {
    return QPointF (M_PI, M_PI);
}
//
// Created by martin on 04/09/17.
//

#include "OrthographicProjection.h"
