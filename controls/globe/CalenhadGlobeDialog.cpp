//
// Created by martin on 16/01/17.
//

#include "CalenhadGlobeDialog.h"
#include "CalenhadGlobeConfigDialog.h"
#include "CalenhadNavigator.h"
#include <QResizeEvent>
#include <ostream>
#include <iostream>
#include "../../legend/Legend.h"
#include "../../legend/LegendWidget.h"
#include <QMenu>
#include "CalenhadGlobeContextMenu.h"
#include <GeographicLib/Geodesic.hpp>
#include <QtWidgets/QToolTip>
#include <CalenhadServices.h>
#include <graph/graph.h>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtXml/QtXml>
#include <sstream>
#include "../../legend/LegendService.h"
#include "../../pipeline/CalenhadModel.h"
#include <QWindow>
#include "../../mapping/projection/Projection.h"

class QwtCompass;


using namespace GeographicLib;
using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls::globe;
using namespace calenhad::qmodule;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace calenhad::graph;
using namespace geoutils;

CalenhadGlobeDialog::CalenhadGlobeDialog (QWidget* parent, QModule* source) : QDialog (parent),

    _configDialog (nullptr),
    _contextMenu (nullptr),
    _moveFrom (QPoint (0, 0)),
    _globe (new CalenhadMapView (this)),
    _graph (nullptr),
    _geodesic (new Geodesic (Constants::WGS84_a(), Constants::WGS84_f())) {

    // Turn on mouse tracking so that we can keep showing the mouse pointer coordinates.
    _globe -> setMouseTracking (true);
    _globe -> setContextMenuPolicy (Qt::CustomContextMenu);
    connect (_globe, &QWidget::customContextMenuRequested, this, &CalenhadGlobeDialog::showContextMenu);
    _globe -> setSource (source);
    _globe -> setProjection ("Orthographic");

}

void CalenhadGlobeDialog::initialise() {

    _zoomSlider = new QwtSlider (this);
    _zoomSlider -> setGroove (true);
    _zoomSlider -> setTrough (false);
    _zoomSlider -> setScalePosition (QwtSlider::NoScale);
    _zoomSlider -> setLowerBound (1.2);
    _zoomSlider -> setUpperBound (0.01);
    _zoomSlider -> move (width() - 20, height() - 20);
    _zoomSlider -> setFixedSize (40, 150);

    // add our own overview map
    _overview = new CalenhadMapView (this);
    _overview -> setFixedSize (210, 105);
    _overview -> move (20, 20);
    _overview -> setSource (_globe -> source());
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect -> setBlurRadius (10);
    effect -> setOffset (4);
    _overview -> setGraphicsEffect (effect);
    connect (_zoomSlider, SIGNAL (valueChanged (const double&)), this, SLOT (setZoom (const double&)));
    _zoomSlider -> setValue (1.0);

    // navigator - our navigator replaces both the navigation buttons and the compass rose
    _navigator = new CalenhadNavigator (this);
    _navigator -> move (width() - 20, 20);
    _zoomSlider -> setFixedSize (100, 100);
    connect (_navigator, SIGNAL (navigationRequested (const NavigationEvent&)), this, SLOT (navigate (const NavigationEvent&)));
    connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
    _mouseDragMode = CalenhadGlobeDragMode::Pan;
    _mouseDoubleClickMode = CalenhadGlobeDoubleClickMode::Goto;
    _positionLabel = new QLabel (this);

    invalidate();
}

CalenhadGlobeDialog::~CalenhadGlobeDialog() {
    if (_configDialog) { delete _configDialog; }
    delete _geodesic;
    if (_graph) { delete _graph; }
}


void CalenhadGlobeDialog::invalidate () {
    _globe -> update();
}

void CalenhadGlobeDialog::resizeEvent (QResizeEvent* e) {
    QWidget::resizeEvent (e);

    // let the map fill the whole widget. To do - have aspect ratio of image be consistent with the projection
    _globe -> setFixedSize (size());

    // update positions and sizes of the control widgets
    _zoomSlider -> move (std::max (20, width() - 20 - _zoomSlider -> width()), std::max (20, height() - 20 - _zoomSlider -> height()));
    _zoomSlider -> setFixedSize (40, std::max (150, height() - 200));
    _navigator -> setFixedSize (100, 100);
    _navigator -> move (std::max (20, width() - 20 - _navigator -> height()), 20);

    emit resized (e -> size());
}

void CalenhadGlobeDialog::showContextMenu (const QPoint& pos) {
    if (! _contextMenu) {
        _contextMenu = new CalenhadGlobeContextMenu (this);
        connect (_contextMenu, SIGNAL (showOverviewMap (const bool&)), this, SLOT (showOverviewMap (const bool&)));
        connect (_contextMenu, SIGNAL (showZoomSlider (const bool&)), this, SLOT (showZoomSlider (const bool&)));
        connect (_contextMenu, SIGNAL (setScaleVisible (const bool&)), this, SLOT (setScaleVisible (const bool&)));
        connect (_contextMenu, SIGNAL (showNavigator (const bool&)), this, SLOT (showNavigator (const bool&)));
        connect (_contextMenu, SIGNAL (showGraticule (const bool&)), this, SLOT (setGraticuleVisible (const bool&)));
        connect (_contextMenu, SIGNAL (dragModeSelected (const CalenhadGlobeDragMode&)), this, SLOT (setMouseDragMode (const CalenhadGlobeDragMode&)));
        connect (_contextMenu, SIGNAL (doubleClickModeSelected (const CalenhadGlobeDoubleClickMode&)), this, SLOT (setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode&)));
        connect (_contextMenu, SIGNAL (projectionSelected (QString)), _globe, SLOT (setProjection (const QString&)));
        connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
    }
    _contextMenu -> exec (mapToGlobal (pos));

}

