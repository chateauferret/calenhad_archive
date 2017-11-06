//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include "QRasterModule.h"
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::qmodule;
using namespace calenhad;
using namespace calenhad::preferences;

QRasterModule::QRasterModule (QModule* parent) : QModule (CalenhadServices::preferences() -> calenhad_module_raster, 0, parent),
    _raster (nullptr),
    _filename (QString::null) {

}

QRasterModule::~QRasterModule() {
    if (_raster) { delete _raster; }
}


/// Initialise a QRasterModule ready for use. Creates the UI.
void QRasterModule::initialise() {
    QModule::initialise();
    // create a panel to hold the parameter widgets, if we haven't done this already
    if (! (_content)) {
        addContentPanel();
    }
    QWidget* filenameWidget = new QWidget (_content);
    QHBoxLayout* filenameLayout = new QHBoxLayout();
    filenameWidget -> setLayout (filenameLayout);
    QLabel* filenameLabel = new QLabel (this);
    QPushButton* selectFileButton = new QPushButton (this);
    filenameLayout -> addWidget (filenameWidget);
    filenameLayout -> addWidget (selectFileButton);
    QLabel* label = new QLabel (this);
    label -> setText ("Filename");
    _contentLayout -> addRow (label, filenameWidget);
    connect (selectFileButton, &QAbstractButton::pressed, this, &QRasterModule::fileDialogRequested);
}

void QRasterModule::setRaster (const QPixmap& raster) {
    if (! raster.isNull()) {
        QPixmap p;
        int h, w;
        if (raster.height() > raster.width() / 2) {
            h = raster.width() / 2;
            p = raster.scaledToHeight (h);
        } else {
            w = raster.height() * 2;
            p = raster.scaledToWidth (w);
        }
        if (_raster) {
            delete _raster;
        }
        p = p.scaled (CalenhadServices::preferences() -> calenhad_globe_texture_height * 2, CalenhadServices::preferences() -> calenhad_globe_texture_height);
        _raster = new QPixmap (p);
    } else {

    }
}

QPixmap* QRasterModule::raster() {
    return _raster;
}

void QRasterModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName ();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    openFile (filename);
}

void QRasterModule::openFile (const QString& filename) {
    QPixmap raster = QPixmap (filename);
    setRaster (raster);
}

