//
// Created by martin on 19/05/17.
//

#include <QtCore/QThread>
#include "RenderJob.h"

RenderJob::RenderJob (const Marble::GeoDataLatLonBox& bounds, noise::module::Module* source, Legend* legend) : QObject(),
    _bounds (bounds),
    _source (source),
    _legend (legend),
    _abandoned (false) {

}

RenderJob::~RenderJob() { }

bool RenderJob::canRender (const noise::module::Module* source) {
    // Only render if all the inputs to all the upstream noise modules are satisfied. If they are not,
    // attempting to render would cause any owner without an expected input to fail an assertion and crash the application.
    if (source && _legend) {
        int inputs = source -> GetSourceModuleCount ();
        if (inputs == 0) {
            return true;
        }
        for (int i = 0; i < inputs; i++) {
            try {
                if (! canRender (& (source -> GetSourceModule (i)))) {
                    return false;
                }
            } catch (noise::ExceptionNoModule e) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

void RenderJob::abandon() {
    QMutexLocker locker (&_abandonMutex);
    _abandoned = true;
}

bool RenderJob::isAbandoned () {
    QMutexLocker locker (&_abandonMutex);
    return _abandoned;
}
