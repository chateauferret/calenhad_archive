//
// Created by martin on 06/06/17.
//

#ifndef CALENHAD_QWIDGETGROUP_H
#define CALENHAD_QWIDGETGROUP_H


#include "QNode.h"

class QNodeGroup : public QNode {
public:
    QNodeGroup (QWidget* parent);
    virtual ~QNodeGroup();
    enum { Type = QGraphicsItem::UserType + 5 };
    void setRect  (const QRect& rect);
    QRect rect();
    bool isWithin (const QPoint& point);
    void setUniqueName () override;

protected:
    QRect _rect;
    void addInputPorts() override;

};


#endif //CALENHAD_QWIDGETGROUP_H
