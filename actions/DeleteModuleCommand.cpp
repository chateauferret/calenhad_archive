//
// Created by martin on 24/01/17.
//

#include "DeleteModuleCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/qneconnection.h"

DeleteModuleCommand::DeleteModuleCommand (QModule* module, CalenhadModel* model) : QUndoCommand(), _model (model), _module (module) {

}

DeleteModuleCommand::~DeleteModuleCommand () {

}

void DeleteModuleCommand::undo() {
    QDomDocument doc;
    doc.setContent (_xml);
    _model -> inflate (doc);
}

void DeleteModuleCommand::redo() {
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    _module -> serialise (doc);

    // serialise also any connections to or from the deleted owner
    // don't delete them here - the model will do that for us when we delete the owner
    for (QNEConnection* c : _model -> connections()) {
        if (c->port1 ()->owner () == _module || c->port2 ()->owner () == _module) {
            c -> serialise (doc);
        }
    }

    _xml = doc.toString ();
    _model -> deleteModule (_module);
}
