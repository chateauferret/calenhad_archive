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


Graticule::Graticule (CalenhadMapWidget* parent) : _globe (parent), _visible (true), _density (1), _majorPen (QPen()), _minorPen (QPen()) {

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

    _majorPen.setColor (CalenhadServices::preferences() -> calenhad_graticule_major_color);
    _majorPen.setStyle (Qt::PenStyle (CalenhadServices::preferences() -> calenhad_graticule_major_style));
    _majorPen.setWidth (CalenhadServices::preferences() -> calenhad_graticule_major_weight);
    _minorPen.setColor (CalenhadServices::preferences() -> calenhad_graticule_minor_color);
    _minorPen.setStyle (Qt::PenStyle (CalenhadServices::preferences() -> calenhad_graticule_minor_style));
    _minorPen.setWidth (CalenhadServices::preferences() -> calenhad_graticule_minor_weight);
    QSet<QPair<double, double>> result;
    int level = 0;
    while (result.size() < 16) {
        result.clear();
        makeGraticule (p, level, result);
        level++;
    }

    result.clear();
    makeGraticule (p, level + _density - 1, result);

    for (QPair<double, double> intersection : result) {
        drawGraticuleIntersection (p, intersection, level - 1);
    }
}

void Graticule::makeGraticule (QPainter& p, const int& level, QSet<QPair<double, double>>& result) {
    Geolocation centre = _globe -> rotation();
    double interval = pitch (level);
    double lat = (std::floor (centre.latitude (Units::Degrees) / interval)) * interval;
    double lon = (std::floor (centre.longitude (Units::Degrees) / interval)) * interval;
    if (_globe -> isInViewport (Geolocation (lat, lon, Units::Degrees))) {
        getIntersections (QPair<double, double> (lat, lon), interval, result);
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
    double interval = pitch (level);

    int segments = 10;
    double q = subdivisions (level);

    // extend the meridian in segments to the north and south
    p.setPen (g.second / q ==  std::floor (g.second / q) ? _majorPen : _minorPen);
    _globe -> screenCoordinates (Geolocation (g.first - interval, g.second, Units::Degrees), start);
    double lat0 = g.first - interval;
    for (int i = 0; i <= segments; i++) {
        Geolocation g0 = Geolocation (lat0, g.second, Units::Degrees);
        bool visible = _globe -> screenCoordinates (g0, end);
        if (visible) {
            if (lat0 > -90.0) {             // removes an artefact which appears if both poles are visible
                if (! _globe -> inset() || (! _globe -> insetRect().contains (start) && ! _globe -> insetRect().contains (end))) {
                    p.drawLine (start, end);
                }
            }
        }
        start = end;
        lat0 += interval / segments;
    }

    // extend the parallel in segments to the east and west
    p.setPen (g.first / q == std::floor (g.first / q) ? _majorPen : _minorPen);
    _globe->screenCoordinates (Geolocation (g.first, g.second - interval, Units::Degrees), start);
    double lon0 = g.second - interval;
    for (int i = 0; i <= segments; i++) {
        Geolocation g0 = Geolocation (g.first, lon0, Units::Degrees);
        bool visible = _globe -> screenCoordinates (g0, end);
        if (visible) {
            if (! _globe -> inset() || (! _globe -> insetRect().contains (start) && ! _globe -> insetRect().contains (end))) {
                p.drawLine (start, end);
            }
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
