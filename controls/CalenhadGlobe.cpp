//
// Created by martin on 16/01/17.
//

#include "CalenhadGlobe.h"
#include "CalenhadGlobeConfigDialog.h"
#include <marble/MarbleDirs.h>
#include <QResizeEvent>
#include "CalenhadOverviewMap.h"
#include "marble/MarbleWidgetInputHandler.h"
#include "../qmodule/QNode.h"
#include <QMenu>
#include <marble/MarbleMap.h>
#include <marble/GeoPainter.h>
#include "../mapping/CalenhadLayer.h"
#include "CalenhadGlobeContextMenu.h"
#include <marble/ViewportParams.h>
#include <GeographicLib/Geodesic.hpp>

class QwtCompass;

using namespace Marble;
using namespace GeographicLib;

CalenhadGlobe::CalenhadGlobe (QModule* source, QWidget* parent) : QWidget (parent),
    _configDialog (nullptr),
    _source (source),
    _mouseMode (CalenhadGlobeMouseMode::Pan),
    _contextMenu (nullptr),
    _moveFrom (QPoint (0, 0)),
    _geodesic (new Geodesic (Constants::WGS84_a(), Constants::WGS84_f())) {
    _map = new MarbleMap();
    _map -> setMapThemeId (QStringLiteral ("earth/calenhad/calenhad.dgml"));
    std::cout <<  "Marble data path '" << Marble::MarbleDirs::marbleDataPath ().toStdString () << "'\n";
    std::cout << "Map theme id '" << _map -> mapThemeId ().toStdString () << "'\n";
    setContextMenuPolicy (Qt::CustomContextMenu);

    _map -> setShowBackground (false);
    _map -> setSize (size());

    // add a layer for the module
    _layer = new CalenhadLayer (source);
    _map -> addLayer (_layer);

    connect (_layer, SIGNAL (imageRefreshed()), this, SLOT (invalidate ()));

    // remove the map components we don't want - scale and overviewmap both relate to Earth
            foreach (AbstractFloatItem* item, _map -> floatItems()) {
            //std::cout << item -> nameId().toStdString () << "\n";
            if (item && (
                item -> nameId() == "overviewmap" ||
                item -> nameId() == "compass" ||
                item -> nameId() == "GpsInfo" ||
                item -> nameId() == "license" ||
                item -> nameId() == "navigation" ||
                item -> nameId() == "porgress" ||
                item -> nameId() == "routing" ||
                item -> nameId() == "speedometer" )) {
                item->setVisible (false);
                _map -> floatItems().removeOne (item);
            }
        }
    connect (_map, &MarbleMap::visibleLatLonAltBoxChanged, this, &CalenhadGlobe::changeView);
    connect (this, SIGNAL (resized (const QSize&)), _layer, SLOT (rescale()));
    connect (&timer, SIGNAL (timeout()), this, SLOT (invalidate()));
    connect (_source, SIGNAL (nodeChanged()), this, SLOT (invalidate()));

    // slider to control the zoom
    QGridLayout* layout = new QGridLayout (this);
    layout -> setMargin (5);
    setLayout (layout);
    _zoomSlider = new QwtSlider (this);
    _zoomSlider -> setGroove (true);
    _zoomSlider -> setTrough (false);
    _zoomSlider -> setScalePosition (QwtSlider::NoScale);
    layout -> setRowMinimumHeight (0, 150);
    layout -> addWidget (_zoomSlider, 2, 2, Qt::AlignRight);
    _zoomSlider -> setLowerBound (1.0);
    _zoomSlider -> setUpperBound (180.0);

    // add our own overview map
    _overview = new CalenhadOverviewMap (source, this);
    _overview -> setFixedSize (210, 105);
    _overview -> initialise();
    layout -> addWidget (_overview, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);

    connect (_zoomSlider, SIGNAL (valueChanged (const double&)), this, SLOT (setZoom (const double&)));
    _zoomSlider -> setValue (10);

    // navigator - our navigator replaces both the navigation buttons and the compass rose
    _navigator = new CalenhadNavigator (this);
    layout -> addWidget (_navigator, 0, 2, Qt::AlignTop | Qt::AlignRight);
    connect (_navigator, SIGNAL (navigationRequested (const NavigationEvent&)), this, SLOT (navigate (const NavigationEvent&)));
    connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
}

