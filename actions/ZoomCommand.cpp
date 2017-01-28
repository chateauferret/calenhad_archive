//
// Created by martin on 23/01/17.
//

#include "ZoomCommand.h"
#include <QGraphicsItem>

ZoomCommand::ZoomCommand (double delta, CalenhadView* view) {
    _delta = delta;
    _view = view;
    setText (QObject::tr ("Zoom to %1").arg (_previousZoom));
}

ZoomCommand::~ZoomCommand () {

}

void ZoomCommand::undo () {
    double z = _view -> currentZoom();
    _view -> setZoom (_previousZoom);
    _previousZoom = z;
}

void ZoomCommand::redo () {
    _previousZoom = _view -> currentZoom();
    _view -> setZoom (_previousZoom + _delta);
}


ZoomToFitCommand::ZoomToFitCommand (CalenhadView* view) : ZoomCommand (0, view){

}


void ZoomToFitCommand::redo() {
    zoomToFit (_view -> scene() -> items());
}

void ZoomToFitCommand::zoomToFit (QList<QGraphicsItem*> list) {
    QRectF box = QRectF (-20, -20, 20, 20); // minimum _view
    for (QGraphicsItem* item : list) {
        if (item -> type() >= QGraphicsItem::UserType) {
            box = box.united (item -> sceneBoundingRect());
        }
    }
    _previousZoom = _view -> currentZoom();
    _view -> fitInView (box, Qt::KeepAspectRatio);
}


ZoomToSelectionCommand::ZoomToSelectionCommand (CalenhadView* view) : ZoomToFitCommand (view){

}

void ZoomToSelectionCommand::redo() {
    zoomToFit (_view -> scene() -> selectedItems());
}
