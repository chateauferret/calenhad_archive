//
// Created by martin on 05/01/17.
//

#include "QNoiseMapExplorer.h"
#include <marble/HttpDownloadManager.h>
#include <marble/MarbleModel.h>
#include <marble/MarbleMap.h>
#include <marble/GeoDataTreeModel.h>
#include "../libnoiseutils/GradientLegend.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace noise::utils;

QNoiseMapExplorer::QNoiseMapExplorer (const QString& title, const QWidget* parent) : QDialog(), _gradient (new GradientLegend()) {

    _mapWidget = new CalenhadMarbleWidget (this);

    std::cout << _mapWidget -> mapThemeId ().toStdString () << "\n";
    _mapWidget -> setWindowTitle (title);
     QVBoxLayout* layout = new QVBoxLayout();
     layout -> addWidget (_mapWidget);
     setLayout (layout);
    _mapWidget -> setZoom (1000);
    setGeometry (200, 200, 640, 480);
}

QNoiseMapExplorer::~QNoiseMapExplorer () {
    if (_mapWidget) { delete _mapWidget; }
    if (_producer) { delete _producer; }
    if (_source) { delete _source; }
    if (_layer) { delete _layer; }
    if (_gradient) { delete _gradient; }
}

void QNoiseMapExplorer::setSource (noise::module::Module* source) {
    if (source) {
        if (_source != source) {
            _source = source;
            _mapWidget -> removeLayer (_layer);
            if (_layer) { delete _layer; }
            _layer = new CalenhadLayer (_source);
            _layer -> setGradient (_gradient);
            _mapWidget -> addLayer (_layer);
        }
    }
}

