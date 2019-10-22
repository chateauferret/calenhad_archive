//
// Created by martin on 08/05/17.
//

#include "CalenhadNavigator.h"
#include <QTimer>
#include <QMouseEvent>
#include <qwt/qwt_compass_rose.h>
#include <QPainter>
#include <iostream>
#include <cmath>
#include "mapping/AbstractMapWidget.h"
#include <geoutils.h>

using namespace calenhad::controls::globe;
using namespace GeographicLib;
using namespace geoutils;

CalenhadNavigator::CalenhadNavigator (QWidget* parent) : QwtCompass (parent), _active (false), _buffer (nullptr), _geodesic (new Geodesic (1, 0)) {
    QPalette p;
    for (int c = 0; c < QPalette::NColorRoles; c++) {
        const QPalette::ColorRole colorRole = static_cast<QPalette::ColorRole> (c);
        p.setColor (colorRole, QColor());
    }

    p.setColor (QPalette::Base, Qt::darkBlue);
    p.setColor (QPalette::WindowText, QColor (Qt::darkBlue).dark (120));
    p.setColor (QPalette::Text, Qt::white);
    p.setColor (QPalette::Background, Qt::transparent);

    for ( int i = 0; i < QPalette::NColorGroups; i++ ) {
        const QPalette::ColorGroup colorGroup = static_cast<QPalette::ColorGroup>(i);
        const QColor light = p.color (colorGroup, QPalette::Base).light (170);
        const QColor dark = p.color (colorGroup, QPalette::Base).dark (170);
        const QColor mid = frameShadow() == QwtDial::Raised
                           ? p.color (colorGroup, QPalette::Base).dark (110)
                           : p.color (colorGroup, QPalette::Base).light (110);

        p.setColor (colorGroup, QPalette::Dark, dark);
        p.setColor (colorGroup, QPalette::Mid, mid);
        p.setColor (colorGroup, QPalette::Light, light);
    }
    setPalette (p);

    QwtSimpleCompassRose *rose = new QwtSimpleCompassRose (16, 4);
    rose -> setPalette (p);
    setRose (rose);

    QwtCompassScaleDraw *scaleDraw = new QwtCompassScaleDraw();
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Ticks, true );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Labels, true );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
    scaleDraw->setTickLength( QwtScaleDiv::MinorTick, 1 );
    scaleDraw->setTickLength( QwtScaleDiv::MediumTick, 1 );
    scaleDraw->setTickLength( QwtScaleDiv::MajorTick, 3 );
    _mousePos = scaleDraw -> center ();

    setFixedSize (200, 200);
    setScaleDraw (scaleDraw);
    setScaleMaxMajor (36);
    setScaleMaxMinor (5);
    setMouseTracking (true);

}

void CalenhadNavigator::mouseMoveEvent (QMouseEvent* e) {
    _mousePos = e -> pos();
    QPointF centre = scaleDraw() -> center ();
    double dx = std::abs ((int) e -> pos().x() - centre.x());
    double dy = std::abs ((int) e -> pos().y() - centre.y());
    _length = 2 * std::sqrt ((dx * dx) + (dy * dy)) / scaleInnerRect().width();
    _active = _length < 1.05;
    _length = std::min (1.0, _length);
    int v = (int) (scrolledTo (e -> pos ())) % 360;
    setValue (v);

    calenhad::mapping::AbstractMapWidget* view = static_cast<calenhad::mapping::AbstractMapWidget*> (parent());
    if (view) {
        double lat, lon;
        double azimuth = value();
        _geodesic -> Direct (view -> rotation().latitude (Units::Degrees), view -> rotation().longitude (Units::Degrees), azimuth, _length, lat, lon);
        setToolTip (geoutils::Math::geoLocationString (Geolocation (lat, lon, Units::Degrees), view -> coordinatesFormat()));
    }
    update();
}

void CalenhadNavigator::mouseReleaseEvent (QMouseEvent* e) {
    if (e -> button() == Qt::LeftButton) {
        _active = false;
        NavigationEvent e (value(), _length);
        emit navigationRequested (e);
    }
}

CalenhadNavigator::~CalenhadNavigator () {
    delete _buffer;
    delete _geodesic;
}

void CalenhadNavigator::paintEvent (QPaintEvent* e) {
    if (_active) {
        setAttribute (Qt::WA_OpaquePaintEvent, true);
        QPainter painter (this);
        painter.drawPixmap (0, 0, *_buffer);
        painter.save ();
        painter.setPen (Qt::yellow);
        QPointF centre = scaleDraw() -> center ();
        painter.drawLine (centre, _mousePos);
        painter.restore ();
    } else {
        setAttribute (Qt::WA_OpaquePaintEvent, false);
        QwtDial::paintEvent (e);
        if (! _buffer) {
            _buffer = new QPixmap (size());
            *_buffer = grab();
        }
    }
}

double CalenhadNavigator::length () {
    return _length;
}

NavigationEvent::NavigationEvent (const double& azimuth, const double& distance) : _azimuth (azimuth), _distance (distance) {

}

NavigationEvent::~NavigationEvent() {

}

double NavigationEvent::azimuth()const {
    return _azimuth;
}

double NavigationEvent::distance()const {
    return _distance;
}