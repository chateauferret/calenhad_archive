//
// Created by martin on 24/01/17.
//

#include "DeleteNodeCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/qneconnection.h"

DeleteNodeCommand::DeleteNodeCommand (QNode* node, CalenhadModel* model) : QUndoCommand(), _model (model), _node (node) {

}

DeleteNodeCommand::~DeleteNodeCommand () {

}

void DeleteNodeCommand::undo() {
    QDomDocument doc;
    doc.setContent (_xml);
    _model -> inflate (doc);
}

void DeleteNodeCommand::redo() {
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    _node -> serialise (doc);

    // serialise also any connections to or from the deleted owner
    // don't delete them here - the model will do that for us when we delete the owner
    for (QNEConnection* c : _model -> connections()) {
        if (c->port1 ()->owner () == _node || c->port2 ()->owner () == _node) {
            c -> serialise (doc);
        }
    }

    _xml = doc.toString ();
    _model->deleteNode (_node);
}
