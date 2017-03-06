//
// Created by martin on 05/01/17.
//

#include "QNoiseMapExplorer.h"
#include <marble/HttpDownloadManager.h>
#include <marble/MarbleModel.h>
#include <marble/MarbleMap.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/GeoDataGroundOverlay.h>
#include <marble/GeoDataDocument.h>
#include "../mapping/GeoSceneEquirectTileProjection.h"
#include <QThread>



#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace noise::utils;
using namespace noise::module;
using namespace Marble;



QNoiseMapExplorer::QNoiseMapExplorer (const QString& title, QModule* source, const QWidget* parent) : QDialog(),
        _title (title), _name (source -> name()), _gradient (new GradientLegend()),
        _image (nullptr),
        _source (source),
        _tileProducer (nullptr),
        _tileMap (new GeoSceneEquirectTileProjection()) {

    _mapWidget = new CalenhadMarbleWidget (this);
    _mapWidget->setMapThemeId ("earth/calenhad/calenhad.dgml");
    std::cout << _mapWidget->mapThemeId ().toStdString () << "\n";
    _mapWidget->setWindowTitle (title);
    _layer = new CalenhadLayer (source);
    connect (_layer, SIGNAL (imageRefreshed()), this, SLOT (invalidate ()));
    _mapWidget -> addLayer (_layer);
    QVBoxLayout* layout = new QVBoxLayout ();
    layout->addWidget (_mapWidget);
    setLayout (layout);
    _mapWidget->setZoom (1000);
    setGeometry (200, 200, 640, 480);

    connect (_mapWidget, SIGNAL (visibleLatLonAltBoxChanged (const GeoDataLatLonAltBox&)), _layer, SLOT (refresh (const GeoDataLatLonAltBox&)));
    connect (_mapWidget, SIGNAL (resized (const QSize&)), _layer, SLOT (rescale (const QSize&)));
    connect (&timer, SIGNAL (timeout()), this, SLOT (boundsChanged()));

}


QNoiseMapExplorer::~QNoiseMapExplorer () {
    if (_mapWidget) { delete _mapWidget; }
    if (_source) { delete _source; }
    if (_layer) { delete _layer; }
    if (_gradient) { delete _gradient; }
    if (_tileProducer) { delete _tileProducer; }
    if (_tileMap) { delete _tileMap; }
    if (_layer) { delete _layer; }
    for (GeoDataDocument* doc : _tileDocuments.values()) {
        delete doc;
    }
}

void QNoiseMapExplorer::invalidate () {
    _mapWidget -> update();
}

void QNoiseMapExplorer::viewChanged (const GeoDataLatLonAltBox&) {
    timer.setSingleShot (true);
    timer.start();
}

void QNoiseMapExplorer::boundsChanged () {
    update();
}


// return 2 to the power of the given index
int TileProducer::powers (int index) {
    if (index < 0) { return 0; }
    int result = 1;
    result <<= (index - 1);
    return result;
}