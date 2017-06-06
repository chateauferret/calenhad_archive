//
// Created by martin on 24/01/17.
//

#include "AddModuleCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/qneconnection.h"

AddModuleCommand::AddModuleCommand (QModule* module, const QPointF pos, CalenhadModel* model) : QUndoCommand(), _model (model), _pos (pos), _copy (module) {

}

AddModuleCommand::~AddModuleCommand () {

}

void AddModuleCommand::undo() {
    _copy = _module -> clone();
    _model->deleteNode (_module);
}

void AddModuleCommand::redo() {
    _module = _copy;
    _model -> addNode (_module, _pos);

}
