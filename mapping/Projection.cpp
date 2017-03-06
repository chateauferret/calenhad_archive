//
// Created by martin on 12/02/17.
//

#include "Projection.h"

using namespace geoutils;


class InvalidGeoProjectionException : public std::runtime_error {
public:
    InvalidGeoProjectionException (const std::string& msg) : runtime_error (msg) {}
};

Projection::Projection (const QString& proj) {

    if (! (pj_out = pj_init_plus (proj.toStdString().c_str())) || ! (pj_latlong = pj_init_plus ("+proj=latlong +ellps=sphere"))) {
        throw InvalidGeoProjectionException ("Failed to create projection");
    }

}

Projection::~Projection () {

}

Geolocation Projection::forward (double x, double y) {
    int p = pj_transform (pj_latlong, pj_out, 1, 1, &x, &y, NULL );
    return Geolocation (x, y);
}

std::pair<double, double> Projection::inverse (const geoutils::Geolocation& latlon) {
    double x = latlon.longitude;
    double y = latlon.latitude;
    int p = pj_transform (pj_latlong, pj_out, 1, 1, &x, &y, NULL );
    return std::make_pair (x, y);

}
