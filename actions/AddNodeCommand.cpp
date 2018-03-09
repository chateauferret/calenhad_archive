//
// Created by martin on 24/01/17.
//

#include "AddNodeCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "nodeedit/Connection.h"
#include "qmodule/Module.h"

using namespace calenhad::actions;
using namespace calenhad::qmodule;
using namespace calenhad::pipeline;

AddNodeCommand::AddNodeCommand (Node* node, const QPointF pos, CalenhadModel* model) : QUndoCommand(), _model (model), _pos (pos), _copy (node) {

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

Node* AddNodeCommand::node() {
    return _node;
}