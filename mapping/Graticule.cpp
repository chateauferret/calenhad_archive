//
// Created by martin on 16/10/17.
//

#include <QtGui/QList>
#include <tgmath.h>
#include "Graticule.h"
#include "AbstractMapWidget.h"
#include <QPainter>
#include <CalenhadServices.h>
#include "../preferences/PreferencesService.h"

using namespace calenhad::mapping;
using namespace geoutils;
using namespace calenhad::controls::globe;


Graticule::Graticule (AbstractMapWidget* parent) : _globe (parent),
                                                   _visible (true),
                                                   _density (0),
                                                   _majorPen (QPen()),
                                                   _minorPen (QPen()),
                                                   _bottom  (QLineF (QPointF (00.0, parent -> height() - 10.0), QPointF (parent -> width(), _globe -> height() - 10.0))),
                                                   _left (QLineF (QPointF (10.0, 0.0), QPointF (10.0, _globe -> height()))),
                                                   _insetRight (QLineF()),
                                                   _insetTop (QLineF()),
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
    if (i <= 0) { return 90.0; }
    if (i == 1) { return 30.0; }
    if (i == 2) { return 15.0; }
    if (i == 3) { return 5.0; }
    if (i == 4) { return 2.5; }
    if (i == 5) { return 1.0; }

    double f = 1.0;
    int j = i;
    while (j > 5) {
        j -= 3;
        f *= 0.1;
    }
    return pitch (j) * f;
}

double Graticule::subdivisions (const int& i) {
    if (i <= 0) { return 90.0; }
    if (i == 1) { return 30.0; }
    if (i == 2) { return 15.0; }
    if (i == 3) { return 10.0; }
    if (i == 4) { return 5.0; }
    if (i == 5) { return 1.0; }


    double f = 1.0;
    int j = i;
    while (j > 5) {
        j -= 3;
        f *= 0.1;
    }
    return subdivisions (j) * f;
}

void Graticule::drawGraticule (QPainter& p) {



    _intersections.clear();
    _bottom = QLineF (QPointF (0.0, _globe -> height() - 10.0), QPointF (_globe -> width(), _globe -> height() - 10.0));
    _left = QLineF (QPointF (10.0, 0.0), QPointF (10.0, _globe -> height()));

        _bottom = QLineF (QPointF (0.0, _globe -> height() - 10.0), QPointF (_globe -> width(), _globe -> height() - 10.0));
        _left = QLineF (QPointF (10.0, 0.0), QPointF (10.0, _globe -> height()));


    int level = 0;
    while (_intersections.size() < 16) {
        _intersections.clear();
        makeGraticule (p, level);
        level++;
    }

    _intersections.clear();
    _longitudesLabelled.clear();
    _latitudesLabelled.clear();

    makeGraticule (p, level + _density - 1);
    QSet<double> longitudes, latitudes;
    for (QPair<double, double> intersection : _intersections) {
        longitudes.insert (intersection.second);
        latitudes.insert (intersection.first);
    }

    for (double lon : longitudes) {
        drawLongitude (p, lon, level + _density - 1);
    }

    for (double lat : latitudes) {
        drawLatitude (p, lat, level + _density - 1);
    }

    for (double lon : longitudes) {
        placeLongitudeLabel (p, lon);
    }

    for (double lat : latitudes) {
        placeLatitudeLabel (p, lat);
    }

}

void Graticule::drawLatitude (QPainter& p, const double& lat, const int& level) {
    for (QPair<double, double> intersection : _intersections) {
        if (intersection.first == lat) {
            drawLatitudeIntersection (p, intersection, level);
        }
    }
}

void Graticule::drawLongitude (QPainter& p, const double& lon, const int& level) {
    for (QPair<double, double> intersection : _intersections) {
        if (intersection.second == lon) {
            drawLongitudeIntersection (p, intersection, level);
        }
    }
}

void Graticule::makeGraticule (QPainter& p, const int& level) {
    Geolocation centre = _globe -> renderMode() == RenderMode::RenderModeOverview ? Geolocation (0, 0, Degrees) : _globe -> rotation();
    double interval = pitch (level);
    _centralLat = (std::floor (centre.latitude (Units::Degrees) / interval)) * interval;
    _centralLon = (std::floor (centre.longitude (Units::Degrees) / interval)) * interval;
    if (_globe -> isInViewport (Geolocation (_centralLat, _centralLon, Units::Degrees))) {
        getIntersections (QPair<double, double> (_centralLat, _centralLon), interval);
    }
}

