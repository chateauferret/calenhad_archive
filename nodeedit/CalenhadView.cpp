//
// Created by martin on 27/10/16.
//

#include "CalenhadView.h"
#include "Calenhad.h"
#include "CalenhadController.h"
#include "../pipeline/CalenhadModel.h"

CalenhadView::CalenhadView (QWidget* parent) : QGraphicsView (parent) {
    setDragMode (QGraphicsView::RubberBandDrag);
    setRubberBandSelectionMode (Qt::IntersectsItemShape);
    //setContextMenuPolicy(Qt::CustomContextMenu);
    setZoom (1.0);
}

CalenhadView::~CalenhadView() {

}

void CalenhadView::setZoom (const qreal& z) {
    qreal factor = z / zoom;
    scale (factor, factor);
    zoom = z;
    QTransform xf = transform ();
    double oldZoom = zoom;
    zoom = (xf.m11 () + xf.m22 ()) / 2;
}

double CalenhadView::currentZoom() {
    return zoom;
}

void CalenhadView::setController (CalenhadController* controller) {
    if (! _controller) {
        _controller = controller;
    } else {
        std::cout << "Cannot reassign controller";
    }
}

void CalenhadView::dragEnterEvent(QDragEnterEvent *event) {
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

void CalenhadView::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();

            QPointF pos = mapToScene (event -> pos());
            ((CalenhadModel*) scene()) -> highlightTargetAt (pos);

        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void CalenhadView::dropEvent(QDropEvent *event) {
    if (event->mimeData ()->hasFormat ("application/x-dnditemdata")) {
        QByteArray itemData = event -> mimeData () -> data ("application/x-dnditemdata");
        QDataStream dataStream (&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QString type;
        //dataStream >> pixmap >> type;
        dataStream >> type;
        std::cout << "Type " << type.toStdString () << "\n";
        //QLabel* newIcon = new QLabel (this);
        //newIcon -> setPixmap (pixmap);
        //newIcon -> move (event -> pos ());
        //newIcon -> show ();
        //newIcon -> setAttribute (Qt::WA_DeleteOnClose);

        QPointF pos = mapToScene (event -> pos());
        ((CalenhadModel*) scene()) -> addNode (pos, type);

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