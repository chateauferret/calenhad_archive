//
// Created by martin on 16/10/17.
//

#include <QtGui/QList>
#include <tgmath.h>
#include "Graticule.h"
#include "CalenhadMapWidget.h"
#include <QPainter>

using namespace calenhad::mapping;
using namespace geoutils;


Graticule::Graticule (CalenhadMapWidget* parent) : _globe (parent), _visible (true), _level (0) {

}

Graticule::~Graticule() {

}

QList<double> Graticule::graticules() {
    QList<double> list;
    list.append ({ 30, 15, 5, 1, 0.5, 0.25, 0.1, 0.05, 0.025, 0.01, 0.005, 0.0025, 0.001 });
    return list;
}

void Graticule::drawGraticule (QPainter& p, const int& level) {
    p.setPen (Qt::yellow);
    Geolocation centre = _globe -> rotation();
    int drawnLat = 0, drawnLon = 0;
    int count = 0;
    // find highest-level lat and lon that are in the viewport
    double interval = graticules().at (level);

    double lat = (std::floor (centre.latitude (Units::Degrees) / interval)) * interval;
    double lon = (std::floor (centre.longitude (Units::Degrees) / interval)) * interval;

    if (_globe -> isInViewport (Geolocation (lat, lon, Units::Degrees))) {
        QSet<QPair<double, double>> result;
        getIntersections (QPair<double, double> (lat, lon), interval, result);
        for (QPair<double, double> g : result) {
            drawGraticuleIntersection (p, g, level);
        }
    } else {
        if (interval != graticules().last()) {
            drawGraticule (p, level + 1);
        }
    }
}

void Graticule::getIntersections (const QPair<double, double>& g, const double& interval, QSet<QPair<double, double>>& result) {
    if (result.contains (g)) { return; }
    result.insert (g);
    if (_globe -> isInViewport (Geolocation (g.first, g.second, Units::Degrees))) {
        double wlon = g.second - interval;
        if (wlon < -180.0) { wlon += 360.0; }
        getIntersections (QPair<double, double> (g.first, wlon), interval, result);
        double elon = g.second + interval;
        if (elon > 180.0) { elon -= 360.0; }
        getIntersections (QPair<double, double>  (g.first, elon), interval, result);
        double nlat = g.first + interval;
        if (nlat <= 90.0) {
            getIntersections (QPair<double, double> (nlat, g.second), interval, result);
        }
        double slat = g.first - interval;
        if (slat >= -90.0) {
            getIntersections (QPair<double, double> (slat, g.second), interval, result);
        }
    }
}

void Graticule::drawGraticuleIntersection (QPainter& p, const QPair<double, double>& g, const int& level) {
    QPointF start, end;
    double interval = graticules().at (level);

    int segments = 10;


    // extend the meridian in segments to the north and south
    _globe -> screenCoordinates (Geolocation (g.first - interval, g.second, Units::Degrees), start);
    double lat0 = g.first - interval;
    for (int i = 0; i <= segments; i++) {
        Geolocation g0 = Geolocation (lat0, g.second, Units::Degrees);
        bool visible = _globe -> screenCoordinates (g0, end);
        if (visible) {
            if (lat0 > -90.0) {             // removes an artefact which appears if both poles are visible
                p.drawLine (start, end);
            }
            start = end;
            lat0 += interval / segments;
        }
    }

    // extend the parallel in segments to the east and west
    _globe -> screenCoordinates (Geolocation (g.first, g.second - interval, Units::Degrees), start);
    double lon0 = g.second - interval;
    for (int i = 0; i <= segments; i++) {
        Geolocation g0 = Geolocation (g.first, lon0, Units::Degrees);
        bool visible = _globe -> screenCoordinates (g0, end);
        if (visible) {
            p.drawLine (start, end);
            start = end;
            lon0 += interval / segments;
        }
    }
}

bool Graticule::visible () const {
    return _visible;
}

void Graticule::setVisible (bool visible) {
    _visible = visible;
}

int Graticule::level () const {
    return _level;
}

void Graticule::setLevel (int level) {
    _level = level;
}