void Graticule::getIntersections (const QPair<double, double>& g, const double& interval) {
    if (_intersections.contains (g) || _intersections.size() > 10000) { return; }

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

void Graticule::drawLatitudeIntersection (QPainter& p, const QPair<double, double>& g, const int& level) {
    QPointF start, end;
    double interval = pitch (level);
    int segments = 10;
    double q = subdivisions (level);

    // extend the meridian in segments to the north and south
    p.setPen (g.second / q == std::floor (g.second / q) ? _majorPen : _minorPen);
    _globe -> screenCoordinates (Geolocation (g.first - interval, g.second, Units::Degrees), start);

    double lat0 = g.first - interval;
    for (int i = 0; i <= segments; i++) {
        if (!isinf (start.x ()) && !isinf (start.y ()) && !isinf (end.x ()) && !isinf (end.y ())) {
            Geolocation g0 = Geolocation (lat0, g.second, Units::Degrees);
            bool visible = _globe->screenCoordinates (g0, end);
            if (visible) {
                if (lat0 > -90.0 && lat0 < 90.0) {             // removes an artefact which appears if both poles are visible
                    QPointF point;
                    QLineF line = QLineF (start, end);
                    if (start.x() > 0 && start.y() > 0) {
                        if (!_longitudesLabelled.contains (g.second)) {

                            if (line.intersect (_bottom, &point) == QLineF::IntersectType::BoundedIntersection) {
                                drawLongitudeLabel (p, point, longitudeLabel (g.second));
                                _longitudesLabelled.append (g.second);
                            }
                        }

                            p.drawLine (line);

                    }
                }
            }
        }
        start = end;
        lat0 += interval / segments;
    }

}

void Graticule::drawLongitudeIntersection (QPainter& p, const QPair<double, double>& g, const int& level) {
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
        if (!isinf (start.x ()) && !isinf (start.y ()) && !isinf (end.x ()) && !isinf (end.y ())) {
            Geolocation g0 = Geolocation (g.first, lon0, Units::Degrees);
            bool visible = _globe -> screenCoordinates (g0, end);
            if (visible) {
                QPointF point;
                QLineF line = QLineF (start, end);
                if (start.x() > -20 && start.y() > 0) {
                    if (!_latitudesLabelled.contains (g.first)) {

                        if (line.intersect (_left, &point) == QLineF::IntersectType::BoundedIntersection) {
                            drawLatitudeLabel (p, point, latitudeLabel (g.first));
                            _latitudesLabelled.append (g.first);
                        }
                    }
                        p.drawLine (line);

                }

            }
        }
        start = end;
        lon0 += interval / segments;
    }
}

void Graticule::placeLatitudeLabel (QPainter& p, const double& lat) {
    QPointF point;
    if (_globe -> screenCoordinates (Geolocation (lat, _centralLon, Units::Degrees), point)) {
        if (! _latitudesLabelled.contains (lat)) {
            drawLatitudeLabel (p, point, latitudeLabel (lat));
            _latitudesLabelled.append (lat);
        }
    }
}

void Graticule::placeLongitudeLabel (QPainter& p, const double& lon) {
    QPointF point;
    if (_globe -> screenCoordinates (Geolocation (_centralLat, lon, Units::Degrees), point)) {
        if (! _longitudesLabelled.contains (lon)) {
            drawLongitudeLabel (p, point, longitudeLabel (lon));
            _longitudesLabelled.append (lon);
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

QString Graticule::longitudeLabel (const double& lon, const bool& major) {
    QString label;
    if (_globe -> coordinatesFormat() == CoordinatesFormat::Traditional) {
        label = geoutils::Math::toTraditional (std::abs (lon));
        if (label.contains (".00")) {
            label = label.replace (".00", "");
            if (label.contains ("00\"")) {
                label = label.replace ("00\"", "");
                if (label.contains ("00\'")) {
                    label = label.replace ("00\'", "");
                }
            }
        }
    } else {
        label = QString::number (std::abs (lon));
        label.append ("°");
    }
    label.append (lon > 0 ? "E" : "W");
    return label;
}

QString Graticule::latitudeLabel (const double& lat, const bool& major) {
    QString label;
    if (lat == 0.0) {
        return ("Equator");
    }
   if (_globe -> coordinatesFormat() == CoordinatesFormat::Traditional) {
       label = geoutils::Math::toTraditional (std::abs (lat));
       if (label.contains ("00\"")) {
           label = label.replace ("00\"", "");
           if (label.contains ("00\'")) {
               label = label.replace ("00\'", "");
           }
       }
    } else {
       label = QString::number (std::abs (lat));
       label.append ("°");
    }
    label.append (lat > 0 ? "N" : "S");
    return label;
}

void Graticule::drawLatitudeLabel (QPainter& p, QPointF point, const QString& text) {
    if (point.y() < _globe -> height() - 20) {
        drawLabel (p, point, text);
    }
}

void Graticule::drawLongitudeLabel (QPainter& p, QPointF point, const QString& text) {
    if (point.x() > 20) {
        drawLabel (p, point, text);
    }
}

void Graticule::drawLabel (QPainter& p, QPointF point, const QString& text) {
    QFont font ("arial", 10);
    QFontMetrics fm (font);
    p.setFont (font);
    int w = fm.width (text);

        point.setX (std::max (10.0, point.x () - w / 2));

    p.drawText (point, text);
}