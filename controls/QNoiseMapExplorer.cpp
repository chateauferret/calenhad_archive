//
// Created by martin on 05/01/17.
//

#include "QNoiseMapExplorer.h"
#include <marble/HttpDownloadManager.h>
#include <marble/MarbleModel.h>
#include <marble/MarbleMap.h>
#include <marble/GeoDataTreeModel.h>
#include "../pipeline/QColorGradient.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

QNoiseMapExplorer::QNoiseMapExplorer (const QString& title, const QWidget* parent) : QDialog(), _gradient (new QColorGradient()) {

    _mapWidget = new CalenhadMarbleWidget (this);
    //_mapWidget->setProjection(Marble::Mercator);
    _mapWidget -> setMapThemeId("/home/martin/.config/calenhad/globe.dgml");

    Marble::MarbleModel* model = _mapWidget -> model();
    _mapWidget -> setWindowTitle (title);
     QVBoxLayout* layout = new QVBoxLayout();
     layout -> addWidget (_mapWidget);
     setLayout (layout);

}

QNoiseMapExplorer::~QNoiseMapExplorer () {
    if (_mapWidget) { delete _mapWidget; }
    if (_producer) { delete _producer; }
    if (_source) { delete _source; }
    if (_layer) { delete _layer; }
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

