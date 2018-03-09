//
// Created by martin on 09/01/18.
//

#include "RerouteConnectionCommand.h"
#include "CreateConnectionCommand.h"
#include "nodeedit/Connection.h"
#include "../pipeline/CalenhadModel.h"
using namespace calenhad::actions;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;

RerouteConnectionCommand::RerouteConnectionCommand (nodeedit::Port* from, nodeedit::Port* oldPort, nodeedit::Port* newPort, pipeline::CalenhadModel* model) :
        _from (from), _oldPort (oldPort), _newPort (newPort), _model (model) {
    setText ("Reroute connection"); // to do - identify connection by owner/port names
}

RerouteConnectionCommand::~RerouteConnectionCommand() {

}

void RerouteConnectionCommand::undo () {
    _model -> rerouteConnection (_from, _newPort, _oldPort);
}

void RerouteConnectionCommand::redo () {
    // reroute the connection

    if (_oldPort && _newPort) {
        _model -> rerouteConnection (_from, _oldPort, _newPort);
    }

}
