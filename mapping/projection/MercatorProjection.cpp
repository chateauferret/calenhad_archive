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
    geolocation.setLongitude (x + _datum.longitude());
    bool valid = (geolocation.latitude() >= -M_PI / 2) && (geolocation.latitude() <= M_PI / 2);
    return valid;
}

bool MercatorProjection::forward (const geoutils::Geolocation& geolocation, QPointF& point) {
    point.setX (geolocation.longitude() - _datum.longitude());
    point.setY (std::log (tan (M_PI / 4 + geolocation.latitude()) / 2));
    bool valid = (geolocation.latitude() >= -M_PI / 2) && (geolocation.latitude() <= M_PI / 2);
    return valid;
}

QString MercatorProjection::name () {
    return "Mercator";
}

QString MercatorProjection::notes () {
    return "";
}

int MercatorProjection::id () {
    return 1; // see map_cs.glsl
}

QString MercatorProjection::glslInverse() {
    QString code = "if (p == PROJ_MERCATOR) {\n";
    code += "    float lat = M_PI / 2 - 2 * atan (exp (-i.y));\n";
    code += "    float lon = i.x + d.x;\n";
    code += "    vec2 g = vec2 (lon, lat);\n";
    code += "    return vec3 (g.xy, abs (g.y / (M_PI / 2)));\n";
    code += "}\n";
    return code;
}

QString MercatorProjection::glslForward() {
    QString code = "if (p == PROJ_MERCATOR) {\n";
    code += "    float y = log (tan (M_PI / 4 + g.y) / 2);\n";
    code += "    float x = g.x - d.x;\n";
    code += "    vec2 i = vec2 (x, y);\n";
    code += "    return vec3 (i.xy, abs (g.y / (M_PI / 2)));\n";
    code += "}\n";
    return code;
}

double MercatorProjection::aspectRatio () {
    return 0;
}
