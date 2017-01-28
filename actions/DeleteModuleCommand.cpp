//
// Created by martin on 24/01/17.
//

#include "DeleteModuleCommand.h"
#include "../pipeline/CalenhadModel.h"

DeleteModuleCommand::DeleteModuleCommand (QModule* module, CalenhadModel* model) : _module (module), _model (model) {

}

DeleteModuleCommand::~DeleteModuleCommand () {

}

void DeleteModuleCommand::undo () {

}

void DeleteModuleCommand::redo () {
    _model -> deleteModule (_module);
}
