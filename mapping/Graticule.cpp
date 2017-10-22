//
// Created by martin on 16/10/17.
//

#include <QtGui/QList>
#include <tgmath.h>
#include "Graticule.h"
#include "CalenhadMapWidget.h"
#include <QPainter>
#include <CalenhadServices.h>
#include "../preferences/PreferencesService.h"

using namespace calenhad::mapping;
using namespace geoutils;


Graticule::Graticule (CalenhadMapWidget* parent) : _globe (parent),
    _visible (true),
     _density (0),
     _majorPen (QPen()),
     _minorPen (QPen()),
     _bottom  (QLineF (QPointF (00.0, parent -> height() - 10.0), QPointF (parent -> width(), _globe -> height() - 10.0))),
    _left (QLineF (QPointF (10.0, 0.0), QPointF (10.0, _globe -> height()))),
    _centralLat (0.0), _centralLon (0.0) {
    _majorPen.setColor (CalenhadServices::preferences() -> calenhad_graticule_major_color);
    _majorPen.setStyle (Qt::PenStyle (CalenhadServices::preferences() -> calenhad_graticule_major_style));
    _majorPen.setWidth (CalenhadServices::preferences() -> calenhad_graticule_major_weight);
    _minorPen.setColor (CalenhadServices::preferences() -> calenhad_graticule_minor_color);
    _minorPen.setStyle (Qt::PenStyle (CalenhadServices::preferences() -> calenhad_graticule_minor_style));
    _minorPen.setWidth (CalenhadServices::preferences() -> calenhad_graticule_minor_weight);
}

Graticule::~Graticule() {

}

double Graticule::pitch (const int& i) {
    if (i <= 0) { return 30.0; }
    if (i == 1) { return 15.0; }
    if (i == 2) { return 5.0; }
    if (i == 3) { return 2.5; }
    if (i == 4) { return 1.0; }

    double f = 1.0;
    int j = i;
    while (j > 4) {
        j -= 3;
        f *= 0.1;
    }
    return pitch (j) * f;
}

double Graticule::subdivisions (const int& i) {
    if (i <= 0) { return 30.0; }
    if (i == 1) { return 15.0; }
    if (i == 2) { return 10.0; }
    if (i == 3) { return 5.0; }
    if (i == 4) { return 1.0; }


    double f = 1.0;
    int j = i;
    while (j > 4) {
        j -= 3;
        f *= 0.1;
    }
    return subdivisions (j) * f;
}

void Graticule::drawGraticule (QPainter& p) {
    _bottom = QLineF (QPointF (0.0, _globe -> height() - 10.0), QPointF (_globe -> width(), _globe -> height() - 10.0));
    _left = QLineF (QPointF (10.0, 0.0), QPointF (10.0, _globe -> height()));

    int level = 0;
    while (_intersections.size() < 16) {
        _intersections.clear();
        makeGraticule (p, level);
        level++;
    }

    _intersections.clear();
    makeGraticule (p, level + _density - 1);
    QSet<double> longitudes, latitudes;
    for (QPair<double, double> intersection : _intersections) {
        longitudes.insert (intersection.second);
        latitudes.insert (intersection.first);
    }

    for (double lat : latitudes) {
        if (! drawLatitude (p, lat, level + _density - 1)) {
            placeLatitudeLabel (p, lat);
        };
    }

    for (double lon : longitudes) {
        if (! drawLongitude (p, lon, level + _density - 1)) {
            placeLongitudeLabel (p, lon);
        };
    }
}

bool Graticule::drawLatitude (QPainter& p, const double& lat, const int& level) {
    bool label = false;
    for (QPair<double, double> intersection : _intersections) {
        if (intersection.first == lat) {
            if (drawLatitudeIntersection (p, intersection, level - 1)) { label = true; }
        }
    }
    return label;
}

bool Graticule::drawLongitude (QPainter& p, const double& lon, const int& level) {
    bool label = false;
    for (QPair<double, double> intersection : _intersections) {
        if (intersection.second == lon) {
            if (drawLongitudeIntersection (p, intersection, level - 1)) { label = true; }
        }
    }
    return label;
}

void Graticule::makeGraticule (QPainter& p, const int& level) {
    Geolocation centre = _globe -> rotation();
    double interval = pitch (level);
    _centralLat = (std::floor (centre.latitude (Units::Degrees) / interval)) * interval;
    _centralLon = (std::floor (centre.longitude (Units::Degrees) / interval)) * interval;
    if (_globe -> isInViewport (Geolocation (_centralLat, _centralLon, Units::Degrees))) {
        getIntersections (QPair<double, double> (_centralLat, _centralLon), interval);
    }
}

