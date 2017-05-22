//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008 Torsten Rahn <tackat@kde.org>
//

#include "CalenhadOverviewMap.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QIcon>
#include <QtCore/QThread>
#include <QRgb>
#include <iostream>
#include <marble/MarbleMap.h>

#include "marble/MarbleDirs.h"
#include "marble/MarbleDebug.h"
// #include "OverviewMapConfigWidget.h"

#include "marble/ViewportParams.h"
#include "marble/MarbleWidget.h"
#include "marble/Planet.h"
#include "marble/PlanetFactory.h"
#include "../mapping/CalenhadLayer.h"
#include "CalenhadGlobe.h"




CalenhadOverviewMap::CalenhadOverviewMap ()
        : CalenhadPreview (nullptr) {
}

CalenhadOverviewMap::CalenhadOverviewMap (QModule* module, CalenhadGlobe* globe)
        : CalenhadPreview (module, globe) {
    setFixedSize (2 * height(), height());
}

CalenhadOverviewMap::~CalenhadOverviewMap () {

}

/*
    QDialog* CalenhadOverviewMap::configDialog () {
       /* if (!m_configDialog) {
            // Initializing configuration dialog
            m_configDialog = new QDialog ();
            ui_configWidget = new OverviewMapConfigWidget();
            ui_configWidget->setupUi (m_configDialog);

            connect (ui_configWidget->m_buttonBox, SIGNAL(accepted ()),
                     SLOT(writeSettings ()));
            connect (ui_configWidget->m_buttonBox, SIGNAL(rejected ()),
                     SLOT(readSettings ()));
            connect (ui_configWidget->m_buttonBox->button (QDialogButtonBox::Reset), SIGNAL(clicked ()),
                     SLOT(restoreDefaultSettings ()));
            QPushButton* applyButton = ui_configWidget->m_buttonBox->button (QDialogButtonBox::Apply);
            connect (applyButton, SIGNAL(clicked ()),
                     SLOT(writeSettings ()));
        }
        return m_configDialog;


        return new QDialog(); // for now
    }
*/
void CalenhadOverviewMap::paintEvent (QPaintEvent* e) {
    QPainter painter (this);
    painter.drawPixmap (0, 0, _pixmap);
    drawBoundingBox();
    drawGrid();
}

void CalenhadOverviewMap::drawBoundingBox () {
    // Now draw the latitude longitude bounding _box
    QPainter painter (this);
    qreal xWest = _renderWidth / 2.0 + _renderWidth / (2.0 * M_PI) * _bounds.west ();
    qreal xEast = _renderWidth / 2.0 + _renderWidth / (2.0 * M_PI) * _bounds.east ();
    qreal xNorth = _renderHeight / 2.0 - _renderHeight / M_PI * _bounds.north ();
    qreal xSouth = _renderHeight / 2.0 - _renderHeight / M_PI * _bounds.south ();

    qreal lon = ((CalenhadGlobe*) parent()) -> centre().longitude ();
    qreal lat = ((CalenhadGlobe*) parent()) -> centre().longitude ();;
    GeoDataCoordinates::normalizeLonLat (lon, lat);
    qreal x = _renderWidth / 2.0 + _renderWidth / (2.0 * M_PI) * lon;
    qreal y = _renderHeight / 2.0 - _renderHeight / M_PI * lat;

    painter.setPen (QPen (Qt::white));
    painter.setBrush (QBrush (Qt::transparent));
    painter.setRenderHint (QPainter::Antialiasing, false);

    qreal boxWidth = xEast - xWest;
    qreal boxHeight = xSouth - xNorth;

    qreal minBoxSize = 2.0;
    if (boxHeight < minBoxSize) { boxHeight = minBoxSize; }

    if (_bounds.west () <= _bounds.east ()) {
        // Make sure the latLonBox is still visible
        if (boxWidth < minBoxSize) { boxWidth = minBoxSize; }

        painter.drawRect (QRectF (xWest, xNorth, boxWidth, boxHeight));
    } else {
        // If the dateline is shown in the viewport  and if the poles are not
        // then there are two boxes that represent the latLonBox of the view.

        boxWidth = xEast;

        // Make sure the latLonBox is still visible
        if (boxWidth < minBoxSize) { boxWidth = minBoxSize; }

        painter.drawRect (QRectF (0, xNorth, boxWidth, boxHeight));

        boxWidth = _renderWidth - xWest;

        // Make sure the latLonBox is still visible
        if (boxWidth < minBoxSize) { boxWidth = minBoxSize; }

        painter.drawRect (QRectF (xWest, xNorth, boxWidth, boxHeight));
    }
}

