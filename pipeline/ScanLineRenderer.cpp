//
// Created by martin on 23/05/17.
//

#include "ScanLineRenderer.h"
#include "../geoutils.h"
#include "legend/Legend.h"

using namespace noise::model;
using namespace calenhad::pipeline;
using namespace calenhad::mapping;
using namespace calenhad::legend;

ScanLineRenderer::ScanLineRenderer (const RenderBuffer::iterator& index, Sphere* sphere, Legend* legend) : QObject(),
    _sphere (sphere),
    _legend (legend),
    _item (index -> begin()),
    _end (index -> end()) {

}


ScanLineRenderer::~ScanLineRenderer() {
}

void ScanLineRenderer::run() {
    QMutexLocker locker (&_mutex);
    double lat, lon;
    QColor color;
    while (_item != _end) {
        lat = _item -> _latDegrees;
        lon = _item -> _lonDegrees;
        double value = _sphere -> GetValue (lat, lon);
        color = _legend -> lookup (value);
        _item -> populate (value, color);
        _item++;
    }
    emit scanline();
    if (_final) {
        emit complete();
    }
}

void ScanLineRenderer::setFinalScanLine (const bool& f) {
    _final = f;
}

