//
// Created by martin on 12/10/18.
//

#include "IcosphereModule.h"
#include "RasterModule.h"

using namespace calenhad::preferences;
using namespace calenhad::module;
using namespace calenhad::icosphere;

IcosphereModule::IcosphereModule() : RasterModule (CalenhadServices::preferences() -> calenhad_module_icospheremap) {
    
}

IcosphereModule::~IcosphereModule () {
    delete _icosphere;
    
}

void IcosphereModule::initialise () {
    RasterModule::initialise();
    // create a panel to hold the parameter widgets, if we haven't done this already
    if (! (_content)) {
        addContentPanel();
    }

    _icosphere = CalenhadServices::icosphere();
}

bool IcosphereModule::isComplete () {
    return false;
}
