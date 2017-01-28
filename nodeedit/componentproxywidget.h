//
// Created by martin on 27/10/16.
//

#ifndef CALENHAD_COMPONENTPROXYWIDGET_H
#define CALENHAD_COMPONENTPROXYWIDGET_H


#include <QtWidgets/QGraphicsProxyWidget>
#include "qneport.h"
#include "../libnoise/module/modulebase.h"
class QNEPort;
class ComponentProxyWidget : public QGraphicsProxyWidget {
    Q_OBJECT
public:
    enum { Type = QGraphicsItem::UserType + 4 };
    ComponentProxyWidget (QGraphicsItem* parent = 0);
    virtual ~ComponentProxyWidget();
    void addPort (QNEPort* port);
    virtual QRectF boundingRect() const;
    int type() const { return Type; }
};


#endif //CALENHAD_COMPONENTPROXYWIDGET_H
