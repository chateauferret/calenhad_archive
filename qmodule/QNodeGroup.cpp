//
// Created by martin on 06/06/17.
//

#include "QNodeGroup.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/QNodeGroupBlock.h"

using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;

QNodeGroup::QNodeGroup (QWidget* parent) : QNode (parent), _rect (QRect (0, 0, 100, 100)) {

}

QNodeGroup::~QNodeGroup() {

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

QNodeBlock* QNodeGroup::makeHandle() {
    QNodeGroupBlock* b = new QNodeGroupBlock (this);
    _handle = b;
    b -> initialise ();
    return _handle;
}