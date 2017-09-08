//
// Created by martin on 04/09/17.
//

#include "MercatorProjection.h"

using namespace calenhad::mapping::projection;
using namespace geoutils;

MercatorProjection::MercatorProjection () : Projection(), _datum (Geolocation (0.0, 0.0)) {

}

MercatorProjection::~MercatorProjection () {

}

void MercatorProjection::setDatum (const Geolocation& datum) {
    _datum.setLatitude (0.0);
    _datum.setLongitude (datum.longitude());
};

bool MercatorProjection::inverse (const QPointF& point, Geolocation& geolocation) {
    double x = point.x();
    double y = point.y();
    geolocation.setLatitude (M_PI / 2 - 2 * atan (exp (-y)));
    geolocation.setLongitude ((x / cos (_datum.latitude())) + _datum.longitude());
    bool valid = (geolocation.latitude() >= -M_PI / 2) && (geolocation.latitude() <= M_PI / 2);
    return valid;
}

bool MercatorProjection::forward (const geoutils::Geolocation& geolocation, QPointF& point) {
    point.setX ((geolocation.longitude() - _datum.longitude()) * cos (_datum.latitude()));
    point.setY (geolocation.latitude() - _datum.latitude());
    bool valid = (geolocation.latitude() >= -M_PI / 2) && (geolocation.latitude() <= M_PI / 2);
    return valid;
}

QString MercatorProjection::name () {
    return "Mercator projection";
}

QString MercatorProjection::notes () {
    return "";
}

int MercatorProjection::id () {
    return 1; // see map_cs.glsl
}

QString MercatorProjection::glsl() {
    QString code = "if (projection == PROJ_MERCATOR) {\n";
    code += "    float lat = M_PI / 2 - 2 * atan (exp (-i.y));\n";
    code += "    float lon = i.x;\n";
    code += "    vec2 g = vec2 (lon, lat);\n";
    code += "    vec3 cart = toCartesian (g);\n";
    code += "   return vec4 (cart, abs (g.y / M_PI * 2));\n";
    code += "}\n";
    return code;
}
