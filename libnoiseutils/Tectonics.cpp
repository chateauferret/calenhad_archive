//
// Created by martin on 04/07/17.
//

#include "Tectonics.h"
#include "icosphere/icosphere.h"

using namespace icosphere;

Tectonics::Tectonics () : Module (0) {
    _icosphere = new Icosphere (8);


}

Tectonics::~Tectonics () {

}

int Tectonics::GetSourceModuleCount () const {
    return 0;
}

double Tectonics::GetValue (double x, double y, double z) const {
    return _icosphere -> getDatum (Math::toGeolocation (Cartesian (x, y, z)), _key);
}
