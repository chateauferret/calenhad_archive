//
// Created by martin on 02/05/17.

#include <CalenhadServices.h>
#include "nodeedit/Calenhad.h"
#include "../../qmodule/QModule.h"
#include "../../legend/Legend.h"
#include "../../pipeline/ImageRenderJob.h"
#include "../../graph/graph.h"
#include "../../pipeline/CalenhadModel.h"
#include "../../mapping/projection/ProjectionService.h"
#include <QMouseEvent>
#include <QtXml/QtXml>
#include <QtWidgets/QToolTip>
#include <qwt/qwt_scale_draw.h>
#include "../../mapping/projection/Projection.h"
#include "../../mapping/CalenhadMapWidget.h"


using namespace icosphere;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace calenhad::graph;
using namespace calenhad;
using namespace calenhad::mapping::projection;
using namespace geoutils;
using namespace GeographicLib;

CalenhadMapView::CalenhadMapView (QWidget* parent) : CalenhadMapWidget (parent),
    _datumFormat (DatumFormat::Scaled),
    _zoomDrag (false),
    _sensitivity (0.5),
    _mouseDoubleClickMode (CalenhadGlobeDoubleClickMode::Goto),
    _bounds (Bounds (-M_PI_2, M_PI_2, 0, M_2_PI)),
    _source (nullptr), _previewType (OverviewPreviewType::WholeWorld),
    _geodesic (new Geodesic (1, 0)){
    setMouseTracking (true);

}

CalenhadMapView::~CalenhadMapView() {
    delete _geodesic;
}

void CalenhadMapView::render() {
    if (_source -> isComplete()) {
        QDomDocument doc = _source -> model() -> serialize (calenhad::nodeedit::CalenhadFileType::CalenhadModelFile);
        Graph* g = new Graph (doc, _source -> name ());
        setGraph (g);
    } else {
    }
}

QModule* CalenhadMapView::source() {
    return _source;
}

void CalenhadMapView::setSource (QModule* qm) {
    _source = qm;
    connect (qm, &QNode::nodeChanged, this, &CalenhadMapView::render);
    render();
}

void CalenhadMapView::showEvent (QShowEvent* e) {
    render();
}

// Zoom so that the region enclosed in the whole viewport is compressed into the target _box.
void CalenhadMapView::zoomOutFrom (const Bounds& target) {
    /*double newRadius = _bounds.width ();
    Geolocation centre = target.center ();

    if(target.height() && target.width()) {
        //work out the needed scale level
        QPointF nw, se;
        double nx, sx, wx, ex;
        screenCoordinates (Geolocation (target.north(), target.west()), nw);
        screenCoordinates (Geolocation (target.east(), target.south()), se);
        double const horizontalRadius = (std::abs (nw.x() - se.x()) / _bounds.height()) / ( 0.25 * M_PI );
        double const verticalRadius = (std::abs (ex - wx) / _bounds.width()) / ( 0.25 * M_PI );
        std::cout << horizontalRadius << " " << verticalRadius << "\n";
        newRadius = qMin<double>(horizontalRadius, verticalRadius );
        newRadius = qMax<double> (100, qMin<double>(newRadius, 1e+08));
    }
    goTo (target.center()); // in radians
    _zoomSlider -> setValue (radiusToZoomFactor (newRadius));
    */
}

// Zoom so that the region enclosed in the target _box fills the whole viewport.
void CalenhadMapView::zoomInTo (const Bounds& target) {
    /*double newRadius = std::max (_bounds.width (), _bounds.height ());
    if(target.height() && target.width()) {
        double const horizontalRadius = ( 0.25 * M_PI ) * (_bounds.height() / target.height());
        double const verticalRadius = ( 0.25 * M_PI ) * (_bounds.width() / target.width());
        newRadius = qMin<double>(horizontalRadius, verticalRadius );
        newRadius = qMax<double> (100, qMin<double>(newRadius, 1e+08));
    }
    goTo (target.center()); // in radians
    _zoomSlider -> setValue (radiusToZoomFactor (newRadius)); */
}

void CalenhadMapView::setDatumFormat (DatumFormat format) {
    _datumFormat = format;
}

void CalenhadMapView::setSensitivity (double sensitivity) {
    _sensitivity = sensitivity;
}

DatumFormat CalenhadMapView::datumFormat () {
    return _datumFormat;
}

CoordinatesFormat CalenhadMapView::coordinatesFormat () {
    return _coordinatesFormat;
}

double CalenhadMapView::sensitivity () {
    return _sensitivity;
}

