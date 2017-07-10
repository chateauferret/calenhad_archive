//
// Created by martin on 19/05/17.
//

#include <QtCore/QThread>
#include <iostream>
#include "RenderJob.h"
#include "../icosphere/icosphere.h"

RenderJob::RenderJob (const Marble::GeoDataLatLonBox& bounds, QModule* module, Legend* legend) : QObject(),
    _bounds (bounds),
    _module (module),
    _legend (legend),
    _abandoned (false) {

}

RenderJob::~RenderJob() { }

bool RenderJob::canRender (const Module* module) {
    // Only render if all the inputs to all the upstream noise modules are satisfied. If they are not,
    // attempting to render would cause any owner without an expected input to fail an assertion and crash the application.
    if (module && _legend) {
        // make sure all predecessor modules have inputs
        int inputs = module-> GetSourceModuleCount();
        if (inputs != 0) {
            for (int i = 0; i < inputs; i++) {
                try {
                    if (!canRender (&(module->GetSourceModule (i)))) {
                        return false;
                    }
                } catch (noise::ExceptionNoModule e) {
                    return false;
                }
            }
        }
        return true; //_module -> generateMap();
    } else {
        return false;
    }
}

void RenderJob::abandon() {
    QMutexLocker locker (&_abandonMutex);
    _abandoned = true;
    emit abandoned();
}

bool RenderJob::isAbandoned () {
    QMutexLocker locker (&_abandonMutex);
    return _abandoned;
}
