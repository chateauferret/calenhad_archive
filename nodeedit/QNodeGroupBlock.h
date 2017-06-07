//
// Created by martin on 07/06/17.
//

#ifndef CALENHAD_QNODEGROUPBLOCK_H
#define CALENHAD_QNODEGROUPBLOCK_H


#include "QModuleBlock.h"



class QNodeGroupBlock : public QModuleBlock {
    Q_OBJECT
public:
    enum { Type = QGraphicsItem::UserType + 8 };
    QNodeGroupBlock (QNode* node, QGraphicsItem *parent = 0);
    virtual ~QNodeGroupBlock();
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const;

    public slots:
    void nodeChanged() override;

protected:
    QPainterPath makePath();

    QRectF _rect;
};


#endif //CALENHAD_QNODEGROUPBLOCK_H
