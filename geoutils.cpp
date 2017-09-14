/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "geoutils.h"
#include <QtGui/QtGui>
#include <sstream>
namespace geoutils {

    Geolocation::Geolocation () : Geolocation (0.0, 0.0, Units::Radians) {}

    Geolocation::Geolocation (const double& newLat, const double& newLon, const Units& units) {
        setLatitude (newLat, units);
        setLongitude (newLon, units);
        conform();
    }

    Geolocation::Geolocation (const Geolocation& other) : Geolocation (other._latitude, other._longitude, Units::Radians) {

    }

    bool Geolocation::operator== (const Geolocation& other) const {
        return _latitude == other._latitude && _longitude == other._longitude;
    }

    void Geolocation::operator+= (const Geolocation& other) {
       setLatitude (_latitude + other._latitude);
       setLongitude (_longitude + other._longitude);
       conform();
    }

    void Geolocation::operator-= (const Geolocation& other) {
        setLatitude (_latitude - other._latitude);
        setLongitude (_longitude - other._longitude);
        conform();
    }

    Geolocation Geolocation::operator+ (const Geolocation& other) {
        Geolocation g = other;
        g._latitude = _latitude + other._latitude;
        g._longitude = _longitude + other._longitude;
        g.conform();
        return g;
    }

    Geolocation Geolocation::operator- (const Geolocation& other) {
        Geolocation g = other;
        g._latitude = _latitude - other._latitude;
        g._longitude = _longitude - other._longitude;
        g.conform();
        return g;
    }

    void Geolocation::conform() {
        if (_latitude > M_PI / 2) { setLatitude (_latitude - M_PI); }
        if (_latitude < -M_PI / 2) { setLatitude (_latitude + M_PI); }
        if (_longitude > M_PI) { setLongitude (_longitude - M_PI * 2); }
        if (_longitude < -M_PI) { setLongitude (_longitude + M_PI * 2); }
    }

    double Geolocation::latitude (const Units& units) const {
        return units == Units::Radians ? _latitude : radiansToDegrees (_latitude);
    }

    double Geolocation::longitude (const Units& units) const {
        return units == Units::Radians ? _longitude : radiansToDegrees (_longitude);
    }

    void Geolocation::setLatitude (const double& lat, const Units& units) {
        _latitude = units == Units::Radians ? lat : degreesToRadians (lat);
        conform();
    }

    void Geolocation::setLongitude (const double& lon, const Units& units) {
        _longitude = units == Units::Radians ? lon : degreesToRadians (lon);
        conform();
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
            if (from.longitude() == unto.longitude()) {
                if (from.latitude() > unto.latitude()) {
                    result = M_PI;
                }
            } else {
                double c = acos (
                        sin (unto.latitude()) * sin (from.latitude()) + cos (unto.latitude()) * cos (from.latitude()) * cos ((unto.longitude() - from.longitude())));
                result = asin (cos (unto.latitude()) * sin ((unto.longitude() - from.longitude())) / sin (c));
            }

            if ((unto.latitude() < from.latitude() && unto.longitude() < from.longitude())) {
                result = M_PI - result;
            } else if ((unto.latitude() < from.latitude() && unto.longitude() > from.longitude())) {
                result = M_PI - result;
            } else if ((unto.latitude() > from.latitude() && unto.longitude() < from.longitude())) {
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
        GeographicLib::Geocentric gc = GeographicLib::Geocentric (1, 0);
        double lat, lon, h;
        gc.Reverse (c.x, c.y, c.z, lat, lon, h);
        return Geolocation (lat, lon, Units::Degrees);
    }

    Cartesian Math::toCartesian (const Geolocation& g) {
        //double x, y, z;
        //x = cos (g.latitude) * cos (g.longitude);
        //y = cos (g.latitude) * sin (g.longitude);
        //z = sin (g.latitude);
        GeographicLib::Geocentric gc = GeographicLib::Geocentric (1, 0);
        double x, y, z;
        gc.Forward (g.latitude (Units::Degrees), g.longitude (Units::Degrees), 0, x, y, z);
        return Cartesian (x, y, z);
    }


    QString Math::toTraditional (double ang, const unsigned& num_dec_places) {
        bool neg (false);
        if (ang < 0.0) {
            neg = true;
            ang = -ang;
        }

        int deg = (int) ang;
        double frac = ang - (double) deg;

        frac *= 60.0;

        int min = (int) frac;

        frac = frac - (double) min;

        // fix the DDD MM 60 case
        // TODO: nearbyint isn’t alway available (Visual C++, for example)
        double sec = nearbyint (frac * 600000.0);
        sec /= 10000.0;

        if (sec >= 60.0) {
            min++;
            sec -= 60.0;
        }

        std::ostringstream oss;

        if (neg) {
            oss << "-";
        }

//  TODO: allow user to define delimiters separating degrees, minutes, and seconds.
        oss.setf (std::ios::fixed, std::ios::floatfield);

        oss << deg << "°";
        oss.fill ('0');
        oss.width (2);
        oss << min << "\'";
        if (num_dec_places == 0) {
            oss.width (2);
            oss.precision (0);
        } else {
            oss.width (num_dec_places + 3);
            oss.precision (num_dec_places);
        }
        oss << sec
            << "\"";

        return QString::fromStdString (oss.str ());
    }


} // namespace geoutils