void CalenhadOverviewMap::drawGrid() {
    QPainter painter (this);
    painter.setPen (QPen (Qt::DashLine));
    painter.drawRect (QRectF (QPoint (0, 0), size ()));

    for (int y = 1; y < 4; ++y) {
        if (y == 2) {
            painter.setPen (QPen (Qt::DashLine));
        } else {
            painter.setPen (QPen (Qt::DotLine));
        }

        painter.drawLine (0, (int) (0.25 * y * _renderHeight), _renderWidth, (int) (0.25 * y * _renderHeight));
    }
    for (int x = 1; x < 8; ++x) {
        if (x == 4) {
            painter.setPen (QPen (Qt::DashLine));
        } else {
            painter.setPen (QPen (Qt::DotLine));
        }

        painter.drawLine ((int) (0.125 * x * _renderWidth), 0, (int) (0.125 * x * _renderWidth), _renderHeight);
    }
}

void CalenhadOverviewMap::setBounds (const GeoDataLatLonAltBox& bounds) {
    if (bounds != _bounds) {
        _bounds = bounds;
        render ();
    }
}

void CalenhadOverviewMap::jobComplete (std::shared_ptr<QImage> image) {
    _image = image;
    _pixmap = QPixmap::fromImage (*image);
    recolour();
    _isRendered = true;
    emit renderComplete (image);
}

void CalenhadOverviewMap::recolour() {
    CalenhadGlobe* globe = (CalenhadGlobe*) parent();
    QColor grey;
    QColor rgb;
    int bw;
    for (int x = 0; x < _image -> width(); x++) {
        for (int y = 0; y < _image->height(); y++) {
            if (! (globe -> isInView (toLatLon (QPoint (x, y))))) {
                rgb = QColor (_image -> pixel (x, y));
                bw = (rgb.red() + rgb.green() + rgb.blue()) / 3;
                grey = QColor (bw, bw, bw);
                _image->setPixelColor (x, y, grey);
            }
        }
    }
    _pixmap = QPixmap::fromImage (*_image);
}

bool CalenhadOverviewMap::eventFilter (QObject* object, QEvent* e) {
    if (! isEnabled() || ! isVisible()) {
        return false;
    }

    if (e->type () == QEvent::MouseButtonDblClick || e->type () == QEvent::MouseMove) {
        QMouseEvent* event = static_cast<QMouseEvent*>(e);

        // Double click triggers recentering the map at the specified position
        if (e -> type () == QEvent::MouseButtonDblClick) {



        }

        if (e -> type () == QEvent::MouseMove
            && !(event->buttons () & Qt::LeftButton)) {

            return true;
        }
    }
}

ImageRenderJob* CalenhadOverviewMap::prepareRender() {
    _renderWidth = renderSize().width();
    _renderHeight = renderSize().height();
    ImageRenderJob* job = new ImageRenderJob (GeoDataLatLonBox (-M_PI_2, M_PI_2, 0, M_2_PI), _source -> module(), _source -> legend());
    std::shared_ptr<QImage> image = std::make_shared<QImage> (_renderWidth, _renderHeight, QImage::Format_ARGB32);
    job -> setImage (image);
    _image = image;
    return job;
}


QSize CalenhadOverviewMap::renderSize() {
    return QSize (height() * 2, height());
}


GeoDataCoordinates CalenhadOverviewMap::toLatLon (QPoint pos) {
    double lon = ((double) pos.x() / (double) width()) * 360.0 - 180.0;
    double lat = (180.0 - ((double) pos.y() / (double) height()) * 180.0) - 90.0;
    GeoDataCoordinates latLon;
    latLon.setLatitude (lat, GeoDataCoordinates::Degree);
    latLon.setLongitude (lon, GeoDataCoordinates::Degree);
    return latLon;
}

void CalenhadOverviewMap::mouseDoubleClickEvent (QMouseEvent* e) {
    CalenhadGlobe* globe = (CalenhadGlobe*) parent();
    GeoDataCoordinates latLon = toLatLon (e -> pos());

    globe -> goTo (latLon);
}


