//
// Created by martin on 07/06/17.
//

#ifndef CALENHAD_QNODEGROUPBLOCK_H
#define CALENHAD_QNODEGROUPBLOCK_H


#include "QNodeBlock.h"

enum NodeGroupHandle { TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight, NoHandle };

class QNodeGroupBlock : public QNodeBlock {
Q_OBJECT

public:
    enum {
        Type = QGraphicsItem::UserType + 8
    };

    QNodeGroupBlock (QNode* node, QGraphicsItem* parent = 0);

    virtual ~QNodeGroupBlock ();

    void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    virtual QRectF boundingRect () const;

    void mouseMoveEvent (QGraphicsSceneMouseEvent* e) override;
    void mouseReleaseEvent (QGraphicsSceneMouseEvent* event) override;
    void hoverMoveEvent (QGraphicsSceneHoverEvent* e) override;

    void setHighlight (bool highlighted);

public slots:

    void nodeChanged () override;

protected:
    QPainterPath makePath ();

    QRectF _rect;

    NodeGroupHandle getNodeGroupHandle (QPointF pos);

    qreal _margin;

    void dropEvent (QGraphicsSceneDragDropEvent* event);



    bool _highlighted;
};


#endif //CALENHAD_QNODEGROUPBLOCK_H
