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
    _rect = QRectF (0, 0, 240, 120);
    setAcceptHoverEvents (true);
    setZValue (-1000);
    _rectSizeGripItem = new SizeGripItem (new NodeGroupResizer, this);
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
    polygon = QRectF (_rect);
    p.addPolygon (polygon);
     _label -> setPos (boundingRect().x(), boundingRect().y());
    return p;
}

void NodeGroupBlock::nodeChanged () {
    _label -> setPlainText (_node -> name());
    _node -> invalidate();
}

void NodeGroupBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    NodeBlock::mouseReleaseEvent (event);
    if (parentItem ()) {
        setZValue (parentItem () -> zValue () + 1);
    } else {
        setZValue (-1000);
    }
    _rectSizeGripItem -> setZValue (zValue());
}

void NodeGroupBlock::setHighlight (bool highlighted) {
    _highlighted = highlighted;

}

void NodeGroupBlock::setRect (const QRectF& rect) {
    _rect = rect;
    setPath (makePath());

}

void NodeGroupResizer::operator() (QGraphicsItem* item, const QRectF& rect) {
    NodeGroupBlock* group = dynamic_cast<NodeGroupBlock*> (item);
    if (group) {
        group -> setRect (rect);
    }
}
