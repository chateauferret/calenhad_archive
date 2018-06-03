
// Created by martin on 28/04/17.
//

#include "DuplicateNodeCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "nodeedit/NodeBlock.h"
#include "../preferences/preferences.h"
#include "../CalenhadServices.h"
#include "qmodule/Module.h"
#include "qmodule/NodeGroup.h"
#include "../pipeline/ModuleFactory.h"

using namespace calenhad::actions;
using namespace calenhad::qmodule;
using namespace calenhad::pipeline;


DuplicateNodeCommand::DuplicateNodeCommand (Node* node, CalenhadModel* model) : _node (node), _model (model), _copy (nullptr) {

}

DuplicateNodeCommand::~DuplicateNodeCommand () {

}

void DuplicateNodeCommand::undo() {
    _model -> doDeleteNode (_copy);
}

void DuplicateNodeCommand::redo() {
    _copy = _node -> clone();
    QPointF p (_node -> handle() -> pos());
    p.setX (p.x() + CalenhadServices::preferences() -> calenhad_module_duplicate_offset_x);
    p.setY (p.y() + CalenhadServices::preferences() -> calenhad_module_duplicate_offset_y);
    _copy -> setName (_model -> uniqueName (_node -> name()));
    _model -> addNode (_copy, p);
}
