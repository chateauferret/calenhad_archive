//
// Created by martin on 07/06/17.
//

#include "QNodeGroupBlock.h"
#include "../CalenhadServices.h"
#include "EditableLabel.h"
#include <QGraphicsSceneMouseEvent>

QNodeGroupBlock::QNodeGroupBlock (QNode* node, QGraphicsItem* parent) : QNodeBlock (node, parent), _margin (6.0) {
    _rect = QRectF (0, 0, 240, 120);
    setAcceptHoverEvents (true);
    setZValue (-1000);

}

QNodeGroupBlock::~QNodeGroupBlock () {

}


void QNodeGroupBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option);
    _brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_nodegroup_brush_color_selected : CalenhadServices::preferences() -> calenhad_nodegroup_brush_color_normal);
    painter -> setBrush (_brush);
    _pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_nodegroup_text_color_selected : CalenhadServices::preferences() -> calenhad_nodegroup_text_color_normal);
    painter -> setPen (_pen);
    painter -> drawPath (path());

}


QRectF QNodeGroupBlock::boundingRect () const {
    return QNodeBlock::boundingRect ();
}

QPainterPath QNodeGroupBlock::makePath() {
    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (_rect);
    p.addPolygon (polygon);
    _label -> setPos (4, 4);
    return p;
}

void QNodeGroupBlock::nodeChanged () {
    _label -> setPlainText (_node -> name());
    _node -> invalidate();
}

void QNodeGroupBlock::mouseMoveEvent (QGraphicsSceneMouseEvent* e) {
    NodeGroupHandle handle = getNodeGroupHandle (e -> pos());
    prepareGeometryChange ();
    if (handle != NodeGroupHandle::NoHandle) {
        if (handle == NodeGroupHandle::TopLeft || handle == NodeGroupHandle::Top || handle == NodeGroupHandle::TopRight) { _rect.setTop (e -> pos().y()); }
        if (handle == NodeGroupHandle::BottomLeft || handle == NodeGroupHandle::Bottom || handle == NodeGroupHandle::BottomRight) { _rect.setBottom (e -> pos().y()); }
        if (handle == NodeGroupHandle::TopLeft || handle == NodeGroupHandle::Left || handle == NodeGroupHandle::BottomLeft) { _rect.setLeft (e -> pos().x()); }
        if (handle == NodeGroupHandle::TopRight || handle == NodeGroupHandle::Right || handle == NodeGroupHandle::BottomRight) { _rect.setRight (e -> pos().x()); }
        setPath (makePath());
        scene() -> update();
    } else {
        QGraphicsPathItem::mouseMoveEvent (e);
    }
}

void QNodeGroupBlock::hoverMoveEvent (QGraphicsSceneHoverEvent* e) {
    NodeGroupHandle handle = getNodeGroupHandle (e -> pos());
    if (handle != NodeGroupHandle::NoHandle) {
        if (handle == NodeGroupHandle::TopLeft ||  handle == NodeGroupHandle::BottomRight) { setCursor (Qt::SizeFDiagCursor); }
        if (handle == NodeGroupHandle::Top || handle == NodeGroupHandle::Bottom) { setCursor (Qt::SizeVerCursor); }
        if (handle == NodeGroupHandle::TopRight || handle == NodeGroupHandle::BottomLeft) { setCursor (Qt::SizeBDiagCursor); }
        if (handle == NodeGroupHandle::Left || handle == NodeGroupHandle::Right) {setCursor (Qt::SizeHorCursor); }
    } else {
        setCursor (Qt::OpenHandCursor);
    }
}

NodeGroupHandle QNodeGroupBlock::getNodeGroupHandle (QPointF pos) {
    if (pos.y() < _margin) {
        if (pos.x() < _margin) { return NodeGroupHandle::TopLeft; }
        if (pos.x() > _rect.width() - _margin) { return NodeGroupHandle::TopRight; }
        return NodeGroupHandle::Top;
    }
    if (pos.y() > _rect.height() - _margin) {
        if (pos.x() < _margin) { return NodeGroupHandle::BottomLeft; }
        if (pos.x() > _rect.width() - _margin) { return NodeGroupHandle::BottomRight; }
        return NodeGroupHandle::Bottom;
    }
    if (pos.x() < _margin) { return NodeGroupHandle::Left; }
    if (pos.x() > _rect.width() - _margin) { return NodeGroupHandle::Right; }
    return NodeGroupHandle::NoHandle;
}

