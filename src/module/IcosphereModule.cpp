//
// Created by martin on 12/10/18.
//

#include "IcosphereModule.h"
#include "StructuredGrid.h"
#include "../grid/icosphere.h"

using namespace calenhad::preferences;
using namespace calenhad::module;
using namespace calenhad::grid;


IcosphereModule::IcosphereModule (const QString& type) : Module (type) {
    _icosphere = new Icosphere (8);
    initialise();
}

IcosphereModule::~IcosphereModule () {
    delete _icosphere;
}


void IcosphereModule::initialise () {

}

bool IcosphereModule::isComplete () {
    return false;
}


Icosphere* IcosphereModule::icosphere () {
    return _icosphere;
}

void IcosphereModule::fetch (calenhad::grid::CubicSphere* buffer) {
    buffer -> fromIcosphere (_icosphere);
}

