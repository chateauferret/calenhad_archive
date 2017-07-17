//
// Created by martin on 28/06/17.
//
#include <messages/QProgressNotification.h>
#include <CalenhadServices.h>
#include <sys/socket.h>
#include "IcosphereModule.h"
#include "IcosphereBuilder.h"


using namespace icosphere;
using namespace noise::utils;

IcosphereModule::IcosphereModule () : Module (1), _icosphere (nullptr), _notification (nullptr) {

}

IcosphereModule::~IcosphereModule() {
    if (_notification) { delete _notification; }
}

int IcosphereModule::GetSourceModuleCount () const {
    return 1;
}

double IcosphereModule::GetValue (double x, double y, double z) const {
    if (_icosphere) {
        return _icosphere -> getDatum (Math::toGeolocation (Cartesian (x, y, z)), _key);
    } else {
        //return GetSourceModule (0).GetValue (x, y, z);
        return -1.0;
    }
}

void IcosphereModule::setKey (const QString& key) {
    _key = key;
}

void IcosphereModule::buildIcosphere (const Bounds& bounds, const int& depth) {
    IcosphereBuilder* builder = new IcosphereBuilder (_icosphere);
    _bounds = bounds;
    _depth = depth;
    if (depth > 6) {
        int estimate = bounds.estimateVertexCount (depth);
        if (_notification) { delete _notification; }
        _notification = CalenhadServices::messages() -> progress ("info", "Constructing icosphere", 1000, estimate, 500);
        _notification -> setToDo (100);
        connect (builder, &IcosphereBuilder::abandoned, _notification, &QProgressNotification::kill, Qt::QueuedConnection);
        connect (builder, SIGNAL (progress (const int&)), _notification, SLOT (setProgress (const int&)), Qt::QueuedConnection);
        connect (builder, SIGNAL (status (const QString&)), _notification, SLOT (setMessage (const QString&)), Qt::QueuedConnection);
        connect (_notification, &QProgressNotification::cancel, builder, &IcosphereBuilder::cancel, Qt::QueuedConnection);

        QTimer* timer = new QTimer (this);
        timer -> setInterval (500);
        timer -> setSingleShot (true);
        timer -> start();
        connect (timer, &QTimer::timeout, _notification, &QWidget::show);
    }
    connect (builder, SIGNAL (complete (std::shared_ptr<Icosphere>)), this, SLOT (built (std::shared_ptr<Icosphere>)));
    builder -> setDepth (depth);
    builder -> setBounds (bounds);
    builder->setModule (const_cast<noise::module::Module*> (&(GetSourceModule (0))));
    builder -> setKey (_key);
    builder->buildAsync ();

    setKey (_key);
}

void IcosphereModule::built (std::shared_ptr<Icosphere> icosphere) {
    _icosphere = icosphere;
    _icosphere -> unlock();


}
int IcosphereModule::vertexCount() {
    if (_icosphere) {
        return _icosphere->vertexCount ();
    } else {
        return _bounds.estimateVertexCount (_depth);
    }
}