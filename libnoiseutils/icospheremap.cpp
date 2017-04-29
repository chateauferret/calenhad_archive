//
// Created by martin on 08/03/17.
//

#include <sys/socket.h>
#include "icospheremap.h"
#include "../icosphere/icosphere.h"
#include "../libnoiseutils/IcosphereBuilder.h"
#include <QThread>

using namespace noise::module;
using namespace noise::utils;
using namespace icosphere;

IcosphereMap::IcosphereMap (std::shared_ptr<Icosphere> icosphere) :  Module (GetSourceModuleCount ()), _icosphere (icosphere) {

}

double IcosphereMap::GetValue (double x, double y, double z) const {
    assert (m_pSourceModule [0] != NULL);
    if (_icosphere) {
        std::experimental::optional<double> value = _icosphere -> getDatum (geoutils::Cartesian (x, y, z), "");
        if (value) {
            return value.value ();
        } else {
            // if the icosphere holds no value for that location, i.e. it is out of icosphere's bounds or is not filled, consult the source owner instead
            return m_pSourceModule[0]->GetValue (x, y, z);
        }
    } else {
        return 0;
    }
}

void IcosphereMap::cancelBuild() {
    emit buildCancelled();
}

void IcosphereMap::buildIcosphere (const unsigned& depth, const Bounds& bounds) {
    IcosphereBuilder* builder = new IcosphereBuilder();
    builder -> setDepth (depth);
    builder -> setBounds (bounds);
    builder -> setModule (const_cast<Module*> (m_pSourceModule [0]));
    QThread* thread = new QThread();
    connect (thread, SIGNAL (started ()), builder, SLOT (build()));
    connect (builder, SIGNAL (complete (std::shared_ptr<icosphere::Icosphere>)), this, SLOT (icosphereBuilt (std::shared_ptr<icosphere::Icosphere>)));
    connect (builder, SIGNAL (complete (std::shared_ptr<icosphere::Icosphere>)), thread, SLOT (quit ()));
    connect (builder, SIGNAL (progress (int)), this, SLOT (setProgress (int)));
    connect (thread, SIGNAL (finished()), thread, SLOT (deleteLater ()));
    connect (thread, SIGNAL (finished()), builder, SLOT (deleteLater ()));
    connect (this, SIGNAL (buildCancelled()), builder, SLOT (cancel()));
    builder -> moveToThread (thread);
    thread -> start ();
}

void IcosphereMap::icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere) {
    _icosphere = icosphere;
    emit available (_icosphere);
}