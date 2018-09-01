//
// Created by martin on 27/10/16.
//

#include "CalenhadView.h"
#include "Calenhad.h"
#include "CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "qmodule/NodeGroup.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <CalenhadServices.h>
#include "../preferences/PreferencesService.h"

using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;


CalenhadView::CalenhadView (QWidget* parent) : QGraphicsView (parent) {
    setDragMode (QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode (Qt::ContainsItemShape);
    setZoom (CalenhadServices::preferences() -> calenhad_desktop_zoom_default);
}

CalenhadView::~CalenhadView() {

}

void CalenhadView::setZoom (const qreal& z) {
    qreal factor = z / zoom;
    scale (factor, factor);
    if (scene()) {
        double extent = CalenhadServices::preferences() -> calenhad_model_extent / 2;
        QRectF minRect (-extent / 2, -extent / 2, extent, extent);
        QRectF bounds = scene() -> itemsBoundingRect();
        setSceneRect (minRect.contains (bounds) ? minRect : bounds);
    }
    zoom = z;
    QTransform xf = transform();
    double oldZoom = zoom;
    zoom = (xf.m11 () + xf.m22 ()) / 2;
    emit viewZoomed (oldZoom, zoom);
}

double CalenhadView::currentZoom() {
    return zoom;
}

void CalenhadView::setController (CalenhadController* controller) {
    if (! _controller) {
        _controller = controller;
    }
}

void CalenhadView::dragEnterEvent (QDragEnterEvent *event) {
    if (event -> mimeData() -> hasFormat("application/x-dnditemdata")) {
        if (event -> source() == this) {
            event -> setDropAction(Qt::MoveAction);
            event -> accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event -> ignore();
    }
}

void CalenhadView::dragLeaveEvent (QDragLeaveEvent *event) {

}

void CalenhadView::dragMoveEvent(QDragMoveEvent *event) {
    QPointF pos = mapToScene (event -> pos());
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event -> source() == this) {
            event -> setDropAction(Qt::MoveAction);
        } else {
            event -> acceptProposedAction();
        }
    } else {
        event -> ignore();
    }
    scene() -> update ();
}

void CalenhadView::dropEvent(QDropEvent *event) {


    if (event -> mimeData ()->hasFormat ("application/x-dnditemdata")) {
        QByteArray itemData = event -> mimeData () -> data ("application/x-dnditemdata");
        QDataStream dataStream (&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QString type;
        //dataStream >> pixmap >> type;
        dataStream >> type;

        QPointF pos = mapToScene (event -> pos());
        ((CalenhadModel*) scene ())->doCreateNode (pos, type);

        if (event -> source() == this) {
            event -> setDropAction (Qt::MoveAction);
            event -> accept ();
        } else {
            event -> acceptProposedAction ();
        }
    } else {
        event -> ignore ();
    }
}

void CalenhadView::wheelEvent (QWheelEvent* event) {
    if (event -> angleDelta().y() > 0) {
        emit zoomInRequested();
    } else {
        emit zoomOutRequested();
    };
}
