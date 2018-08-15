//
// Created by martin on 06/06/17.
//

#include <CalenhadServices.h>
#include "NodeGroup.h"
#include "../pipeline/CalenhadModel.h"
#include "nodeedit/NodeGroupBlock.h"

using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;

NodeGroup::NodeGroup (QWidget* parent) : Node (parent), _itemGroup (new QGraphicsItemGroup()) {

}

NodeGroup::~NodeGroup() {
    if (_itemGroup) { delete _itemGroup; }
}

bool NodeGroup::isWithin (const QPoint& point) {
    return ((NodeGroupBlock*) _block) -> rect().contains (point);
}

QString NodeGroup::nodeType () {
    return "nodegroup";
}


void NodeGroup::initialise() {
    Node::initialise();
}

QGraphicsItem* NodeGroup::makeHandle() {
    NodeGroupBlock* b = new NodeGroupBlock (this, nullptr);
    _block = b;
    b -> initialise();
    _itemGroup -> addToGroup (_block);
    _itemGroup -> setFlag (QGraphicsItem::ItemIsMovable);
    _itemGroup -> setFlag (QGraphicsItem::ItemIsSelectable);
    _itemGroup -> setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
    _itemGroup -> setCursor (Qt::OpenHandCursor);
    return _itemGroup;
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

void NodeGroup::assignGroup() {
    Node::assignGroup();

    if (group()) {
        _block -> setZValue (group() -> handle() -> zValue() + 1);

    } else {
        _block -> setZValue (-1000);
    }
}


void NodeGroup::attach (Node* node) {
    _itemGroup -> addToGroup (node -> handle());
}

void NodeGroup::detach (Node* node) {
    _itemGroup -> removeFromGroup (node -> handle());
}

QGraphicsItem* NodeGroup::handle () {
    return _itemGroup;
}
