//
// Created by martin on 06/02/17.
//

#include "IcosphereBuilder.h"
#include "../icosphere/icosphereutils.h"
#include "../icosphere/vertex.h"
#include "libnoise/module/modulebase.h"


using namespace noise::utils;
using namespace noise::module;
using namespace geoutils;
using namespace icosphere;

IcosphereBuilder::IcosphereBuilder (const char& depth, const Bounds& bounds) : _depth (depth), _bounds (bounds), _icosphere (nullptr) {

}

IcosphereBuilder::~IcosphereBuilder () {
    if (_icosphere) {
        delete _icosphere;
    }
}

void IcosphereBuilder::build (std::string dataset, Legend* legend, Module* module) {

}

icosphere::Icosphere* IcosphereBuilder::icosphere () {
    return _icosphere;
}
