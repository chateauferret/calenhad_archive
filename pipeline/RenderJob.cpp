//
// Created by martin on 30/12/16.
//

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include "RenderJob.h"
#include "../libnoiseutils/NoiseContstants.h"
#include "noise/module/modulebase.h"
#include "../libnoiseutils/NoiseMapBuilderSphere.h"
#include "../libnoiseutils/NoiseMap.h"
#include "../libnoiseutils/RendererImage.h"
#include "../libnoiseutils/GradientLegend.h"
#include "../mapping/GeoSceneEquirectTileProjection.h"
#include <QMutexLocker>

using namespace noise::utils;
using namespace geoutils;
using namespace Marble;


RenderJob::RenderJob (const TileId& id, noise::module::Module* source, Marble::GeoSceneAbstractTileProjection* projection) :
        _id (id),
        _source (source),
        _percentComplete (0) {

    int zoom = id.zoomLevel() > MAX_ZOOM ? MAX_ZOOM : id.zoomLevel();

    projection -> geoCoordinates (zoom, id.x(), id.y(), _bounds);
    std::cout << "Tile bounds " << _bounds.north () << " " << _bounds.west () << " - " << _bounds.south () << " " << _bounds.east () << "\n";
}


RenderJob::RenderJob (const GeoDataLatLonBox& bounds, noise::module::Module* source, icosphere::Legend* legend) :
    _id (TileId()),_source (source), _percentComplete (0), _bounds (bounds), _legend (legend) {
}


RenderJob::~RenderJob () {

}

bool RenderJob::canRender () {

    if (_image && _source && _legend) {
        int inputs = _source -> GetSourceModuleCount ();
        if (inputs == 0) {
            return true;
        }
        for (int i = 0; i < inputs; i++) {
            try {
                if (&(_source -> GetSourceModule (i)) == nullptr) {
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

void RenderJob::startJob() {

    if (canRender()) {

        render ();

        if (! QThread::currentThread() -> isInterruptionRequested()) {
            if (_image) {
                emit complete (_id, _image);
            } else {
                std::cout << "Interrupted";
            }

        } else {
            std::cout << "No image for render target\n";
        }
    }
}

void RenderJob::render() {

    NoiseMap heightMap;
    NoiseMapBuilderSphere heightMapBuilder (this);

    heightMapBuilder.setSourceModule (*_source);
    heightMapBuilder.setDestNoiseMap (heightMap);
    heightMapBuilder.setDestSize (_image -> width(), _image -> height());
    heightMapBuilder.SetBounds (_bounds);
    heightMapBuilder.build();

    RendererImage renderer (this);

    renderer.setSourceNoiseMap (heightMap);
    renderer.setDestImage (_image);
    //GradientLegend legend = GradientLegend ("default");
    renderer.setLegend (_legend);
    renderer.setLightEnabled (true);
    renderer.setLightContrast (3.0);
    renderer.setLightBrightness (2.0);
    renderer.render();
}

void RenderJob::setImage (std::shared_ptr<QImage>& image) {
    _image = image;
}

std::shared_ptr<QImage> RenderJob::image() {
    return _image;
}

void RenderJob::sendProgress (int p) {
    emit progress (p);
}

void RenderJob::abandon() {
    QMutexLocker locker (&_mutex);
    _abandoned = true;
}

bool RenderJob::isAbandoned () {
    QMutexLocker locker (&_mutex);
    return _abandoned;
}
