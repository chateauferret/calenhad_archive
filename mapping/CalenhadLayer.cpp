//
// Created by martin on 26/01/17.
//

#include "CalenhadLayer.h"
#include <marble/GeoPainter.h>
#include <marble/ViewportParams.h>
#include "../libnoiseutils/NoiseContstants.h"
#include "Legend.h"
#include "../CalenhadServices.h"
#include "../controls/CalenhadGlobe.h"
#include <libnoise/model/sphere.h>
#include <ctime>
#include <QThread>
#include <messages/QProgressNotification.h>

using namespace geoutils;
using namespace Marble;
using namespace icosphere;
using namespace noise::model;

CalenhadLayer::CalenhadLayer (QModule* source) :
        _viewport (nullptr),
        _source (source),
        _toDo (0), _done (0), _finished (false),
        _globeChanged (true),
        _buffer (nullptr),
        _overview (new QImage (210, 105, QImage::Format_ARGB32)) {
}

CalenhadLayer::~CalenhadLayer() {
    if (_overview) { delete _overview; }
}


Legend* CalenhadLayer::legend() {
    return _source -> legend();
}

QStringList CalenhadLayer::renderPosition() const {
    QStringList list;
    list << QString ("SURFACE");
    return list;
}

int CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport) {
    renderOverview();
    time_t start = std::clock();
    _viewport = viewport;
    if (_globeChanged) {
        populate();
    } else {
        if (_buffer) {
            QMutexLocker locker (&mutex);
            QListIterator<RenderPoint> i (*_buffer);
            while (i.hasNext()) {
                RenderPoint p = i.next();
                painter -> setPen (p._color);
                painter -> drawPoint (GeoDataCoordinates (p._lon, p._lat, GeoDataCoordinates::Radian));
            }
            double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            return 0;
        }
    }
}

void CalenhadLayer::populate() {

    if (_source -> legend()) {
        if (_viewport) {
            emit (abandonJobs());

            _job = new GlobeRenderJob (_viewport, _source, _source -> legend ());
            // progress bar
            QProgressNotification* _progressBar = CalenhadServices::messages() -> progress ("info", "Rendering map", 500, _toDo, 10000);
            connect (_job, &RenderJob::progress, _progressBar, &QProgressNotification::setProgress, Qt::QueuedConnection);
            connect (_job, &RenderJob::abandoned, _progressBar, &QProgressNotification::kill, Qt::QueuedConnection);
            connect (_job, SIGNAL (complete()), _progressBar, SLOT (setComplete()));

            QThread* thread = new QThread ();
            _job -> moveToThread (thread);
            connect (thread, SIGNAL (started ()), _job, SLOT (startJob ()), Qt::QueuedConnection);
            connect (_job, SIGNAL (bufferUpdated (std::shared_ptr<GlobeBuffer>)), this, SLOT (updateBuffer (std::shared_ptr<GlobeBuffer>)), Qt::QueuedConnection);
            connect (_job, SIGNAL (complete()), thread, SLOT (quit ()), Qt::QueuedConnection);
            //connect (_job, SIGNAL (complete()), this, SLOT (updateBuffer (std::shared_ptr<GlobeBuffer>)), Qt::QueuedConnection);
            connect (_job, SIGNAL (progress (const double&)), this, SIGNAL (progress (const double&)), Qt::QueuedConnection);
            connect (thread, SIGNAL (finished ()), thread, SLOT (deleteLater ()), Qt::QueuedConnection);
            connect (this, &CalenhadLayer::abandonJobs, _job, &RenderJob::abandon, Qt::QueuedConnection);
            thread -> start ();
        }
    }
}

void CalenhadLayer::updateBuffer (std::shared_ptr<GlobeBuffer> buffer) {
    _buffer = buffer;
    emit imageRefreshed();
    _globeChanged = false;
}

bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    _box = viewport -> viewLatLonAltBox ();
    return render (painter, viewport) == 0;
}

void CalenhadLayer::rescale() {
    refresh (_buffer);
}

void CalenhadLayer::renderOverview() {
    double minimum = 0.0, maximum = 0.0;
    Sphere* sphere = new Sphere();
    sphere -> SetModule (* (_source -> module ()));
    QColor c;
    double dLat = 360.0 / _overview -> width();
    double dLon = 180.0 / _overview -> height();
    double lat, lon;
    for (int px = 0; px < _overview -> width(); px ++) {
        for (int py = 0; py < _overview -> height(); py ++) {
            lon = -180.0 + (dLon * px);
            lat = 90.0 - (dLat * py);
            double value = sphere -> GetValue (lat, lon);
            if (value < minimum || minimum == 0.0) { minimum = value; }
            if (value > maximum || maximum == 0.0) { maximum = value; }
            c = legend() -> lookup (value);
            _overview -> setPixelColor (px, py, c);
        }
    }
    if (_overview) {
        emit overviewRendered (*_overview);
    }
    delete sphere;
    CalenhadServices::statistics() -> recordGlobalExtremes (minimum, maximum);
}

void CalenhadLayer::refresh (std::shared_ptr<GlobeBuffer> buffer) {
   _globeChanged = true;
    emit imageRefreshed();
    _buffer = buffer;
}