void CalenhadMapView::goTo (const Geolocation& geolocation) {
    rotate (geolocation);
}

void CalenhadMapView::mousePressEvent (QMouseEvent* e) {
    Geolocation loc;
    if (e -> button () == Qt::LeftButton) {
        if (geoCoordinates (e -> pos(), loc)) {
            _moveFrom = e -> pos ();
            setCursor (Qt::OpenHandCursor);
        }
    }
}

void CalenhadMapView::mouseDoubleClickEvent (QMouseEvent* e) {
    if (_mouseDoubleClickMode == CalenhadGlobeDoubleClickMode::Goto) {
        Geolocation loc;
        if (geoCoordinates (e -> pos(), loc)) {
            goTo (loc);
        }
    }
    if (_mouseDoubleClickMode == CalenhadGlobeDoubleClickMode::Place) {
        // to do
    }
}

void CalenhadMapView::mouseMoveEvent (QMouseEvent* e) {
    Geolocation se, nw;
    bool isOnGlobe = true; // for now
    double south = se.latitude (Units::Degrees);
    double north = nw.latitude (Units::Degrees);
    double east = se.longitude (Units::Degrees);
    double west = nw.longitude (Units::Degrees);
    double temp;
    if (e -> buttons() & Qt::LeftButton) {
        if (cursor().shape () != Qt::ClosedHandCursor) {
            setCursor (Qt::ClosedHandCursor);
        }

        double dx = e -> pos().x() - _moveFrom.x();
        double dy = e -> pos().y() - _moveFrom.y();

        if (_mouseDragMode == CalenhadGlobeDragMode::Pan) {
            double dLon = 180.0 * _scale * _sensitivity * dx / 50;
            double dLat = 180.0 * _scale * _sensitivity * dy / 50;
            _moveFrom = e -> pos();
            goTo (Geolocation (_rotation.latitude() + qDegreesToRadians (dLat), _rotation.longitude() - qDegreesToRadians (dLon)));
        }

        if (_mouseDragMode == CalenhadGlobeDragMode::Zoom) {
            double dz = dy * _sensitivity / 10;
            emit zoomRequested (_scale + dz);
            _moveFrom = e -> pos();
            update();
        }
    } else {
        setCursor (Qt::CrossCursor);
        if  ( _coordinatesFormat != CoordinatesFormat::NoCoordinates) {
            Geolocation loc;
            QPointF point;
            point.setX ((double) e -> pos().x());
            point.setY ((double) e -> pos().y());

            if (geoCoordinates (point, loc)) {
                QString text = geoutils::Math::geoLocationString (loc, _coordinatesFormat);
                QToolTip::showText (e -> globalPos(), text, this);
            }
        }
    }
}

void CalenhadMapView::wheelEvent (QWheelEvent* event) {
    double dz =  - event -> delta() * _sensitivity / 1200;
    emit zoomRequested (_scale + dz);
    update();
}

void CalenhadMapView::mouseReleaseEvent (QMouseEvent* e) {
    _zoomDrag = false;
    if (_mouseDragMode == CalenhadGlobeDragMode::Zoom) {
        if (e->modifiers () & Qt::ControlModifier) {
            zoomOutFrom (_zoomBox);
        } else {
            zoomInTo (_zoomBox);
        }
    }
    Geolocation loc;

    // rollback cursor for mouse move without buttons
    if (geoCoordinates (e -> pos(), loc)) {
        setCursor (Qt::CrossCursor);
    } else {
        setCursor (Qt::ArrowCursor);
    }
}


void CalenhadMapView::setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mode) {
    _mouseDoubleClickMode = mode;
}

void CalenhadMapView::setMouseDragMode (const CalenhadGlobeDragMode& mode) {
    _mouseDragMode = mode;
}

CalenhadGlobeDoubleClickMode CalenhadMapView::mouseDoubleClickMode () {
    return _mouseDoubleClickMode;
}

CalenhadGlobeDragMode CalenhadMapView::mouseDragMode () {
    return _mouseDragMode;
}

// move the view centre along a given azimuth
void CalenhadMapView::navigate (const NavigationEvent& e) {
    // move the viewport centre in the chosen direction by the distance multiplied by the current scale
    double lat, lon;
    double distance = e.distance() * scale();
    _geodesic -> Direct (rotation().latitude (Units::Degrees), rotation().longitude (Units::Degrees), e.azimuth(), distance, lat, lon);
    goTo (Geolocation (lat, lon, Units::Degrees));
}