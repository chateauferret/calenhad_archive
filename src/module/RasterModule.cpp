//
// Created by martin on 07/10/2019.
//

#include <QtWidgets/QFileDialog>
#include <QtGui/QPainter>
#include "RasterModule.h"


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;

RasterModule::RasterModule (const QString& type) : Module (type), _raster (nullptr),
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
    connect (selectFileButton, &QAbstractButton::pressed, this, &RasterModule::fileDialogRequested);
}

void calenhad::module::RasterModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName ();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    openFile (filename);
}

void calenhad::module::RasterModule::openFile (const QString& filename) {
    delete _raster;
    _raster = new QImage (filename);
    _filename = filename;
    _filenameLabel -> setToolTip (_filename);
    QPixmap pixmap = QPixmap::fromImage (*_raster).scaled (_filenameLabel -> size());
    _filenameLabel -> setPixmap (pixmap);
    invalidate();

}

void RasterModule::inflate (const QDomElement& element) {
    Module::inflate (element);
    QDomElement rasterElement = element.firstChildElement ("filename");
    QString filename = rasterElement.attribute ("filename");
    openFile (filename);
}

void RasterModule::serialize (QDomElement& element) {
    Module::serialize (element);
    QDomElement rasterElement = _document.createElement ("filename");
    rasterElement.setAttribute ("filename", _filename);
    _element.appendChild (rasterElement);
}

QImage* RasterModule::raster() {
    return  _raster;
}

bool RasterModule::isComplete() {
    if (_raster) {
        return Module::isComplete();
    } else {
        return false;
    }
}

QString RasterModule::glsl() {
    return  "raster (c, %rasterIndex, %rasterSize)";
}
