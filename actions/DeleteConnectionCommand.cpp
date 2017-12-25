//
// Created by martin on 24/01/17.
//

#include "DeleteConnectionCommand.h"
#include "../nodeedit/qneconnection.h"
#include "../pipeline/CalenhadModel.h"
using namespace calenhad::actions;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;

DeleteConnectionCommand::DeleteConnectionCommand (QNEConnection* connection, CalenhadModel* model) : _connection (connection), _model (model) {
    setText ("Delete connection"); // to do - identify connection by owner/port names
}

DeleteConnectionCommand::~DeleteConnectionCommand() {

}

void DeleteConnectionCommand::undo () {
    if (_from && _to) {
        _connection = _model -> connectPorts (_from, _to);
        _model -> update();
    }
}

void DeleteConnectionCommand::redo () {
    // delete the connection
    if (_connection) {
        _from = _connection -> port1();
        _to = _connection -> port2();
        _model -> disconnectPorts (_connection);
    }
}
