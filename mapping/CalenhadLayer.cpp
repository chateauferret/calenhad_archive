    //
// Created by martin on 26/01/17.
//

#include "CalenhadLayer.h"
#include <marble/GeoPainter.h>

#include "../legend/Legend.h"
#include "libnoiseutils/NoiseConstants.h"
#include "../CalenhadServices.h"
#include "../controls/globe/StatisticsService.h"
#include <ctime>
#include <QThread>
#include <messages/QProgressNotification.h>
#include <pipeline/ScanLineRenderer.h>
#include <QtCore/QThreadPool>
#include <iostream>

using namespace Marble;
using namespace icosphere;
using namespace noise::model;
using namespace calenhad::mapping;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::legend;

CalenhadLayer::CalenhadLayer (QModule* source) :
        _viewport (nullptr),
        _restart (true),
        _source (source),
        _buffer (std::make_shared<RenderBuffer>()),
        _sphere (new Sphere()),
        _overview (new QImage (210, 105, QImage::Format_ARGB32)),
        _timer (new QTimer()) {
    _sphere -> SetModule (*(_source -> module()));
    _timer -> setInterval (0);
    connect (_timer, &QTimer::timeout, this, &CalenhadLayer::populate);

}

CalenhadLayer::~CalenhadLayer() {
    if (_overview) { delete _overview; }
    if (_sphere) { delete _sphere; }
    if (_timer) { delete _timer; }
}

void CalenhadLayer::makeBuffer (ViewportParams* viewport) {
    _viewport = viewport;
    _buffer -> clear();

    _minimum = _maximum = 0.0;
    for (int x = 0; x < viewport -> width(); x++) {
        RenderLine line;
        for (int y = 0; y < viewport -> height(); y++) {
            RenderPoint rp (QPoint(x, y), viewport);
            if (rp.isValid ()) {
                line.push_back (rp);
            }
        }
        if (! line.empty()) {
            _buffer -> push_back (line);
        }
    }
    i = _buffer -> begin();
    _scanline = _buffer -> begin();
    _timer -> start();
}


Legend* CalenhadLayer::legend() {
    return _source -> legend();
}

QStringList CalenhadLayer::renderPosition() const {
    QStringList list;
    list << QString ("SURFACE");
    return list;
}

int CalenhadLayer::render (GeoPainter* painter) {
    renderOverview();
    time_t start = std::clock();
    emit renderingStart();
    if (_buffer) {
        _count = 0;
        QMutexLocker locker (&_mutex);
        RenderLine::iterator j;
        i = _buffer -> begin();
        while (i != _buffer -> end()) {
            j = i -> begin ();
            while (j != i -> end ()) {
                RenderPoint p = *j;
                if (p.isReady ()) {
                    painter -> setPen (p.getColor ());
                    painter -> drawPoint (GeoDataCoordinates (p._lonRadians, p._latRadians));
                    if (p.getValue () < _minimum || _minimum == 0.0) { _minimum = p.getValue (); }
                    if (p.getValue () > _maximum || _maximum == 0.0) { _maximum = p.getValue (); }
                    _count++;
                } else {
                    break; // stop if one isn't ready because any subsequent won't be either
                }
                j++;
            }
            i++;
        }
        double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        //std::cout << "Render " << _count << " in " << duration << " seconds\n";
        return 0;
    }

    return -1;
}

void CalenhadLayer::populate() {
    if (_scanline != _buffer -> end()) {
        ScanLineRenderer* renderer = new ScanLineRenderer (_scanline, _sphere, _source -> legend());
        if (_scanline + 1 == _buffer -> end()) {
            renderer -> setFinalScanLine (true);
            connect (renderer, &ScanLineRenderer::complete, this, &CalenhadLayer::gatherStats);
            _timer -> stop();
        }
        connect (renderer, &ScanLineRenderer::scanline, this, &CalenhadLayer::refresh, Qt::QueuedConnection);

        QThreadPool::globalInstance() -> start (renderer);
        _scanline++;
    }
}

void CalenhadLayer::gatherStats() {
    time_t finish = std::clock();
    CalenhadServices::statistics() -> recordMapExtremes (_minimum, _maximum);
    double secs = double (finish - _start) / CLOCKS_PER_SEC;
    CalenhadServices::statistics() -> recordRenderMetrics (QDateTime::currentDateTime(), _count, (int) (secs * 1000));
    emit complete();
}

bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    if ((! _viewport) || _restart ||
        _box != viewport -> viewLatLonAltBox())  {
        _restart = false;
        makeBuffer (viewport);
        _viewport = viewport;
        _box = viewport -> viewLatLonAltBox();
    }
    return render (painter) == 0;
}

void CalenhadLayer::rescale() {
    refresh();
}

void CalenhadLayer::renderOverview() {
    double minimum = 0.0, maximum = 0.0;
    QColor c;
    double dLat = 360.0 / _overview -> width();
    double dLon = 180.0 / _overview -> height();
    double lat, lon;
    for (int px = 0; px < _overview -> width(); px ++) {
        for (int py = 0; py < _overview -> height(); py ++) {
            lon = -180.0 + (dLon * px);
            lat = 90.0 - (dLat * py);
            double value = _sphere -> GetValue (lat, lon);
            if (value < minimum || minimum == 0.0) { minimum = value; }
            if (value > maximum || maximum == 0.0) { maximum = value; }
            c = legend() -> lookup (value);
            _overview -> setPixelColor (px, py, c);
        }
    }
    if (_overview) {
        emit overviewRendered (*_overview);
    }
    CalenhadServices::statistics() -> recordGlobalExtremes (minimum, maximum);
}

void CalenhadLayer::refresh() {
    emit imageRefreshed();
}

void CalenhadLayer::restart () {
    if (_source -> isComplete()) {
        _restart = true;
        refresh ();
    }
}
