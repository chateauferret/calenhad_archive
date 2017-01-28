//
// Created by martin on 16/10/16.
//

#include "qneblockhandle.h"
#include "qnemainwindow.h"
#include "../qmodule/QModule.h"
#include "../preferences.h"

QNEBlockHandle::QNEBlockHandle (QModule* w, QGraphicsItem* parent) : QGraphicsItem (parent), _module (w) {
    setFlag (QGraphicsItem::ItemIsMovable, true);
    setFlag (QGraphicsItem::ItemIsSelectable, true);
    setCursor (Qt::OpenHandCursor);
}

QNEBlockHandle::~QNEBlockHandle () {

}


void QNEBlockHandle::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option)
    QRectF rect = QRectF (0, 0, _module -> width(), 20);
    rect.setWidth (rect.width() - 2);
    rect.moveTo (1, 1);
    QPen pen = QPen();
    QBrush brush = QBrush (QNEMainWindow::preferences -> calenhad_handle_brush_color_bg);

    painter -> setPen (pen);
    painter -> setBrush (brush);
    painter -> drawRect (rect);

    rect.setWidth (rect.width() - 4);
    rect.setHeight (rect.height() - 6);
    rect.moveTo (3, 4);
    brush = QBrush (isSelected() ? QNEMainWindow::preferences -> calenhad_handle_brush_color_selected : QNEMainWindow::preferences -> calenhad_handle_brush_color_normal);
    painter -> setBrush (brush);
    QPainterPath path = QPainterPath();
    path.addRoundedRect (rect, 4, 4);
    painter -> drawPath (path);
    pen = QPen (isSelected() ? QNEMainWindow::preferences -> calenhad_handle_text_color_selected : QNEMainWindow::preferences -> calenhad_handle_text_color_normal);
    painter -> setBrush (brush);
    rect.setWidth (rect.width() - 4);
    rect.moveTo (5, 3);
    painter -> drawText (rect, Qt::AlignLeft, _module -> name());
}

QRectF QNEBlockHandle::boundingRect () const {
    return QRectF (0, 0, _module -> width(), 20);
}

void QNEBlockHandle::mousePressEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent (event);

}

void QNEBlockHandle::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent (event);
}

void QNEBlockHandle::refresh () {
    update();
}

QModule* QNEBlockHandle::module () {
    return _module;
}
