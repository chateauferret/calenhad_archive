//
// Created by martin on 07/06/17.
//

#include "QNodeGroupBlock.h"
#include "../CalenhadServices.h"
#include "EditableLabel.h"

QNodeGroupBlock::QNodeGroupBlock (QNode* node, QGraphicsItem* parent) : QModuleBlock (node, parent) {
    _rect = QRectF (0, 0, 32, 32);
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
    return QModuleBlock::boundingRect ();
}

QPainterPath QNodeGroupBlock::makePath () {
    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (_rect);
    p.addPolygon (polygon);

    return p;
}

void QNodeGroupBlock::nodeChanged () {
    _label -> setPlainText (_node -> name());
    _label -> setPos (32 - (_label -> boundingRect().width() / 2 ), 38);
    _node -> invalidate();
}
