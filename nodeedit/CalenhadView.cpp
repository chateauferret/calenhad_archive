//
// Created by martin on 27/10/16.
//

#include "CalenhadView.h"
#include "Calenhad.h"
#include "CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "module/NodeGroup.h"
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

}

CalenhadView::~CalenhadView() = default;

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

void CalenhadView::dragMoveEvent(QDragMoveEvent *event) {
    //QPointF pos = mapToScene (event -> pos());
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
    }
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

void CalenhadView::setModel (CalenhadModel* model) {
    setScene (model);
    connect (model, &QGraphicsScene::changed, this, &CalenhadView::modelChanged);
}


// Whnever the model changes we resize the canvas to its new bounds so that the view pans appropriately.
// This code comes from apalomer on StackOverflow 7 March 2019 (adapted for house style)
// https://stackoverflow.com/questions/55007339/allow-qgraphicsview-to-move-outside-scene

void CalenhadView::modelChanged() {

    // Widget viewport recangle
    QRectF rectInScene (mapToScene (-20, -20), mapToScene (rect().bottomRight() + QPoint(20, 20)));

    // Copy the new size from the old one
    QPointF newTopLeft (sceneRect().topLeft());
    QPointF newBottomRight (sceneRect().bottomRight());

    // Check that the scene has a bigger limit in the top side
    if (sceneRect().top() > rectInScene.top()) {
        newTopLeft.setY (rectInScene.top());
    }

    // Check that the scene has a bigger limit in the bottom side
    if (sceneRect().bottom() < rectInScene.bottom()) {
        newBottomRight.setY (rectInScene.bottom());
    }

    // Check that the scene has a bigger limit in the left side
    if (sceneRect().left() > rectInScene.left()) {
        newTopLeft.setX (rectInScene.left());
    }

    // Check that the scene has a bigger limit in the right side
    if (sceneRect().right() < rectInScene.right()) {
        newBottomRight.setX (rectInScene.right());
    }

    // Set new scene size
    setSceneRect (QRectF(newTopLeft, newBottomRight));
}