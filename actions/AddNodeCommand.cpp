//
// Created by martin on 24/01/17.
//

#include "AddNodeCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/qneconnection.h"

AddNodeCommand::AddNodeCommand (QNode* node, const QPointF pos, CalenhadModel* model) : QUndoCommand(), _model (model), _pos (pos), _copy (node) {

}

AddNodeCommand::~AddNodeCommand () {

}

void AddNodeCommand::undo() {
    _copy = _node -> clone();
    _model->deleteNode (_node);
}

void AddNodeCommand::redo() {
    _node = _copy;
    _model -> addNode (_node, _pos);

}

QNode* AddNodeCommand::node() {
    return _node;
}