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
#include <QtWidgets/QToolTip>
#include <ostream>
#include <sstream>
#include "marble/GeoDataPolygon.h"
#include "../CalenhadServices.h"


class QwtCompass;

using namespace Marble;
using namespace GeographicLib;
using namespace icosphere;

CalenhadGlobe::CalenhadGlobe (QModule* source, QWidget* parent) : QWidget (parent),
    _coordinatesFormat (CoordinatesFormat::Traditional),
    _datumFormat (DatumFormat::Scaled),
    _layer (nullptr),
    _zoomDrag (false),
    _configDialog (nullptr),
    _source (source),
    _contextMenu (nullptr),
    _moveFrom (QPoint (0, 0)),
    _geodesic (new Geodesic (Constants::WGS84_a(), Constants::WGS84_f())) {

    // Turn on mouse tracking so that we can keep showing the mouse pointer coordinates.
    setMouseTracking (true);

    // We need a sphere model to obtain map values for the tooltips.
    sphereModel.SetModule (*(source -> module()));

    _map = new MarbleMap();
    _map -> setMapThemeId (QStringLiteral ("earth/calenhad/calenhad.dgml"));
    std::cout <<  "Marble data path '" << Marble::MarbleDirs::marbleDataPath ().toStdString () << "'\n";
    std::cout << "Map theme id '" << _map -> mapThemeId ().toStdString () << "'\n";
    setContextMenuPolicy (Qt::CustomContextMenu);
    _map -> setShowBackground (false);
    _map -> setSize (size());

    // add a layer for the module
    _layer = new CalenhadLayer (source);

    connect (_layer, &CalenhadLayer::renderingStart, this, &CalenhadGlobe::renderingStart);
    connect (_layer, &CalenhadLayer::renderingFinished, this, &CalenhadGlobe::renderingFinished);

    _map -> addLayer (_layer);
    //_map -> setShowRuntimeTrace (true);

    connect (_layer, SIGNAL (imageRefreshed()), this, SLOT (invalidate ()));

    // remove the map components we don't want - scale and overviewmap both relate to Earth
            foreach (AbstractFloatItem* item, _map -> floatItems()) {
            if (item && (
                item -> nameId() == "overviewmap" ||
                item -> nameId() == "compass" ||
                item -> nameId() == "GpsInfo" ||
                item -> nameId() == "license" ||
                item -> nameId() == "navigation" ||
                item -> nameId() == "progress" ||
                item -> nameId() == "routing" ||
                item -> nameId() == "speedometer" )) {
                item->setVisible (false);
                _map -> floatItems().removeOne (item);
            }
        }
    connect (_map, &MarbleMap::visibleLatLonAltBoxChanged, this, &CalenhadGlobe::changeView);
    connect (this, SIGNAL (resized (const QSize&)), _layer, SLOT (rescale()));
    connect (&_renderTimer, SIGNAL (timeout()), this, SLOT (invalidate()));
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
    layout -> addWidget (_zoomSlider, 3, 3, Qt::AlignRight);
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
    layout -> addWidget (_navigator, 0, 3, Qt::AlignTop | Qt::AlignRight);
    connect (_navigator, SIGNAL (navigationRequested (const NavigationEvent&)), this, SLOT (navigate (const NavigationEvent&)));
    connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
    _mouseDragMode = CalenhadGlobeDragMode::Pan;
    _mouseDoubleClickMode = CalenhadGlobeDoubleClickMode::Goto;
    _positionLabel = new QLabel (this);
    layout -> addWidget (_positionLabel, 3, 3, Qt::AlignBottom | Qt::AlignRight);
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

void CalenhadGlobe::resizeEvent (QResizeEvent* e) {
    //Marble::
    QWidget::resizeEvent (e);
    _map -> setSize (width(), height ());
    emit resized (e -> size());
}

void CalenhadGlobe::showContextMenu (const QPoint& pos) {
    if (! _contextMenu) {
        _contextMenu = new CalenhadGlobeContextMenu (this);
        connect (_contextMenu, SIGNAL (showOverviewMap (const bool&)), this, SLOT (showOverviewMap (const bool&)));
        connect (_contextMenu, SIGNAL (showZoomSlider (const bool&)), this, SLOT (showZoomSlider (const bool&)));
        connect (_contextMenu, SIGNAL (setFloatItemVisible (const bool&)), this, SLOT (setFloatItemVisible (const bool&)));
        connect (_contextMenu, SIGNAL (showNavigator (const bool&)), this, SLOT (showNavigator (const bool&)));
        connect (_contextMenu, SIGNAL (showGraticule (const bool&)), this, SLOT (setGraticuleVisible (const bool&)));
        connect (_contextMenu, SIGNAL (dragModeSelected (const CalenhadGlobeDragMode&)), this, SLOT (setMouseDragMode (const CalenhadGlobeDragMode&)));
        connect (_contextMenu, SIGNAL (doubleClickModeSelected (const CalenhadGlobeDoubleClickMode&)), this, SLOT (setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode&)));
        connect (_contextMenu, SIGNAL (projectionSelected (const Projection&)), this, SLOT (setProjection (const Projection&)));
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

// move the view centre along a given azimuth
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
    goTo (GeoDataCoordinates (lon, lat));

}

void CalenhadGlobe::paintEvent (QPaintEvent* e) {
    QWidget::paintEvent (e);
    if (_layer) {
        GeoPainter geoPainter (this, _map->viewport (), _map->mapQuality ());
        _map->paint (geoPainter, e->rect ());
        if (_zoomDrag) {
            QPen pen = QPen (Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
            QBrush brush = QBrush (Qt::NoBrush);
            geoPainter.setPen (pen);
            geoPainter.setBrush (brush);
            geoPainter.drawRect (_zoomBox.center (), RAD2DEG * _zoomBox.width (), RAD2DEG * _zoomBox.height (), true);
        } else {
            _positionLabel->setText (geoLocationString (centre ()));
        }
    }
}

QString CalenhadGlobe::geoLocationString (GeoDataCoordinates pos) {
    QString ns = (pos.latitude () > 0 ? " N " : " S ");
    QString ew = (pos.longitude () > 0 ? " E" : " W");
    if (_coordinatesFormat == CoordinatesFormat::Decimal) {
        return
                QString::number (std::abs (pos.latitude ())) + ns + ", " + QString::number (std::abs (pos.longitude ())) + ew ;
    } else {
        return toTraditional (std::abs (pos.latitude())) + ns + ", " + toTraditional (std::abs (pos.longitude())) + ew;
    }
}

void CalenhadGlobe::mousePressEvent (QMouseEvent* e) {
    double east, south;    if (e->button () == Qt::LeftButton) {
        if (_map->viewport ()->geoCoordinates (e->pos ().x (), e->pos ().y (), east, south)) {
            _moveFrom = e->pos ();
            setCursor (Qt::OpenHandCursor);
        }
    }
}

void CalenhadGlobe::mouseDoubleClickEvent (QMouseEvent* e) {
    double lat, lon;
    if (_mouseDoubleClickMode == CalenhadGlobeDoubleClickMode::Goto) {
        if (_map->viewport ()->geoCoordinates (e->pos ().x (), e->pos ().y (), lon, lat)) {
            goTo (GeoDataCoordinates (lon, lat));
        }
    }
    if (_mouseDoubleClickMode == CalenhadGlobeDoubleClickMode::Place) {
        // to do
    }
}

void CalenhadGlobe::mouseMoveEvent (QMouseEvent* e) {
    double dx = e -> pos().x() - _moveFrom.x();
    double dy = e -> pos().y() - _moveFrom.y();
    double north, south, east, west, temp;
    bool isOnGlobe = _map -> viewport()->geoCoordinates (e -> pos().x(), e -> pos().y(), east, south) && _map -> viewport() -> geoCoordinates (_moveFrom.x(), _moveFrom.y(), west, north);
    if (e -> buttons() & Qt::LeftButton) {
        if (cursor().shape () != Qt::ClosedHandCursor) {
            setCursor (Qt::ClosedHandCursor);
        }
        if (south > north) {
            temp = north;
            north = south;
            south = temp;
        }
        if (_map->viewport ()->viewLatLonAltBox ().width () > M_PI) {
            temp = east;
            east = west;
            west = temp;
        }

        if (_mouseDragMode == CalenhadGlobeDragMode::Pan) {
            double dLat = (180.0 / _map->radius ()) * dy * _sensitivity;
            double dLon = (180.0 / _map->radius ()) * dx * _sensitivity;
            _map->setCenterLatitude (_map->centerLatitude () + dLat);
            _map->setCenterLongitude (_map->centerLongitude () - dLon);
            _moveFrom = e->pos ();

            update ();
        }

        if (_mouseDragMode == CalenhadGlobeDragMode::Zoom) {
            _zoomDrag = true;

            if (isOnGlobe) {
                if (_positionLabel -> isVisible()) {
                    _positionLabel->setText (geoLocationString (GeoDataCoordinates (west, north)) + " - " + geoLocationString (GeoDataCoordinates (east, south)));
                }

                _zoomBox = GeoDataLatLonBox (north, south, east, west, GeoDataCoordinates::Degree);
                update ();
            } else {
                _positionLabel->setText (QString::null);
                _zoomDrag = false;
            }
        }
    } else {
        // If we are not panning or zooming, and if the display of mouse coordinates is not suppressed, show the current mouse position as geolocation coordinates in a tooltip.
        if (isOnGlobe) {
            setCursor (Qt::CrossCursor);
            if  ( _coordinatesFormat != CoordinatesFormat::NoCoordinates) {
                QString tip = "";
                if (_datumFormat != DatumFormat::NoDatum) {
                    tip = ": " + QString::number (sphereModel.GetValue (south, east));
                }
                QToolTip::showText (e->globalPos (), geoLocationString (GeoDataCoordinates (east, south)) + tip, this);
            }
        } else {
            setCursor (Qt::ArrowCursor);
            QToolTip::showText (e -> pos(), "", this);
        }
    }
}

void CalenhadGlobe::mouseReleaseEvent (QMouseEvent* e) {
    _zoomDrag = false;
    if (_mouseDragMode == CalenhadGlobeDragMode::Zoom) {
        if (e->modifiers () & Qt::ControlModifier) {
            zoomOutFrom (_zoomBox);
        } else {
            zoomInTo (_zoomBox);
        }
    }
    double south, east;

    // restore cursor for mouse move without buttons
    if (_map->viewport() -> geoCoordinates (e -> pos().x(), e -> pos().y(), east, south)) {
        setCursor (Qt::CrossCursor);
    } else {
        setCursor (Qt::ArrowCursor);
    }
}

void CalenhadGlobe::setZoom (const double& zoom) {
    double radius = zoomFactorToRadius (zoom);
    _map -> setRadius (radius);
    _zoom = zoom;

}

int CalenhadGlobe::zoomFactorToRadius (const double& zoom) {
    return (int) std::pow (2, zoom / 16) * 100;
}

double CalenhadGlobe::radiusToZoomFactor (const int& radius) {
    return (16 * std::log (radius / 100.0)) / std::log (2);
}

double CalenhadGlobe::zoom() {
    return _zoom;
}

void CalenhadGlobe::changeView() {
    Bounds bounds = Bounds (_map -> viewport() -> viewLatLonAltBox());
    _overview -> setBounds (bounds);
    _overview -> render();
    emit viewChanged (bounds);
    _layer -> rescale();
    _renderTimer.setSingleShot (true);
    _renderTimer.start();
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

void CalenhadGlobe::setMouseDragMode (const CalenhadGlobeDragMode& mouseMode) {
   if (mouseMode != _mouseDragMode) {
        _mouseDragMode = mouseMode;
    }
}

void CalenhadGlobe::setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mouseMode) {
    _mouseDoubleClickMode = mouseMode;
}

const CalenhadGlobeDragMode CalenhadGlobe::dragMode () {
    return _mouseDragMode;
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
    _configDialog -> commitChanges();
    setFloatItemVisible (_configDialog -> scaleCheckState(), "scalebar");
    showOverviewMap (_configDialog -> overviewCheckState ());
    showZoomSlider (_configDialog -> zoomBarCheckState ());
    showNavigator (_configDialog -> compassCheckState());
    setGraticuleVisible (_configDialog -> graticuleCheckState ());
    setMouseDragMode (_configDialog -> dragMode ());
    setMouseDoubleClickMode (_configDialog -> doubleClickMode());
    setSensitivity (_configDialog -> mouseSensitivity());
    setProjection (_configDialog -> selectedProjection());
    _configDialog -> update();
    _contextMenu -> update();
    for (Legend* legend : CalenhadServices::legends() -> all()) {
        legend -> widget () -> updateLegend();
    }
    _source -> setLegend (_configDialog -> selectedLegend());
     update();
    _coordinatesFormat = _configDialog -> coordinatesFormat();
    _datumFormat = _configDialog -> datumFormat();
}

void CalenhadGlobe::setSensitivity (const double& sensitivity) {
    _sensitivity = sensitivity;
}

double CalenhadGlobe::sensitivity() {
    return _sensitivity;
}

const CalenhadGlobeDoubleClickMode CalenhadGlobe::doubleClickMode () {
    return _mouseDoubleClickMode;
}

void CalenhadGlobe::goTo (const GeoDataCoordinates& target) {
    _map->setCenterLatitude (target.latitude());
    _map->setCenterLongitude (target.longitude());
    update ();
}

// Zoom so that the region enclosed in the whole viewport is compressed into the target _box.
void CalenhadGlobe::zoomOutFrom (const GeoDataLatLonBox& target) {
    int newRadius = _map -> radius();
    ViewportParams* v = _map -> viewport();
    GeoDataCoordinates centre = target.center ();

    if(target.height() && target.width()) {
        //work out the needed zoom level

        double nx, sx, wx, ex;
        v -> screenCoordinates (target.west(), target.north(), wx, nx);
        v -> screenCoordinates (target.east(), target.south(), ex, sx);
        int const horizontalRadius = (std::abs (nx - sx) / v -> viewLatLonAltBox ().height()) / ( 0.25 * M_PI );
        int const verticalRadius = (std::abs (ex - wx) / v -> viewLatLonAltBox ().width()) / ( 0.25 * M_PI );
        std::cout << horizontalRadius << " " << verticalRadius << "\n";
        newRadius = qMin<int>(horizontalRadius, verticalRadius );
        newRadius = qMax<int> (100, qMin<int>(newRadius, 1e+08));
    }
    goTo (GeoDataCoordinates (RAD2DEG * target.center().longitude(), RAD2DEG * target.center().latitude()));
    //_map -> setRadius (newRadius);
    _zoomSlider -> setValue (radiusToZoomFactor (newRadius));

}

// Zoom so that the region enclosed in the target _box fills the whole viewport.
void CalenhadGlobe::zoomInTo (const GeoDataLatLonBox& target) {
    if (target.isEmpty()) {
        return;
    }

    int newRadius = _map -> radius();
    ViewportParams* v = _map -> viewport();

    if(target.height() && target.width()) {
        int const horizontalRadius = ( 0.25 * M_PI ) * (v -> height() / target.height());
        int const verticalRadius = ( 0.25 * M_PI ) * (v -> width() / target.width());
        newRadius = qMin<int>(horizontalRadius, verticalRadius );
        newRadius = qMax<int> (100, qMin<int>(newRadius, 1e+08));
    }
    goTo (GeoDataCoordinates (RAD2DEG * target.center().longitude(), RAD2DEG * target.center().latitude()));
    _zoomSlider -> setValue (radiusToZoomFactor (newRadius));
}

GeoDataCoordinates CalenhadGlobe::centre () {
    return GeoDataCoordinates (_map -> centerLongitude (), _map -> centerLatitude ());
}

bool CalenhadGlobe::isInView (const GeoDataCoordinates& coordinates) {
    double x, y;
    bool hidden;
    bool result = _map -> viewport() -> screenCoordinates (coordinates, x, y, hidden);
    return result && ! hidden;
}

void CalenhadGlobe::setGraticuleVisible (const bool& visible) {
    _map -> setShowGrid (visible);
}

bool CalenhadGlobe::isGraticuleVisible() {
     return _map -> showGrid();
}

void CalenhadGlobe::setProjection (const Projection& projection) {
    if (projection != _map -> projection ()) {
        _map -> setProjection (projection);
    }
}

const Projection& CalenhadGlobe::projection () {
    _projection = _map -> projection ();
    return _projection;
}

void CalenhadGlobe::setLegend (Legend* legend) {
    _source -> setLegend (legend);
}

Legend* CalenhadGlobe::legend() {
    return _layer -> legend();
}

CoordinatesFormat CalenhadGlobe::coordinatesFormat () {
    return _coordinatesFormat;
}

DatumFormat CalenhadGlobe::datumFormat () {
    return _datumFormat;
}


/**
 *  dms.h -- C++ functions to convert between decimal degrees
 *           and degrees, minutes, and seconds
 *
 *  Copyright (C) 2005-2008 by James A. Chappell
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */
//=================================================================
/*
 * dms.h:  Version 0.02
 * Created by James A. Chappell
 * Created 23 August 2005
 *
 * History:
 * 23-aug-2005  created
 * 25-apr-2008  added latitude/longitude conversions
 *
 */
//=================================================================


QString CalenhadGlobe::toTraditional (double ang, unsigned int num_dec_places) {
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

QModule* CalenhadGlobe::module() {
    return _source;
}

void CalenhadGlobe::renderingStart () {


}

void CalenhadGlobe::renderingFinished() {

}
