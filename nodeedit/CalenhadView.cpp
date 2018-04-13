//
// Created by martin on 27/10/16.
//

#include "CalenhadView.h"
#include "Calenhad.h"
#include "CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "qmodule/NodeGroup.h"
#include "NodeGroupBlock.h"
#include <QDragEnterEvent>
#include <QMimeData>
using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;


CalenhadView::CalenhadView (QWidget* parent) : QGraphicsView (parent) {
    setDragMode (QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode (Qt::ContainsItemShape);
    setZoom (1.0);
}

CalenhadView::~CalenhadView() {

}

void CalenhadView::setZoom (const qreal& z) {
    qreal factor = z / zoom;
    scale (factor, factor);
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
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}


void CalenhadView::dragLeaveEvent (QDragLeaveEvent *event) {
    for (QGraphicsItem* item : scene ()->items ()) {
        if (dynamic_cast<NodeGroupBlock*> (item)) {
            ((NodeGroupBlock*) item)->setHighlight (false);
        }
    }
}

void CalenhadView::dragMoveEvent(QDragMoveEvent *event) {
    QPointF pos = mapToScene (event -> pos());
    ((CalenhadModel*) scene()) -> highlightGroupAt (pos);
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event -> source() == this) {
            event -> setDropAction(Qt::MoveAction);
        } else {
            event -> acceptProposedAction();
        }
    } else {
        event -> ignore();
    }
    scene()->update ();

}

void CalenhadView::dropEvent(QDropEvent *event) {

    for (QGraphicsItem* item : scene ()->items ()) {
        if (dynamic_cast<NodeGroupBlock*> (item)) {
            ((NodeGroupBlock*) item)->setHighlight (false);
        }
    }

    if (event->mimeData ()->hasFormat ("application/x-dnditemdata")) {
        QByteArray itemData = event -> mimeData () -> data ("application/x-dnditemdata");
        QDataStream dataStream (&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QString type;
        //dataStream >> pixmap >> type;
        dataStream >> type;

        QPointF pos = mapToScene (event -> pos());
        ((CalenhadModel*) scene()) -> createNode (pos, type);

        if (event -> source () == this) {
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
        emit zoomInRequested ();
    } else {
        emit zoomOutRequested();
    };
}