CalenhadGlobe::~CalenhadGlobe() {
    if (_layer) { delete _layer; }
    if (_map) { delete _map; }
    if (_configDialog) { delete _configDialog; }
    delete _geodesic;
}


void CalenhadGlobe::invalidate () {
    update();
    _overview -> update();
}

void CalenhadGlobe::paintEvent (QPaintEvent* e) {
    GeoPainter geoPainter( this, _map -> viewport(), _map -> mapQuality() );
    _map -> paint( geoPainter, e -> rect() );
}

void CalenhadGlobe::resizeEvent (QResizeEvent* e) {
    //Marble::
    QWidget::resizeEvent (e);
    _map -> setSize (width(), height ());
    emit resized (e -> size());
}

void CalenhadGlobe::showContextMenu (const QPoint& pos) {
    std::cout << "showContextMenu mouseMode=" << _mouseMode << "\n";
    if (! _contextMenu) {
        _contextMenu = new CalenhadGlobeContextMenu (this);
        connect (_contextMenu, SIGNAL (showOverviewMap (const bool&)), this, SLOT (showOverviewMap (const bool&)));
        connect (_contextMenu, SIGNAL (showZoomSlider (const bool&)), this, SLOT (showZoomSlider (const bool&)));
        connect (_contextMenu, SIGNAL (setFloatItemVisible (const bool&)), this, SLOT (setFloatItemVisible (const bool&)));
        connect (_contextMenu, SIGNAL (showNavigator (const bool&)), this, SLOT (showNavigator (const bool&)));
        connect (_contextMenu, SIGNAL (mouseModeSelected (const CalenhadGlobeMouseMode&)), this, SLOT (setMouseMode (const CalenhadGlobeMouseMode&)));
        connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
    }
    _contextMenu -> exec (mapToGlobal (pos));

}

void CalenhadGlobe::showOverviewMap (const bool& show) {
    _overview -> setVisible (show);
    update();
}


void CalenhadGlobe::showZoomSlider (const bool& show) {
    _zoomSlider -> setVisible (show);
    update();
}

void CalenhadGlobe::showNavigator (const bool& show) {
    _navigator -> setVisible (show);
    update();
}

void CalenhadGlobe::navigate (const NavigationEvent& e) {
    // use GeographicLib to solve the geodesic problems
    GeoDataLatLonAltBox box = _map -> viewport() -> viewLatLonAltBox();

    // calculate the great circle distance across the viewport's diagonal
    double s;
    _geodesic -> Inverse (RAD2DEG * box.south(), RAD2DEG * box.east(), RAD2DEG * box.north(), RAD2DEG * box.west(), s);
    std::cout << s << "\n";
    // move the viewport centre in the chosen direction by half this distance multiplied by the chosen navigation magnitude
    double lat, lon;
    double distance = e.distance() * s / 2;
    _geodesic -> Direct (_map -> centerLatitude(), _map -> centerLongitude(), e.azimuth(), distance, lat, lon);
    _map -> setCenterLatitude (lat);
    _map -> setCenterLongitude (lon);
    update();
}

void CalenhadGlobe::mousePressEvent (QMouseEvent* e) {

    if (e->button () == Qt::RightButton) {

    }
    if (e -> button() == Qt::LeftButton) {
        _moveFrom = e->pos ();
    }
}

