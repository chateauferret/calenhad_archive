//
// Created by martin on 27/10/16.
//

#include "componentproxywidget.h"
#include <QGraphicsScene>
#include <QWidget>
#include <iostream>

ComponentProxyWidget::ComponentProxyWidget (QGraphicsItem* parent) : QGraphicsProxyWidget (parent) {
    setFlag (QGraphicsItem::ItemIsMovable, true);
    setFlag (QGraphicsItem::ItemIsSelectable, true);
}

ComponentProxyWidget::~ComponentProxyWidget () {

}

void ComponentProxyWidget::addPort (QNEPort* port) {
    int horzMargin = 20;
    int vertMargin = 25;
    QFontMetrics fm (scene() -> font());
    int yInput = port -> radius();
    int yOutput = yInput;
    int w = fm.width (port -> portName());
    int h = fm.height();

    foreach (QGraphicsItem* port_, childItems()) {
        if (port_ -> type() == QNEPort::Type) {
            QNEPort* p = (QNEPort*) port_;
            if (p -> portType() == QNEPort::OutputPort) {
                p -> setPos (widget () -> width () + p -> radius (), yOutput + vertMargin);
                yOutput += h;
            } else {
                p -> setPos (-p -> radius (), yInput + vertMargin);
                yInput += h;
            }
        }
    }
}

QRectF ComponentProxyWidget::boundingRect() const {
    QRectF r = widget () -> rect();
    foreach (QGraphicsItem* port_, childItems ()) {
        if (port_ -> type() == QNEPort::Type) {
            r = r.united (port_ -> boundingRect());
        }
    }
    return r;
}
