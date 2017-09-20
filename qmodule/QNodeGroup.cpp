//
// Created by martin on 06/06/17.
//

#include "QNodeGroup.h"
#include "../pipeline/CalenhadModel.h"

using namespace calenhad::qmodule;

QNodeGroup::QNodeGroup (QWidget* parent) : QNode (parent), _rect (QRect (0, 0, 100, 100)) {

}

QNodeGroup::~QNodeGroup () {

}

void QNodeGroup::setRect (const QRect& rect) {
    _rect = rect;
}

QRect QNodeGroup::rect () {
    return _rect;
}

bool QNodeGroup::isWithin (const QPoint& point) {
    return _rect.contains (point);
}


void QNodeGroup::addInputPorts () {
    // do nothing for now - groups don't have ports
}

QString QNodeGroup::nodeType () {
    return "NodeType";
}


void QNodeGroup::initialise() {
    QNode::initialise();
}

QNodeGroup* QNodeGroup::clone () {
    QNodeGroup* other = new QNodeGroup ((QWidget*) parent());
    other -> setRect (_rect);
    QString n = "Copy_of_" + _name;
    int i = 0;
    QString e;
    setName (n);
    return other;
}