void CalenhadGlobeDialog::showOverviewMap (const bool& show) {
    _overview -> setVisible (show);
    update();
}


void CalenhadGlobeDialog::showZoomSlider (const bool& show) {
    _zoomSlider -> setVisible (show);
    update();
}

void CalenhadGlobeDialog::showNavigator (const bool& show) {
    _navigator -> setVisible (show);
    update();
}

// move the view centre along a given azimuth
void CalenhadGlobeDialog::navigate (const NavigationEvent& e) {

    // calculate the great circle distance across the viewport's diagonal
    double s;
    _geodesic -> Inverse (qRadiansToDegrees (_bounds.south()), qRadiansToDegrees ( _bounds.east()), qRadiansToDegrees ( _bounds.north()), qRadiansToDegrees ( _bounds.west()), s);
    // move the viewport centre in the chosen direction by half this distance multiplied by the chosen navigation magnitude
    double lat, lon;
    double distance = e.distance() * s / 2;
    _geodesic -> Direct (qRadiansToDegrees (_bounds.center().latitude()), qRadiansToDegrees ( _bounds.center().longitude()), e.azimuth(), distance, lat, lon);
    _globe -> goTo (Geolocation (lon, lat));

}

void CalenhadGlobeDialog::paintEvent (QPaintEvent* e) {
    QWidget::paintEvent (e);
    if (_graticuleVisible) {
        // draw graticule
    }
}

void CalenhadGlobeDialog::setZoom (const double& zoom) {
    _globe -> setZoom (zoom);
    invalidate();
}

double CalenhadGlobeDialog::zoom() {
    return _globe -> zoom();
}

bool CalenhadGlobeDialog::isOverviewVisible () {
    //return _overview -> isVisible();
    return false; // for now
}

bool CalenhadGlobeDialog::isZoomBarVisible () {
    return _zoomSlider -> isVisible();
}

bool CalenhadGlobeDialog::isCompassVisible () {
    return _navigator -> isVisible();
}

void CalenhadGlobeDialog::setMouseDragMode (const CalenhadGlobeDragMode& mouseMode) {
   if (mouseMode != _mouseDragMode) {
        _mouseDragMode = mouseMode;
    }
}

void CalenhadGlobeDialog::setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mouseMode) {
    _mouseDoubleClickMode = mouseMode;
}

const CalenhadGlobeDragMode CalenhadGlobeDialog::dragMode () {
    return _mouseDragMode;
}

void CalenhadGlobeDialog::showConfigDialog() {
    if (!_configDialog) {
        _configDialog = new CalenhadGlobeConfigDialog (this);
        connect (_configDialog, &QDialog::accepted, this, &CalenhadGlobeDialog::updateConfig);
    }
    _configDialog -> initialise();
    _configDialog -> exec();
}

void CalenhadGlobeDialog::updateConfig () {
    _configDialog -> commitChanges();
    setScalebarVisible (_configDialog -> scaleCheckState());
    showOverviewMap (_configDialog -> overviewCheckState ());
    showZoomSlider (_configDialog -> zoomBarCheckState ());
    showNavigator (_configDialog -> compassCheckState());
    setGraticuleVisible (_configDialog -> graticuleCheckState ());
    setMouseDragMode (_configDialog -> dragMode ());
    setMouseDoubleClickMode (_configDialog -> doubleClickMode());
    _globe -> setSensitivity (_configDialog -> mouseSensitivity());
    //setProjection (_configDialog -> selectedProjection());
    _configDialog -> update();
    _contextMenu -> update();
    for (Legend* legend : CalenhadServices::legends() -> all()) {
        legend -> widget () -> updateLegend();
    }
     update();
    _globe -> setCoordinatesFormat (_configDialog -> coordinatesFormat());
    _globe -> setDatumFormat (_configDialog -> datumFormat());
}


const CalenhadGlobeDoubleClickMode CalenhadGlobeDialog::doubleClickMode () {
    return _mouseDoubleClickMode;
}

void CalenhadGlobeDialog::setGraticuleVisible (const bool& visible) {
   _graticuleVisible = visible;
}

bool CalenhadGlobeDialog::isGraticuleVisible() {
     return _graticuleVisible;
}

bool CalenhadGlobeDialog::isScaleVisible () {
    // to do
}

void CalenhadGlobeDialog::setScalebarVisible (bool visible) {
    // to do
}

CalenhadMapView* CalenhadGlobeDialog::mapWidget () {
    return _globe;
}
