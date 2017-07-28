//
// Created by martin on 05/01/17.
//

#include "QNoiseMapExplorer.h"
#include <marble/MarbleMap.h>
#include <marble/GeoDataGroundOverlay.h>
#include <marble/GeoDataDocument.h>
#include "controls/globe/CalenhadOverviewMap.h"
#include <marble/MarbleWidgetInputHandler.h>
#include <QThread>
#include "CalenhadGlobe.h"
#include <QVBoxLayout>
#include "../../qmodule/QModule.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace noise::module;
using namespace Marble;
using namespace icosphere;
using namespace calenhad::qmodule;
using namespace calenhad::controls::globe;

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

    connect (_mapWidget, &CalenhadGlobe::viewChanged, this, &QNoiseMapExplorer::updateView);

}


QNoiseMapExplorer::~QNoiseMapExplorer() {
    if (_mapWidget) { delete _mapWidget; }

}

void QNoiseMapExplorer::changeView () {
    _mapWidget -> changeView();
}

void QNoiseMapExplorer::updateView (const Bounds& bounds) {
    emit viewChanged (bounds);
}
