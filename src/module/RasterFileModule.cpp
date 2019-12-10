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
initialise ();
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

void RasterFileModule::initialise () {
    RasterModule::initialise ();

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
            _expander->setItemEnabled (_previewIndex, isComplete ());
            invalidate ();
            _filenameLabel->setToolTip (_filename);
        }
}

QImage* RasterFileModule::raster() {
    return _raster;
}
