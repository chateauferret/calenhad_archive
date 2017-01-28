/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "geoutils.h"
#include <cmath>
#include <math.h>
#include <QtGui/QtGui>
namespace geoutils {
// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

    const unsigned Geolocation::RADS = 0;
    const unsigned Geolocation::DEGS = 1;

    Geolocation::Geolocation () : Geolocation (0.0, 0.0, Geolocation::RADS) {}

    Geolocation::Geolocation (const double& newLat, const double& newLon, const unsigned& units) {
        if (units == DEGS) {
            latitude = degreesToRadians (newLat);
            longitude = degreesToRadians (newLon);
            latDegrees = newLat;
            lonDegrees = newLon;
        } else {
            latitude = newLat;
            longitude = newLon;
            latDegrees = radiansToDegrees (newLat);
            lonDegrees = radiansToDegrees (newLon);
        }
    }

    Geolocation::Geolocation (const Geolocation& other) : Geolocation (other.latitude, other.longitude, Geolocation::RADS) {
        latDegrees = other.latDegrees;
        lonDegrees = other.lonDegrees;
    }

    bool Geolocation::operator== (const Geolocation& other) const {
        return latitude == other.latitude && longitude == other.longitude;
    }

    Geolocation Geolocation::operator+ (const Geolocation& other) {
        Geolocation g = other;
        g.latitude = latitude + other.latitude;
        g.longitude = longitude + other.longitude;
        g.latDegrees = latDegrees + other.latDegrees;
        g.lonDegrees = lonDegrees + other.lonDegrees;
        return g;
    }

    QString Geolocation::toString () {
        return ""; // to do
    }

    Cartesian::Cartesian () : Cartesian (0.0, 0.0, 1.0) {}

    Cartesian::Cartesian (const double& newx, const double& newy, const double& newz) : x (newx), y (newy), z (newz) { Math::normalise (*this); }

    Cartesian::Cartesian (const Cartesian& other) : Cartesian (other.x, other.y, other.z) {}

