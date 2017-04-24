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
        _title (title), _name (source -> name()), _gradient (new GradientLegend ("default")),
        _image (nullptr),
        _source (source) {

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


    connect (_mapWidget, SIGNAL (visibleLatLonAltBoxChanged (const GeoDataLatLonAltBox&)), this, SLOT (changeView (const GeoDataLatLonAltBox&)));
    connect (_mapWidget, SIGNAL (resized (const QSize&)), _layer, SLOT (rescale()));
    connect (&timer, SIGNAL (timeout()), this, SLOT (changeBounds()));
    connect (this, SIGNAL (boundsChanged  (const GeoDataLatLonBox&)), _source, SLOT (changeBounds (const GeoDataLatLonBox&)));
    connect (_source, SIGNAL (nodeChanged()), this, SLOT (invalidate()));

}

QNoiseMapExplorer::~QNoiseMapExplorer() {
    if (_mapWidget) { delete _mapWidget; }
    if (_layer) { delete _layer; }
    if (_gradient) { delete _gradient; }
    if (_layer) { delete _layer; }
}

void QNoiseMapExplorer::invalidate () {
    _mapWidget -> update();
}

void QNoiseMapExplorer::changeView (const GeoDataLatLonAltBox& bounds) {
    changeView ((GeoDataLatLonBox) bounds);
}

void QNoiseMapExplorer::changeView (const GeoDataLatLonBox& bounds) {
    _bounds = bounds;
    _layer -> rescale();
    timer.setSingleShot (true);
    timer.start();
}

void QNoiseMapExplorer::changeBounds () {
    emit boundsChanged (_bounds);
}


// return 2 to the power of the given index
int TileProducer::powers (int index) {
    if (index < 0) { return 0; }
    int result = 1;
    result <<= (index - 1);
    return result;
}