//
// Created by martin on 06/06/17.
//

#include <CalenhadServices.h>
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

QString NodeGroup::nodeType () {
    return "nodegroup";
}


void NodeGroup::initialise() {
    Node::initialise();
}

NodeBlock* NodeGroup::makeHandle() {
    NodeGroupBlock* b = new NodeGroupBlock (this);
    _handle = b;
    b -> initialise();
    return _handle;
}


void NodeGroup::inflate (const QDomElement& element) {
    Node::inflate (element);
}

void NodeGroup::serialize (QDomElement& element) {
    Node::serialize (element);
    NodeGroupBlock* block = (NodeGroupBlock*) handle();
    _element.setAttribute ("height", block -> rect().height());
    _element.setAttribute ("width", block -> rect().width());

    QDomElement nodesElement = element.ownerDocument().createElement ("nodes");
    _element.appendChild (nodesElement);
    for (Node* node : _model -> nodes()) {
        if (node -> group() == this) {
            node -> serialize (nodesElement);
        }
    }
}
