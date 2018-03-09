//
// Created by martin on 06/06/17.
//

#include "NodeGroup.h"
#include "../pipeline/CalenhadModel.h"
#include "nodeedit/NodeGroupBlock.h"

using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;

NodeGroup::NodeGroup (QWidget* parent) : Node (parent), _rect (QRect (0, 0, 100, 100)) {

}

NodeGroup::~NodeGroup() {

}

void NodeGroup::setRect (const QRect& rect) {
    _rect = rect;
}

QRect NodeGroup::rect () {
    return _rect;
}

bool NodeGroup::isWithin (const QPoint& point) {
    return _rect.contains (point);
}


void NodeGroup::addInputPorts () {
    // do nothing for now - groups don't have ports
}

QString NodeGroup::nodeType () {
    return "NodeType";
}


void NodeGroup::initialise() {
    Node::initialise();
}

NodeBlock* NodeGroup::makeHandle() {
    NodeGroupBlock* b = new NodeGroupBlock (this);
    _handle = b;
    b -> initialise ();
    return _handle;
}