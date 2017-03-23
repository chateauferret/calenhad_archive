//
// Created by martin on 06/02/17.
//

#include "IcosphereBuilder.h"
#include "../icosphere/vertex.h"
#include <QMutexLocker>

using namespace noise::utils;
using namespace noise::module;
using namespace geoutils;
using namespace icosphere;

IcosphereBuilder::IcosphereBuilder() : _icosphere (nullptr), _module (nullptr) {
}

IcosphereBuilder::~IcosphereBuilder () {

}

void IcosphereBuilder::build() {
    if (! (_icosphere) || (_depth != _icosphere -> depth() && _bounds != _icosphere -> bounds())) {
        _icosphere = std::make_shared<Icosphere> (_depth, _bounds);
    }
    fill();
}

void IcosphereBuilder::fill() {
    if (_module) {
        {
            QMutexLocker locker (&_mutex);
            _cancelled = false;
        }
        Cartesian c;
        for (Vertex* v : _icosphere -> vertices ()) {
            if (isCancelled()) {
                break;
            }
            c = Math::toCartesian (v -> getGeolocation ());
            v -> setDatum ("", _module -> GetValue (c.x, c.y, c.z));
        }
    }
    emit complete (_icosphere);
}

void IcosphereBuilder::cancel() {
    QMutexLocker locker (&_mutex);
    _cancelled = true;
}

bool IcosphereBuilder::isCancelled() {
    QMutexLocker locker (&_mutex);
    return _cancelled;
}

std::shared_ptr<Icosphere> IcosphereBuilder::icosphere () {
    return _icosphere;
}

void IcosphereBuilder::setDepth (const int& depth) {
    _depth = depth;
}

void IcosphereBuilder::setBounds (const Bounds& bounds) {
    _bounds = bounds;
}

void IcosphereBuilder::setModule (Module* module) {
    _module = module;
}

void IcosphereBuilder::setIcosphere (std::shared_ptr<Icosphere> icosphere) {
    _icosphere = icosphere;
    _bounds = _icosphere -> bounds();
    _depth = icosphere -> depth();
}
