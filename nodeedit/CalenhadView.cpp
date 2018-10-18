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

void CalenhadView::drawBackground(QPainter *painter, const QRectF &rect) {
    // thanks to Bitto at QtCentre for this - https://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene

    if (gridVisible()) {

        QPen majorPen;
        QPen minorPen;
        majorPen.setColor (CalenhadServices::preferences ()->calenhad_desktop_grid_major_color);
        majorPen.setStyle (Qt::PenStyle (CalenhadServices::preferences ()->calenhad_desktop_grid_major_style));
        majorPen.setWidth (CalenhadServices::preferences ()->calenhad_desktop_grid_major_weight);
        minorPen.setColor (CalenhadServices::preferences ()->calenhad_desktop_grid_minor_color);
        minorPen.setStyle (Qt::PenStyle (CalenhadServices::preferences ()->calenhad_desktop_grid_minor_style));
        minorPen.setWidth (CalenhadServices::preferences ()->calenhad_desktop_grid_minor_weight);
        QRectF r = mapToScene (viewport ()->geometry ()).boundingRect ();
        int gridSize = CalenhadServices::preferences ()->calenhad_desktop_grid_density;
        qreal left = int (r.left ()) - (int (r.left ()) % gridSize);
        qreal top = int (r.top ()) - (int (r.top ()) % gridSize);

        QVector<QLineF> lines;

        painter->setPen (minorPen);
        for (qreal x = left; x < r.right (); x += gridSize)
            lines.append (QLineF (x, r.top (), x, r.bottom ()));
        for (qreal y = top; y < r.bottom (); y += gridSize)
            lines.append (QLineF (r.left (), y, r.right (), y));
        painter->drawLines (lines.data (), lines.size ());

        if (CalenhadServices::preferences() -> calenhad_desktop_grid_major_steps > 1) {
            lines.clear ();
            gridSize *= CalenhadServices::preferences() -> calenhad_desktop_grid_major_steps;
            painter->setPen (majorPen);
            for (qreal x = left; x < r.right (); x += gridSize)
                lines.append (QLineF (x, r.top (), x, r.bottom ()));
            for (qreal y = top; y < r.bottom (); y += gridSize)
                lines.append (QLineF (r.left (), y, r.right (), y));

            painter->drawLines (lines.data (), lines.size ());
        }
    }
}

void CalenhadView::setGridVisible (const bool& visible) {
    CalenhadServices::preferences() -> calenhad_desktop_grid_visible = visible;
    update();
}

bool CalenhadView::gridVisible() {
    return CalenhadServices::preferences() -> calenhad_desktop_grid_visible;
}


void CalenhadView::setSnapToGrid (const bool& enabled) {
    CalenhadServices::preferences() -> calenhad_desktop_grid_snap = enabled;
    update();
}

bool CalenhadView::snapToGrid() {
    return CalenhadServices::preferences() -> calenhad_desktop_grid_snap;
}