//
// Created by martin on 07/10/2019.
//

#include <QtWidgets/QFileDialog>
#include <QtGui/QPainter>
#include "RasterModule.h"
#include "geoutils.h"
#include <QApplication>
#include "../messages/QProgressNotification.h"


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;
using namespace calenhad::mapping;
using namespace calenhad::grid;
using namespace geoutils;
using namespace calenhad::notification;

RasterModule::RasterModule (const QString& type) : StructuredGrid (type), _buffer (nullptr),
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
    connect (selectFileButton, &QAbstractButton::pressed, this, [this] () { fileDialogRequested(); });
}

void RasterModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    openFile (filename);
}

void RasterModule::openFile (const QString& filename) {
    if (! filename.isNull() && ! filename.isEmpty()) {
        QApplication::setOverrideCursor (QCursor (Qt::WaitCursor));
        QImage* raster = new QImage (filename);
        _filename = filename;
        _filenameLabel->setToolTip (_filename);
        QPixmap pixmap = QPixmap::fromImage (*raster).scaled (_filenameLabel -> size());
        assimilateRaster (raster);
        _filenameLabel -> setPixmap (pixmap);
        invalidate();
        QApplication::restoreOverrideCursor();
    }
}

void RasterModule::inflate (const QDomElement& element) {
    StructuredGrid::inflate (element);
    QDomElement rasterElement = element.firstChildElement ("filename");
    QString filename = rasterElement.attribute ("filename");
    openFile (filename);
}

void RasterModule::serialize (QDomElement& element) {
    StructuredGrid::serialize (element);
    QDomElement rasterElement = _document.createElement ("filename");
    rasterElement.setAttribute ("filename", _filename);
    _element.appendChild (rasterElement);
}

CubicSphere* RasterModule::raster() {
    return  _buffer;
}

void RasterModule::fetch (CubicSphere* buffer) {
    buffer -> copy (_buffer);
}

bool RasterModule::isComplete() {
    if (_buffer) {
        return StructuredGrid::isComplete ();
    } else {
        return false;
    }
}

QString RasterModule::glsl() {
    return  "grid (%gridIndex); }";
}

void RasterModule::assimilateRaster (QImage* image) {
    if (! _buffer) {
        _buffer = new CubicSphere (CalenhadServices::preferences() -> calenhad_compute_gridsize);
    }
    if (image) {
        _buffer->fromRaster (image);
    }
}
