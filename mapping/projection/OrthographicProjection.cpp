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
    double x = point.x();
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
    double x = std::cos (geolocation.latitude()) * std::sin (geolocation.longitude() - _datum.longitude());
    double y = std::cos (_datum.latitude()) * std::sin (geolocation.latitude ()) - std::sin (_datum.latitude() * std::cos (geolocation.latitude()) * std::cos (geolocation.longitude() - _datum.longitude()));
    point.setX (x);
    point.setY (y);
    bool valid = isVisible (geolocation);
    return valid;
}

QString OrthographicProjection::name () {
    return "Orthographic";
}

QString OrthographicProjection::notes () {
    return "";
}

int OrthographicProjection::id () {
    return 2; // see map_cs.glslInverse
}


QString OrthographicProjection::glslInverse() {
    QString code = "if (p == PROJ_ORTHOGRAPHIC) {\n";
    code += "   float x = i.x;\n";
    code += "   float y = i.y;\n";
    code += "   float rho = sqrt (x * x + y * y);\n";
    code += "   float c = asin (rho);\n";
    code += "   float lat = asin (cos (c) * sin (d.y) + (((y * sin (c)) * cos (d.y) / rho)));\n";
    code += "   float lon = d.x + atan ((x * sin (c)), (rho * cos (d.y) * cos (c) - y * sin (d.y) * sin (c)));\n";
    code += "   float visible = rho;\n";
    code += "   return vec3 (lon, lat, visible);\n";
    code += "}\n";
    return code;
}

QString OrthographicProjection::glslForward() {
    QString code = "if (p == PROJ_ORTHOGRAPHIC) {\n";
    code += "   float x = cos (g.y) * sin (g.x - d.x);\n";
    code += "   float y = cos (d.y) * sin (g.y) - sin (d.y) * cos (g.y) * cos (g.x - d.x);\n";
    code += "   float rho = sin (d.y) * sin (g.y) + cos (d.y) * cos (g.y) * cos (g.x - d.x);\n";
    code += "   return vec3 (x, y, rho);\n";
    code += "}\n";
    return code;
}


QPointF OrthographicProjection::range () {
    return QPointF (M_PI, M_PI);
}

double OrthographicProjection::aspectRatio () {
    return 0;
}
//
// Created by martin on 04/09/17.
//

#include "OrthographicProjection.h"
