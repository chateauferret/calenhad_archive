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
    _mapWidget->setShowBackground (false);

    // add our own overview map
    _overview = new CalenhadOverviewMap (_source, _mapWidget);
    _overview -> initialise();
    _overview -> setFixedSize (210, 105);


    // remove the map components we don't want - scale and overviewmap both relate to Earth
            foreach (AbstractFloatItem* item, _mapWidget -> floatItems()) {
            if (item && (item -> nameId() == "overviewmap" || item -> nameId() == "scalebar")) {
                item->setVisible (false);
            }
        }

    std::cout << _mapWidget->mapThemeId ().toStdString () << "\n";
    _mapWidget->setWindowTitle (title);
    _layer = new CalenhadLayer (source);
    _mapWidget -> addLayer (_layer);
    //connect (_layer, SIGNAL (overviewRendered (const QImage&)), _overview, SLOT (setImage (const QImage&)));
    connect (_layer, SIGNAL (imageRefreshed()), this, SLOT (invalidate ()));

    QVBoxLayout* layout = new QVBoxLayout ();
    layout -> addWidget (_mapWidget);
    setLayout (layout);

    _mapWidget->setZoom (1000);

    setGeometry (200, 200, 640, 480);

    connect (_mapWidget, &MarbleWidget::visibleLatLonAltBoxChanged, this, &QNoiseMapExplorer::changeView);
    connect (_mapWidget, SIGNAL (resized (const QSize&)), _layer, SLOT (rescale()));
    connect (&timer, SIGNAL (timeout()), this, SLOT (invalidate()));
    connect (_source, SIGNAL (nodeChanged()), this, SLOT (invalidate()));

}


QNoiseMapExplorer::~QNoiseMapExplorer() {
    if (_overview) { delete _overview; }
    if (_mapWidget) { delete _mapWidget; }
    if (_layer) { delete _layer; }
    if (_gradient) { delete _gradient; }
    if (_layer) { delete _layer; }
}

void QNoiseMapExplorer::invalidate () {
    _mapWidget -> update();
    _overview -> update();
}

void QNoiseMapExplorer::changeView() {
    GeoDataLatLonAltBox bounds = _mapWidget -> viewport() -> viewLatLonAltBox();
    _overview -> setBounds (bounds);
    _overview -> render();
    emit viewChanged (bounds);
    _bounds = bounds;
    _layer -> rescale();
    timer.setSingleShot (true);
    timer.start();
}

// return 2 to the power of the given index
int TileProducer::powers (int index) {
    if (index < 0) { return 0; }
    int result = 1;
    result <<= (index - 1);
    return result;
}