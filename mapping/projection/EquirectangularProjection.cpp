//
// Created by martin on 01/09/17.
//

#include "EquirectangularProjection.h"

using namespace calenhad::mapping::projection;
using namespace geoutils;

EquirectangularProjection::EquirectangularProjection () : Projection(),
    _datum (Geolocation (0.0, 0.0)) {

}

EquirectangularProjection::~EquirectangularProjection () {

}

void EquirectangularProjection::setDatum (const Geolocation& datum) {
    _datum.setLatitude (0.0);
    _datum.setLongitude (datum.longitude());
};

bool EquirectangularProjection::inverse (const QPointF& point, Geolocation& geolocation) {
    double x = point.x();
    double y = point.y();
    geolocation.setLatitude (y);
    geolocation.setLongitude (x + _datum.longitude());
    bool valid = (geolocation.latitude() >= -M_PI / 2) && (geolocation.latitude() <= M_PI / 2);
    return valid;
}

bool EquirectangularProjection::forward (const geoutils::Geolocation& geolocation, QPointF& point) {
    point.setX ((geolocation.longitude() - _datum.longitude()) * cos (_datum.latitude()));
    point.setY (geolocation.latitude());
    bool valid = (geolocation.latitude() >= -M_PI / 2) && (geolocation.latitude() <= M_PI / 2);
    return valid;
}

QString EquirectangularProjection::name () {
    return "Equirectangular projection";
}

QString EquirectangularProjection::notes () {
    return "";
}

int EquirectangularProjection::id () {
    return 0; // see map_cs.glslInverse
}

QString EquirectangularProjection::glslInverse() {
    QString code = "if (p == PROJ_EQUIRECTANGULAR) {\n";
    code += "   vec2 g = vec2 (i.x + d.x, i.y);\n";
    code += "   float visible = abs (g.y / (M_PI / 2));\n";
    code += "   return vec3 (g.xy, visible);\n";
    code += "}\n";
    return code;
}

QString EquirectangularProjection::glslForward() {
    QString code = "if (p == PROJ_EQUIRECTANGULAR) {\n";
    code += "   vec2 i = vec2 ((g.x - d.x) * cos (d.y), g.y);\n";
    code += "   float visible = abs (g.y / (M_PI / 2));\n";
    code += "   return vec3 (i.xy, visible);\n";
    code += "}\n";
    return code;
}
