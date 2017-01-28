//
// Created by martin on 24/01/17.
//

#include "DeleteConnectionCommand.h"
#include "../nodeedit/qneconnection.h"

DeleteConnectionCommand::DeleteConnectionCommand (QNEConnection* connection, CalenhadModel* model) : _connection (connection), _model (model) {
    setText ("Delete connection"); // to do - identify connection by module/port names
}

DeleteConnectionCommand::~DeleteConnectionCommand() {

}

void DeleteConnectionCommand::undo () {
    if (_from && _to) {
        _model -> connectPorts (_from, _to);
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
