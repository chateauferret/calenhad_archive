//
// Created by martin on 26/01/17.
//

#include "CalenhadLayer.h"
#include <marble/GeoPainter.h>
#include <marble/ViewportParams.h>
#include "../libnoiseutils/NoiseContstants.h"
#include "../libnoiseutils/GradientLegend.h"
#include <libnoise/model/sphere.h>
#include <ctime>
#include <QThread>

using namespace geoutils;
using namespace Marble;
using namespace noise::utils;
using namespace noise::model;

CalenhadLayer::CalenhadLayer (QModule* source) :
        _source (source),
        _gradient (new GradientLegend ("default")),
        _sphere (new Sphere (*source -> module ())),
        _step (INITIAL_STEP),
        _toDo (0), _done (0), _finished (false),
        _overview (new QImage (210, 105, QImage::Format_ARGB32)) {

}

CalenhadLayer::~CalenhadLayer() {
    if (_gradient) { delete _gradient; }
    if (_overview) { delete _overview; }
}

void CalenhadLayer::setGradient (GradientLegend* gradient) {
    _gradient = gradient;
}

QStringList CalenhadLayer::renderPosition() const {
    QStringList list;
    list << QString ("SURFACE");
    return list;
}

int CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport) {

   return render (painter, viewport, 0);
}


int CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const int& offset) {
    _sphere -> SetModule (* (_source -> module()));

    // If we are starting a fresh render, we perform a "dry run" of the loops without any rendering in order to determine
    // how much work is to be done. Then we can report progress against this.
    if (_step == INITIAL_STEP) {
        renderOverview();
        _done = 0;
        for (int i = INITIAL_STEP; i > 1; i /= 2) {
            _step = i;
            renderMainMap (painter, viewport, true);
        }
        _toDo = _done;
        _done = 0;
        _step = INITIAL_STEP;
        emit renderingStarted();
        _finished = false;
    }

    // Now perform the current pass of rendering the main map, for real.
    if (! _finished) {
        renderMainMap (painter, viewport);
    }

    // Set the dither step for the next pass
    if (_step > 2) {
        _step /= 2;
        emit imageRefreshed();
    } else {
        _finished = true;
        emit renderingFinished();
    }

    return 0;
}

void CalenhadLayer::renderOverview() {
    QColor c;
    double dLat = 360.0 / _overview -> width() * _step;
    double dLon = 180.0 / _overview -> height() * _step;
    double lat, lon;
    for (int px = 0; px < _overview -> width(); px += _step) {
        for (int py = 0; py < _overview -> height(); py += _step) {
            lon = -180.0 + (dLon * px);
            lat = 90.0 - (dLat * py);
            double value = _sphere -> GetValue (lat, lon);
            c = _gradient -> lookup (value);
            _overview -> setPixelColor (px, py, c);
        }
    }
    if (_overview) {
        emit overviewRendered (*_overview);
    }
}

void CalenhadLayer::renderMainMap (GeoPainter* painter, ViewportParams* viewport, bool rehearse) {
    GeoDataLatLonAltBox box = viewport -> viewLatLonAltBox();
    QColor color;
    std::time_t start = std::clock();
    double lat;
    double lon = box.west();
    bool finished = false;
    while (! finished) {
        lon += _angularResolution * _step;
        if (box.east() > box.west()) {
            if (lon > box.east()) {
                finished = true;
            }
        } else {
            if (lon > box.east() && lon < box.west()) {
                finished = true;
            }
        }
        if (lon > M_PI) {
            lon -= 2 * M_PI;
        }
        lat = box.south();
        while (lat < box.north()) {
            lat += _angularResolution * _step;
            if (! rehearse) {
                double value = _sphere->GetValue (radiansToDegrees (lat), radiansToDegrees (lon));
                color = _gradient->lookup (value);
                painter->setPen (color);
                painter->drawPoint (GeoDataCoordinates ((qreal) lon, (qreal) lat));
            }
        }
        _done++;
        if (! rehearse) {
            emit progress (( (double) _done / (double) _toDo) * 100);
            std::cout << (( (double) _done / (double) _toDo) * 100) << "\n";
        }
    }
}

bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    _angularResolution = viewport -> angularResolution() / 8;
    return render (painter, viewport) == 0;
}

void CalenhadLayer::rescale() {
    _step = INITIAL_STEP;
}

QImage* CalenhadLayer::overview () {
    return _overview;
}
