//
// Created by martin on 07/06/17.
//

#include "QNodeGroupBlock.h"
#include "../CalenhadServices.h"
#include "EditableLabel.h"
#include "../pipeline/CalenhadModel.h"
#include <QGraphicsSceneMouseEvent>
#include "../preferences/PreferencesService.h"
#include <QPainter>
#include "../qmodule/QNode.h"

using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::controls;


QNodeGroupBlock::QNodeGroupBlock (QNodeGroup* node, QGraphicsItem* parent) : QNodeBlock (node, parent) {
    _rect = QRectF (0, 0, 240, 120);
    setAcceptHoverEvents (true);
    setAcceptDrops (true);
    setZValue (-1000);
    SizeGripItem* rectSizeGripItem = new SizeGripItem (new NodeGroupResizer, this);
    setAcceptDrops (true);
}

QNodeGroupBlock::~QNodeGroupBlock () {

}


void QNodeGroupBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
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

QRectF QNodeGroupBlock::boundingRect () const {
    return _rect;
}

QPainterPath QNodeGroupBlock::makePath() {

    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (_rect);
    p.addPolygon (polygon);
     _label -> setPos (boundingRect().x(), boundingRect().y());
    return p;
}

void QNodeGroupBlock::nodeChanged () {
    _label -> setPlainText (_node -> name());
    _node -> invalidate();
}

void QNodeGroupBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    QNodeBlock::mouseReleaseEvent (event);
    if (parentItem ()) {
        setZValue (parentItem () -> zValue () + 1);
    } else {
        setZValue (-1000);
    }
}

void QNodeGroupBlock::setHighlight (bool highlighted) {
    _highlighted = highlighted;

}

void QNodeGroupBlock::setRect (const QRectF& rect) {
    _rect = rect;
    setPath (makePath());

}
