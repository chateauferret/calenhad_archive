//
// Created by martin on 25/12/17.
//

#include "CreateConnectionCommand.h"
#include "nodeedit/Connection.h"
#include "../pipeline/CalenhadModel.h"
using namespace calenhad::actions;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;

CreateConnectionCommand::CreateConnectionCommand (Port* from, Port* to, CalenhadModel* model) : _connection (nullptr), _from (from), _to (to), _model (model) {
    setText ("Create connection"); // to do - identify connection by owner/port names
}

CreateConnectionCommand::~CreateConnectionCommand() {

}

void CreateConnectionCommand::undo () {
    if (_connection) {
        _model -> disconnectPorts (_connection);
    }
}

void CreateConnectionCommand::redo () {
    // create the connection

    if (_from && _to) {
        _connection = _model -> connectPorts (_from, _to);
        _model -> update();
    }

}
