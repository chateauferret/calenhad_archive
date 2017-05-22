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
#include <QtCore/QQueue>

using namespace geoutils;
using namespace Marble;
using namespace noise::utils;
using namespace noise::model;

CalenhadLayer::CalenhadLayer (QModule* source) :
        _viewport (nullptr),
        _source (source),
        _gradient (new GradientLegend ("default")),
        _toDo (0), _done (0), _finished (false),
        _globeChanged (true),
        _buffer (std::make_shared <GlobeBuffer>()),
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
    renderOverview();
    time_t start = std::clock();
    _viewport = viewport;
    if (_globeChanged) {
        populate();
    } else {
        double lon, lat;
        QColor color;

        QListIterator<QPair<double, Scanline>> i (*_buffer);
        while (i.hasNext()) {
            QPair <double, Scanline> p = i.next();
            QListIterator<QPair<double, QColor>> j (p.second);
            while (j.hasNext ()) {
                QPair <double, QColor> q = j.next();
                lon = p.first;
                lat = q.first;
                color = q.second;
                painter->setPen (color);
                painter->drawPoint (GeoDataCoordinates (lon, lat, GeoDataCoordinates::Radian));
            }
        }
    }
    double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << "CalenhadLayer::render " << duration << " sec\n";
    return 0;
}

void CalenhadLayer::populate() {
    if (_source -> legend()) {
        if (_viewport) {
            emit (abandonJobs());
            GlobeRenderJob* job = new GlobeRenderJob (_viewport, _source -> module (), _source -> legend ());
            QThread* thread = new QThread ();
            job->moveToThread (thread);
            connect (thread, SIGNAL (started ()), job, SLOT (startJob ()));
            connect (job, SIGNAL (complete (std::shared_ptr<GlobeBuffer>)), thread, SLOT (quit ()));
            connect (job, SIGNAL (progress (const double&)), this, SIGNAL (progress (const double&)));
            connect (job, SIGNAL (bufferUpdated (std::shared_ptr<GlobeBuffer>)), this, SLOT (updateBuffer (std::shared_ptr<GlobeBuffer>)));
            connect (thread, SIGNAL (finished ()), thread, SLOT (deleteLater ()));
            connect (this, &CalenhadLayer::abandonJobs, job, &RenderJob::abandon);

            thread->start ();
        }
    }
}

void CalenhadLayer::updateBuffer (std::shared_ptr<GlobeBuffer> buffer) {
    _buffer = buffer;
    emit imageRefreshed();
    _globeChanged = false;

}

bool CalenhadLayer::render (GeoPainter* painter, ViewportParams* viewport, const QString& renderPos, GeoSceneLayer* layer) {
    //if (viewport -> viewLatLonAltBox () != _box) {
        _box = viewport -> viewLatLonAltBox ();
        return render (painter, viewport) == 0;
    //}
    //return false;
}

void CalenhadLayer::rescale() {
    refresh (_buffer);
}

void CalenhadLayer::renderOverview() {
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
            c = _gradient -> lookup (value);
            _overview -> setPixelColor (px, py, c);
        }
    }
    if (_overview) {
        emit overviewRendered (*_overview);
    }
    delete sphere;
}

void CalenhadLayer::refresh (std::shared_ptr<GlobeBuffer> buffer) {
   _globeChanged = true;
    emit imageRefreshed();
}
