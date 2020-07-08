//
// Created by martin on 07/10/2019.
//

#include <QtWidgets/QFileDialog>
#include "RasterFileModule.h"


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;

RasterFileModule::RasterFileModule (const QString& type) : RasterModule(),
                               _filename (QString::null),
                                _rasterLayout (new QFormLayout()) {
}

void calenhad::module::RasterFileModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName ();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    QPixmap pixmap (filename);
    openFile (filename);
}

void calenhad::module::RasterFileModule::openFile (const QString& filename) {
    QImage raster = QImage (filename);
    _filename = filename;
    setRaster (raster);
}

void RasterFileModule::inflate (const QDomElement& element) {
    RasterModule::inflate (element);
    QDomElement rasterElement = element.firstChildElement ("filename");
    QString filename = rasterElement.attribute ("filename");
    openFile (filename);
}

void RasterFileModule::serialize (QDomElement& element) {
    RasterModule::serialize (element);
    QDomElement rasterElement = _document.createElement ("filename");
    rasterElement.setAttribute ("filename", _filename);
    _element.appendChild (rasterElement);
}

void RasterFileModule::setRaster (const QImage& raster) {
        if (! raster.isNull()) {
            QImage p = raster.scaled (CalenhadServices::preferences ()->calenhad_globe_texture_height, CalenhadServices::preferences ()->calenhad_globe_texture_height);
            delete _raster;

            _raster = new QImage (p);
            QPixmap pixmap = QPixmap::fromImage (p).scaled (_filenameLabel->size ());
            _filenameLabel->setPixmap (pixmap);
            invalidate ();
            _filenameLabel->setToolTip (_filename);
        }
}

QImage* RasterFileModule::raster() {
    return _raster;
}
