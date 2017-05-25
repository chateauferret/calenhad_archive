//
// Created by martin on 23/05/17.
//

#include "ScanLineRenderer.h"
#include "../geoutils.h"

using namespace noise::model;
using namespace icosphere;

ScanLineRenderer::ScanLineRenderer (const ScanLineRendererParams& params, Sphere* sphere, Legend* legend) : QObject(),
    _lon (params.longitude),
    _south (params.south),
    _north (params.north),
    _dLat (params.dLatitude),
    _final (params._final),
    _sphere (sphere),
    _legend (legend) {
}

ScanLineRenderer::~ScanLineRenderer() {
}

void ScanLineRenderer::run() {
    _scanline = std::make_shared <GlobeBuffer>();
    QColor color;
    _lat = _south;
    while (_lat < _north) {
        double value = _sphere -> GetValue (radiansToDegrees (_lat), radiansToDegrees (_lon));
        color = _legend -> lookup (value);
        _lat += _dLat;
        writeRenderPoint (RenderPoint (_lon, _lat, color));
    }
    emit scanline (_scanline);
    if (_final) {
        emit complete (_scanline);
    }
}

void ScanLineRenderer::writeRenderPoint (const RenderPoint& point) {
    QMutexLocker locker (&mutex);
    _scanline -> append (point);
}

