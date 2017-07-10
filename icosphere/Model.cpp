
#include "Model.h"

using namespace icosphere;

    Model::Model (const unsigned& depth) : _depth (depth) {
        _gc = new GeographicLib::Geocentric (1.0, 0.0); // sphere
        _gd = new GeographicLib::Geodesic (1.0, 0.0);
        _rhumb = new GeographicLib::Rhumb (1.0, 0.0);
    }

    Model::~Model() {
        delete _gc;
        delete _gd;
        delete _rhumb;
    }

    double Model::distance (const Geolocation& from, const Geolocation& unto) const {
        double d;
        _gd -> Inverse (from.latitude, from.longitude, unto.latitude, unto.longitude, d);
        return d;
    }

    double Model::loxodrome (const Geolocation& from, const Geolocation& unto) {
        double azimuth;
        double length;
        _rhumb -> Inverse (from.latitude, from.longitude, unto.latitude, unto.longitude, azimuth, length);
        return azimuth;
    }

