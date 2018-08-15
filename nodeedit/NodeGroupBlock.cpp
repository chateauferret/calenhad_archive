//
// Created by martin on 07/06/17.
//

#include "NodeGroupBlock.h"
#include "../CalenhadServices.h"
#include "EditableLabel.h"
#include "../pipeline/CalenhadModel.h"
#include <QGraphicsSceneMouseEvent>
#include "../preferences/PreferencesService.h"
#include <QPainter>
#include "qmodule/Node.h"

using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::controls;


NodeGroupBlock::NodeGroupBlock (NodeGroup* node, QGraphicsItem* parent) : NodeBlock (node, parent) {
    double w = CalenhadServices::preferences() -> calenhad_desktop_nodegroup_width_default;
    double h = CalenhadServices::preferences() -> calenhad_desktop_nodegroup_height_default;
    _rect = QRectF (0, 0, w, h);
    setAcceptHoverEvents (true);
    setZValue (-1000);
    _sizeGrip = new SizeGripItem (new NodeGroupResizer, this);
    setAcceptDrops (true);
}

NodeGroupBlock::~NodeGroupBlock () {

}


void NodeGroupBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option);
    _brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_nodegroup_brush_color_selected : CalenhadServices::preferences() -> calenhad_nodegroup_brush_color_normal);
    if (_highlighted) {
        _brush.setColor (_brush.color().lighter());
    }

    painter -> setBrush (_brush);
    _pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_nodegroup_text_color_selected : CalenhadServices::preferences() -> calenhad_nodegroup_text_color_normal);
    painter -> setPen (_pen);
    painter -> drawPath (path());

}

QRectF NodeGroupBlock::boundingRect () const {
    return _rect;
}

QPainterPath NodeGroupBlock::makePath() {

    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (boundingRect());
    p.addPolygon (polygon);
     _label -> setPos (boundingRect().x(), boundingRect().y());
    return p;
}

void NodeGroupBlock::nodeChanged () {
    _label -> setPlainText (_node -> name());
    _node -> invalidate();
}


void NodeGroupBlock::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {

    // override this because we don't want a group to hide other groups it contains whilst it's being dragged about

    QGraphicsItem::mouseMoveEvent (event);
    if (event->buttons() | Qt::LeftButton) {
        setCursor (Qt::ClosedHandCursor);


    } else {
        setCursor (Qt::OpenHandCursor);
    }
}


void NodeGroupBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    node() -> assignGroup();
    NodeBlock::mouseReleaseEvent (event);
    if (node() -> group()) {
        setZValue (node() -> group() -> handle() -> zValue () + 1);
    } else {
        setZValue (-1000);
    }
    _sizeGrip -> setZValue (zValue());

}

void NodeGroupBlock::setHighlight (bool highlighted) {
    _highlighted = highlighted;

}

void NodeGroupBlock::setRect (const QRectF& rect) {
    _rect = rect;
    setPath (makePath());
    node() -> assignGroup();

}

void NodeGroupBlock::resize (const QRectF& rect) {
    QRectF r = mapFromScene (rect).boundingRect();
    _sizeGrip -> setLeft (r.left());
    _sizeGrip -> setRight (r.right());
    _sizeGrip -> setTop (r.top());
    _sizeGrip -> setBottom (r.bottom());
}

QRectF NodeGroupBlock::rect() {
    return _rect;
}



void NodeGroupResizer::operator() (QGraphicsItem* item, const QRectF& rect) {
    NodeGroupBlock* block = dynamic_cast<NodeGroupBlock*> (item);
    if (block) {
        block -> setRect (rect);
    }
}


