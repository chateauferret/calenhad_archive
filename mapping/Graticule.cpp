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


Graticule::Graticule (CalenhadMapWidget* parent) : _globe (parent), _visible (true), _density (0) {

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

void Graticule::drawGraticule (QPainter& p) {
    int level = 0;
    int drawn = 0;
    while (drawn < 32) {
        drawn = drawGraticule (p, level);
        level++;
        std::cout << "Level " << level << " drawn " << drawn << "\n";
    }
    std::cout << "done\n";
}

int Graticule::drawGraticule (QPainter& p, const int& level) {
    int _level = level;
    QPen pen;
    pen.setColor (CalenhadServices::preferences() -> calenhad_graticule_major_color);
    pen.setStyle (Qt::PenStyle (CalenhadServices::preferences() -> calenhad_graticule_major_style));
    pen.setWidth (CalenhadServices::preferences() -> calenhad_graticule_major_weight);
    p.setPen (pen);
    Geolocation centre = _globe -> rotation();

    // find highest-level lat and lon that are in the viewport


    QSet<QPair<double, double>> result;
    double interval = pitch (_level);
    double lat = (std::floor (centre.latitude (Units::Degrees) / interval)) * interval;
    double lon = (std::floor (centre.longitude (Units::Degrees) / interval)) * interval;
    if (_globe -> isInViewport (Geolocation (lat, lon, Units::Degrees))) {
        getIntersections (QPair<double, double> (lat, lon), interval, result);
    }

    for (QPair<double, double> intersection : result) {
        drawGraticuleIntersection (p, intersection, _level);
    }
    return result.size();
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
    double interval = pitch (level);

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

int Graticule::density () const {
    return _density;
}

void Graticule::setDensity (int density) {
    _density = density;
}
