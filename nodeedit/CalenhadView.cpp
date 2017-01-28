//
// Created by martin on 27/10/16.
//

#include "CalenhadView.h"
#include "qnemainwindow.h"
#include "CalenhadController.h"
#include <QGraphicsView>
#include <QWidget>
#include <QAction>
#include <QTimeLine>
#include <QToolBar>
#include <QUndoStack>
#include "qnetoolbox.h"
#include "qneconnection.h"

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