void CalenhadGlobe::mouseMoveEvent (QMouseEvent* e) {
    double dx = e -> pos().x() - _moveFrom.x();
    double dy = e -> pos().y() - _moveFrom.y();

    if (_mouseMode == CalenhadGlobeMouseMode::Pan) {
        double dLat = (180.0 / _map -> radius()) * dy * _sensitivity;
        double dLon = (180.0 / _map -> radius()) * dx * _sensitivity;
        _map->setCenterLatitude (_map -> centerLatitude() + dLat);
        _map->setCenterLongitude (_map -> centerLongitude() - dLon);
        _moveFrom = e -> pos();
        update ();
    }

    if (_mouseMode == CalenhadGlobeMouseMode::Zoom) {

        double zoom = _zoom;
        double dz = -dy * _sensitivity * 0.2;
        if (zoom > 1.0 + dz || zoom < 180.0 - dz) {
            _zoomSlider->setValue (zoom + dz);
            std::cout << zoom << "    " << _zoom << "\n";
        }
    }
}

void CalenhadGlobe::mouseReleaseEvent (QMouseEvent* e) {

}

void CalenhadGlobe::setZoom (const double& zoom) {
    double radius = (int) (std::pow (2, zoom / 16)) * 100;
    _map -> setRadius (radius);
    _zoom = zoom;

}

double CalenhadGlobe::zoom() {
    return _zoom;
}

void CalenhadGlobe::changeView() {
    GeoDataLatLonAltBox bounds = _map -> viewport() -> viewLatLonAltBox();
    _overview -> setBounds (bounds);
    _overview -> render();
    emit viewChanged (bounds);
    _layer -> rescale();
    timer.setSingleShot (true);
    timer.start();
}

void CalenhadGlobe::setFloatItemVisible (const bool& visible) {
    QAction* source = (QAction*) sender();
    setFloatItemVisible (visible, source -> data().toString());
}

void CalenhadGlobe::setFloatItemVisible (const bool& visible, const QString& nameId) {
            foreach (AbstractFloatItem* item, _map -> floatItems()) {
            if (item && (item -> nameId() == nameId)) {
                item -> setVisible (visible);
                update();
            }
        }
}

bool CalenhadGlobe::isOverviewVisible () {
    return _overview -> isVisible();
}

bool CalenhadGlobe::isFloatItemVisible (const QString& nameId) {
   foreach (AbstractFloatItem* item, _map -> floatItems()) {
        if (item && (item -> nameId() == nameId)) {
            return item -> visible();
        }
   }
    return false;
}

bool CalenhadGlobe::isZoomBarVisible () {
    return _zoomSlider -> isVisible();
}

bool CalenhadGlobe::isCompassVisible () {
    return _navigator -> isVisible();
}

void CalenhadGlobe::setMouseMode (const CalenhadGlobeMouseMode& mouseMode) {
    if (mouseMode != _mouseMode) {
        _mouseMode = mouseMode;
        std::cout << "CalenhadGlobe -> setMouseMode (new) mouseMode =" << _mouseMode << "\n";
    }
}

const CalenhadGlobeMouseMode CalenhadGlobe::mouseMode () {
    return _mouseMode;
}

void CalenhadGlobe::showConfigDialog() {
    if (!_configDialog) {
        _configDialog = new CalenhadGlobeConfigDialog (this);
        connect (_configDialog, &QDialog::accepted, this, &CalenhadGlobe::updateConfig);
    }
    _configDialog -> initialise();
    _configDialog -> exec();
}

void CalenhadGlobe::updateConfig () {
    std::cout << "CalenhadGlobe -> updateConfig mouseMode=" << _mouseMode << "\n";
    setFloatItemVisible (_configDialog -> scaleCheckState(), "scalebar");
    showOverviewMap (_configDialog -> overviewCheckState ());
    showZoomSlider (_configDialog -> zoomBarCheckState ());
    showNavigator (_configDialog -> compassCheckState());
    setMouseMode (_configDialog -> mouseMode ());
    setSensitivity (_configDialog -> mouseSensitivity());
    _configDialog -> update();
    _contextMenu -> update();
}

void CalenhadGlobe::setSensitivity (const double& sensitivity) {
    _sensitivity = sensitivity;
}

double CalenhadGlobe::sensitivity() {
    return _sensitivity;
}