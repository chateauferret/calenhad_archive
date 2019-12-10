//
// Created by martin on 12/10/18.
//

#include "IcosphereModule.h"
#include "RasterModule.h"

using namespace calenhad::preferences;
using namespace calenhad::module;
using namespace calenhad::grid;

IcosphereModule::IcosphereModule() : RasterModule (CalenhadServices::preferences() -> calenhad_module_icospheremap) {
    _icosphere = new Icosphere (8);
    initialise();
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

    uint height = CalenhadServices::preferences() -> calenhad_globe_texture_height;
    _raster = new QImage (height * 2, height, QImage::Format_ARGB32);
}

bool IcosphereModule::isComplete () {
    return false;
}

void IcosphereModule::updateCache() {
    Module::updateCache();
    _preview -> fillIcosphere (_icosphere);
    _icosphere -> makeRaster (_raster);
}

Icosphere* IcosphereModule::icosphere () {
    return _icosphere;
}

QImage* IcosphereModule::raster() {
    _icosphere -> makeRaster (_raster);
    return _raster;
}