void Graticule::getIntersections (const QPair<double, double>& g, const double& interval) {
    if (_intersections.contains (g)) { return; }

    _intersections.insert (g);
    if (_globe -> isInViewport (Geolocation (g.first, g.second, Units::Degrees))) {
        double wlon = g.second - interval;
        if (wlon < -180.0) { wlon += 360.0; }
        getIntersections (QPair<double, double> (g.first, wlon), interval);
        double elon = g.second + interval;
        if (elon > 180.0) { elon -= 360.0; }
        getIntersections (QPair<double, double>  (g.first, elon), interval);
        double nlat = g.first + interval;
        if (nlat <= 90.0) {
            getIntersections (QPair<double, double> (nlat, g.second), interval);
        }
        double slat = g.first - interval;
        if (slat >= -90.0) {
            getIntersections (QPair<double, double> (slat, g.second), interval);
        }
    }
}

bool Graticule::drawLatitudeIntersection (QPainter& p, const QPair<double, double>& g, const int& level) {
    QPointF start, end;
    double interval = pitch (level);
    bool label = false;
    int segments = 10;
    double q = subdivisions (level);

    // extend the meridian in segments to the north and south
    p.setPen (g.second / q == std::floor (g.second / q) ? _majorPen : _minorPen);
    _globe->screenCoordinates (Geolocation (g.first - interval, g.second, Units::Degrees), start);
    double lat0 = g.first - interval;
    for (int i = 0; i <= segments; i++) {
        Geolocation g0 = Geolocation (lat0, g.second, Units::Degrees);
        bool visible = _globe->screenCoordinates (g0, end);
        if (visible) {
            if (lat0 > -90.0) {             // removes an artefact which appears if both poles are visible
                if (!_globe -> inset () || (!_globe->insetRect ().contains (start) && !_globe->insetRect ().contains (end))) {
                    QPointF point;
                    QLineF line = QLineF (start, end);
                    p.drawLine (line);
                    QLineF::IntersectType type = line.intersect (_bottom, &point);
                    if (type == QLineF::IntersectType::BoundedIntersection) {
                        p.drawText (point, QString::number (g.second));
                        label = true;
                    }
                }
            }
        }
        start = end;
        lat0 += interval / segments;
    }
    return label;
}

bool Graticule::drawLongitudeIntersection (QPainter& p, const QPair<double, double>& g, const int& level) {
    QPointF start, end;

    double interval = pitch (level);
    bool label = false;
    int segments = 10;
    double q = subdivisions (level);
    // extend the parallel in segments to the east and west
    p.setPen (g.first / q == std::floor (g.first / q) ? _majorPen : _minorPen);
    _globe->screenCoordinates (Geolocation (g.first, g.second - interval, Units::Degrees), start);
    double lon0 = g.second - interval;
    for (int i = 0; i <= segments; i++) {
        Geolocation g0 = Geolocation (g.first, lon0, Units::Degrees);
        bool visible = _globe -> screenCoordinates (g0, end);
        if (visible) {
            if (! _globe -> inset() || (! _globe -> insetRect().contains (start) && ! _globe -> insetRect().contains (end))) {

                QPointF point;
                QLineF line = QLineF (start, end);
                p.drawLine (line);
                QLineF::IntersectType type = line.intersect (_left, &point);
                if (type == QLineF::IntersectType::BoundedIntersection) {
                    p.drawText (point, QString::number (g.first));
                    label = true;
                }
            }
            start = end;
            lon0 += interval / segments;
        }
    }
    return label;
}

void Graticule::placeLatitudeLabel (QPainter& p, const double& lat) {
    QPointF point;
    if (_globe -> screenCoordinates (Geolocation (lat, _centralLon, Units::Degrees), point)) {
        p.drawText (point, QString::number (lat));
    }
}

void Graticule::placeLongitudeLabel (QPainter& p, const double& lon) {
    QPointF point;
    if (_globe -> screenCoordinates (Geolocation (_centralLat, lon, Units::Degrees), point)) {
        p.drawText (point, QString::number (lon));
    }
}

bool Graticule::visible () const {
    return _visible;
}

void Graticule::setVisible (bool visible) {
    _visible = visible;
}

int Graticule::density () const {
    return _density;
}

void Graticule::setDensity (const int& density) {
    _density = density;
}

QPen Graticule::majorPen() {
    return _majorPen;
}

QPen Graticule::minorPen() {
    return _minorPen;
}

void Graticule::setPens (QPen majorPen, QPen minorPen) {
    _majorPen = majorPen;
    _minorPen = minorPen;
}