    bool Cartesian::operator== (const Cartesian& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    Cartesian Cartesian::operator+ (const Cartesian& other) {
        Cartesian c;
        c.x = x + other.x;
        c.y = y + other.y;
        c.z = z + other.z;
        Math::normalise (c);
        return c;
    }

    Cartesian Cartesian::operator- (const Cartesian& other) {
        Cartesian c = Cartesian (x - other.x, y - other.y, z - other.z);
        Math::normalise (c);
        return c;
    }

    GeoQuad::GeoQuad (const Geolocation& g1, const Geolocation& g2) : _crossesDateline (false) {
        if (g1.latitude == g2.latitude || g1.longitude == g2.longitude) { throw IllegalGeoCoordinatesException ("Bounds must define a region"); }
        _north = g1.latitude > g2.latitude ? g1.latitude : g2.latitude;
        _south = g1.latitude <= g2.latitude ? g1.latitude : g2.latitude;
        _east = g1.longitude > g2.longitude ? g1.longitude : g2.longitude;
        _west = g1.longitude <= g2.longitude ? g1.longitude : g2.longitude;
    }

    Geolocation GeoQuad::nw () const {
        return Geolocation (_north, _west, Geolocation::RADS);
    }

    Geolocation GeoQuad::se () const {
        return Geolocation (_south, _east, Geolocation::RADS);
    }

    Geolocation GeoQuad::ne () const {
        return Geolocation (_north, _east, Geolocation::RADS);
    }

    Geolocation GeoQuad::sw () const {
        return Geolocation (_south, _west, Geolocation::RADS);
    }

    QString GeoQuad::toString () {
        return ne ().toString () + " - " + sw ().toString ();
    }

    GeoQuad::GeoQuad (const Geolocation& g1, const Geolocation& g2, const Geolocation& pointWithin) : GeoQuad (g1, g2) {
        if (!(contains (pointWithin))) {
            _crossesDateline = true;
            if (!contains (pointWithin)) {
                throw IllegalGeoCoordinatesException ("Point given must lie within latitude range of bounds");
            }
        }
    }

    std::pair<std::vector<GeoQuad>::iterator, std::vector<GeoQuad>::iterator> GeoQuad::rationalise () {
        std::vector<GeoQuad> partitions;
        if (crossesDateline ()) {
            partitions.push_back (GeoQuad (Geolocation (se ().longitude, se ().latitude), Geolocation (M_PI / 2, nw ().latitude)));
            partitions.push_back (GeoQuad (Geolocation (-M_PI / 2, se ().latitude), Geolocation (nw ().longitude, nw ().latitude)));
        } else {
            partitions.push_back (*this);
        }
        return make_pair (partitions.begin (), partitions.end ());
    }

    bool GeoQuad::crossesDateline () {
        return _crossesDateline;
    }

    bool GeoQuad::contains (const Geolocation& g) const {
        if (_crossesDateline) {
            return (g.latitude > se ().latitude && g.latitude < nw ().latitude && !(g.longitude > nw ().longitude && g.longitude < se ().longitude));
        } else {
            return (g.latitude > se ().latitude && g.latitude < nw ().latitude && g.longitude > nw ().longitude && g.longitude < se ().longitude);
        }
    }

    const Geolocation GeoQuad::centre () const {
        double lat = (nw ().latitude + se ().latitude) / 2;
        double lon = (nw ().longitude + se ().longitude) / 2;
        if (_crossesDateline) {
            lon += lon > 0 ? -M_PI : M_PI;
        }
        return Geolocation (lat, lon);
    }

    double Math::distSquared (const Cartesian& a, const Cartesian& b) {
        double dx = abs (a.x - b.x);
        double dy = abs (a.y - b.y);
        double dz = abs (a.z - b.z);
        double dist = dx * dx + dy * dy + dz * dz;
        return dist;
    }

    bool Math::isNearer (const Cartesian& a, const Cartesian& b, const Cartesian& c) {
        return distSquared (a, b) < distSquared (a, c);
    }

    double Math::azimuth (const Geolocation& from, const Geolocation& unto) {
        double result = 0.0;
        if (from == unto) {
            return result;
        } else {
            if (from.longitude == unto.longitude) {
                if (from.latitude > unto.latitude) {
                    result = M_PI;
                }
            } else {
                double c = acos (
                        sin (unto.latitude) * sin (from.latitude) + cos (unto.latitude) * cos (from.latitude) * cos ((unto.longitude - from.longitude)));
                result = asin (cos (unto.latitude) * sin ((unto.longitude - from.longitude)) / sin (c));
            }

            if ((unto.latitude < from.latitude && unto.longitude < from.longitude)) {
                result = M_PI - result;
            } else if ((unto.latitude < from.latitude && unto.longitude > from.longitude)) {
                result = M_PI - result;
            } else if ((unto.latitude > from.latitude && unto.longitude < from.longitude)) {
                result += 2 * M_PI;
            }
        }
        return result;
    }

    void Math::normalise (Cartesian& cartesian) {
        double magnitude = sqrt (cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z);
        cartesian.x /= magnitude;
        cartesian.y /= magnitude;
        cartesian.z /= magnitude;
    }


    Geolocation Math::toGeolocation (const Cartesian& c) {
        //double lon = atan2 (c.y, c.x);
        //double lat = asin (c.z);
        //return Geolocation (lat, lon, Geolocation::RADS);
        GeographicLib::Geocentric gc = GeographicLib::Geocentric (1, 0);
        double lat, lon, h;
        gc.Reverse (c.x, c.y, c.z, lat, lon, h);
        return Geolocation (lat, lon, Geolocation::DEGS);

    }

    Cartesian Math::toCartesian (const Geolocation& g) {
        //double x, y, z;
        //x = cos (g.latitude) * cos (g.longitude);
        //y = cos (g.latitude) * sin (g.longitude);
        //z = sin (g.latitude);
        GeographicLib::Geocentric gc = GeographicLib::Geocentric (1, 0);
        double x, y, z;
        gc.Forward (g.latDegrees, g.lonDegrees, 0, x, y, z);
        return Cartesian (x, y, z);
    }
} // namespace geoutils