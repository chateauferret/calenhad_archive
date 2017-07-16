
//
// Created by martin on 06/02/17.
//

#include "IcosphereBuilder.h"
#include "../icosphere/vertex.h"
#include <QMutexLocker>
#include <qmodule/QModule.h>

using namespace noise::utils;
using namespace noise::module;
using namespace geoutils;
using namespace icosphere;

IcosphereBuilder::IcosphereBuilder (std::shared_ptr<Icosphere> icosphere) : _icosphere (icosphere) {
}

IcosphereBuilder::~IcosphereBuilder () {

}

void IcosphereBuilder::build() {
    emit status ("Constructing icosphere");
    if ((!(_icosphere)) || _depth != _icosphere->depth () || _bounds != _icosphere->bounds ()) {
        _icosphere = std::make_shared<Icosphere> (_depth);
        connect (_icosphere.get (), SIGNAL (progress (const int&)), this, SIGNAL (progress (const int&)), Qt::QueuedConnection);
        connect (_icosphere.get (), SIGNAL (ready ()), this, SLOT (fill()), Qt::QueuedConnection);
        _icosphere -> lock();
        _icosphere->assemble (_bounds);
    }
}

void IcosphereBuilder::fill() {
    emit status ("Populating vertices");
    emit progress (0);

    int prog = 0, j = 0, p = 0;
    if (_module) {
        for (Vertex* v : * (_icosphere -> vertices())) {
            Cartesian c = v -> getCartesian ();
            double value = _module -> GetValue (c.x, c.y, c.z);
            v -> setDatum (_key, value);
            j++;
            p = (int) ((double) j / (double) _icosphere -> vertexCount ()) * 100;
            if (p != prog) {
                emit progress (p);
                prog = p;
            }
        }
    }
    emit complete();
    _icosphere -> unlock();
}

void IcosphereBuilder::cancel() {
    QMutexLocker locker (&_mutex);
    _cancelled = true;
}

bool IcosphereBuilder::isAbandoned() {
    QMutexLocker locker (&_mutex);
    return _cancelled;
}

std::shared_ptr<Icosphere> IcosphereBuilder::icosphere () {
    return _icosphere;
}


int IcosphereBuilder::vertexCount () {
    if (_icosphere) {
        return _icosphere -> vertexCount();
    } else {
        return _bounds.estimateVertexCount (_depth);
    }
}

void IcosphereBuilder::setDepth (const int& depth) {
    _depth = depth;
}

void IcosphereBuilder::setBounds (const Bounds& bounds) {
    _bounds = bounds;
}
void IcosphereBuilder::setModule (noise::module::Module* module) {
    _module = module;
}

void IcosphereBuilder::setKey (const QString& key) {
    _key = key;
}
