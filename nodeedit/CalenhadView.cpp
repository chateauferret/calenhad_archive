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
    setDragMode (QGraphicsView::ScrollHandDrag);
    setRubberBandSelectionMode (Qt::ContainsItemShape);
    setRenderHints (QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setZoom (CalenhadServices::preferences() -> calenhad_desktop_zoom_default);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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
    QGraphicsView::drawBackground (painter, rect);
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
        int steps = CalenhadServices::preferences() -> calenhad_desktop_grid_major_steps;
        int offset = gridSize * (steps + 1);
        qreal minorLeft = (int (r.left ()) - (int (r.left ())) % offset) - offset;
        qreal minorTop = (int (r.top ()) - (int (r.top ())) % offset) - offset;

        QVector<QLineF> minorLines, majorLines;

        painter->setPen (minorPen);
        int i = 0, j = 0;
        for (qreal x = minorLeft; x < r.right (); x += gridSize) {
            QLineF l (x, r.top (), x, r.bottom ());
            minorLines.append (l);
            if (i++ == steps) {
                i = 0;
                majorLines.append (l);
            }
        }
        for (qreal y = minorTop; y < r.bottom (); y += gridSize) {
            QLineF  l (r.left (), y, r.right (), y);
            minorLines.append (l);
            if (j++ == steps) {
                j = 0;
                majorLines.append (l);
            }
        }
        painter->drawLines (minorLines.data (), minorLines.size ());
        painter->setPen (majorPen);
        painter->drawLines (majorLines.data (), majorLines.size ());

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