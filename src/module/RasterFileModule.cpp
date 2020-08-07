//
// Created by martin on 07/10/2019.
//

#include <QtWidgets/QFileDialog>
#include <QtGui/QPainter>
#include "RasterFileModule.h"


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;

RasterFileModule::RasterFileModule (const QString& type) : Convolution(),
                                                           _filename (QString::null),
                                                           _rasterLayout (new QFormLayout()) {
    _rasterContent = new QWidget (_expander);
    _rasterContent -> setLayout (_rasterLayout);
    _rasterLayout->setContentsMargins (5, 0, 5, 0);
    _rasterLayout->setVerticalSpacing (0);
    addPanel ("Raster", _rasterContent);
    _filenameLabel = new QLabel (this);
    _filenameLabel -> setMinimumSize (QSize (160, 80));
    QPushButton* selectFileButton = new QPushButton (this);
    selectFileButton -> setFixedSize (100, 20);
    selectFileButton -> setText ("Select image...");
    _rasterLayout -> addWidget (_filenameLabel);
    _rasterLayout -> addWidget (selectFileButton);
    connect (selectFileButton, &QAbstractButton::pressed, this, &RasterFileModule::fileDialogRequested);
}

void calenhad::module::RasterFileModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName ();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    openFile (filename);
}

void calenhad::module::RasterFileModule::openFile (const QString& filename) {
    delete _raster;
    _raster = new QImage (filename);
    _filename = filename;
    _filenameLabel -> setToolTip (_filename);
    QPixmap pixmap = QPixmap::fromImage (*_raster).scaled (_filenameLabel -> size());
    _filenameLabel -> setPixmap (pixmap);
    invalidate();

}

void RasterFileModule::inflate (const QDomElement& element) {
    Convolution::inflate (element);
    QDomElement rasterElement = element.firstChildElement ("filename");
    QString filename = rasterElement.attribute ("filename");
    openFile (filename);
}

void RasterFileModule::serialize (QDomElement& element) {
    Convolution::serialize (element);
    QDomElement rasterElement = _document.createElement ("filename");
    rasterElement.setAttribute ("filename", _filename);
    _element.appendChild (rasterElement);
}

QImage* RasterFileModule::raster() {
    return  _raster;
}
