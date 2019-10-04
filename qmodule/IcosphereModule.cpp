//
// Created by martin on 12/10/18.
//

#include "IcosphereModule.h"
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::preferences;
using namespace calenhad::qmodule;
using namespace calenhad::icosphere;

IcosphereModule::IcosphereModule (Module* parent) : Module (CalenhadServices::preferences ()->calenhad_module_raster, nullptr) {

}

IcosphereModule::~IcosphereModule () {
    if (_icosphere) {
        delete _icosphere;
    }
}

void IcosphereModule::initialise () {
    Module::initialise();
    // create a panel to hold the parameter widgets, if we haven't done this already
    if (! (_content)) {
        addContentPanel();
    }

    _icosphere = new Icosphere (7);
}

bool IcosphereModule::isComplete () {
    return false;
}

QString IcosphereModule::glsl () {
    return QString();
}
