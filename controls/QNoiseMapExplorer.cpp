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
#include <marble/PluginManager.h>
#include "../mapping/GeoSceneEquirectTileProjection.h"
#include "CalenhadOverviewMap.h"
#include <marble/MarbleWidgetInputHandler.h>
#include <QThread>
#include <marble/Planet.h>



#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace noise::utils;
using namespace noise::module;
using namespace Marble;

QNoiseMapExplorer::QNoiseMapExplorer (const QString& title, QModule* source, const QWidget* parent) : QDialog(),
        _title (title), _name (source -> name()),
        _image (nullptr),
        _source (source) {

    _mapWidget = new CalenhadGlobe (source, this);

    _mapWidget->setWindowTitle (title);
    //_layer = new CalenhadLayer (source);
    //_mapWidget -> addLayer (_layer);
    //connect (_layer, SIGNAL (overviewRendered (const QImage&)), _overview, SLOT (setImage (const QImage&)));
    //connect (_layer, SIGNAL (imageRefreshed()), this, SLOT (invalidate ()));

    QVBoxLayout* layout = new QVBoxLayout ();
    layout -> addWidget (_mapWidget);
    setLayout (layout);

    _mapWidget -> setZoom (1000);

    setGeometry (200, 200, 640, 480);

    connect (_mapWidget, SIGNAL (viewChanged (const GeoDataLatLonAltBox&)), this, SLOT (notifyViewChanged (const GeoDataLatLonAltBox&)));

}


QNoiseMapExplorer::~QNoiseMapExplorer() {
    if (_mapWidget) { delete _mapWidget; }

}

void QNoiseMapExplorer::changeView () {
    _mapWidget -> changeView();
}

void QNoiseMapExplorer::notifyViewChanged (const GeoDataLatLonAltBox& box) {
    emit viewChanged (box);
}


// return 2 to the power of the given index
int TileProducer::powers (int index) {
    if (index < 0) { return 0; }
    int result = 1;
    result <<= (index - 1);
    return result;
}