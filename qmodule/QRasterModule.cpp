//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include "QRasterModule.h"
#include "../CalenhadServices.h"
#include "../controls/QAngleControl.h"
#include "../preferences/PreferencesService.h"
#include "../controls/globe/CalenhadMapView.h"


using namespace calenhad::qmodule;
using namespace calenhad;
using namespace calenhad::preferences;
using namespace icosphere;
using namespace calenhad::controls;
using namespace geoutils;

QRasterModule::QRasterModule (QModule* parent) : QModule (CalenhadServices::preferences() -> calenhad_module_raster, 1, parent),
    _raster (nullptr),
    _filename (QString::null) {
    initialise();
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

    _bounds = Bounds();
    _boundsLayout = new QGridLayout();
    _boundsLayout->setSpacing(0);
    _boundsLayout->setMargin(0);
    _boundsLayout->setContentsMargins(0,0,0,0);
    _boundsContent = new QWidget (_expander);
    _boundsContent -> setFixedSize (300, 150);
    _boundsContent -> setLayout (_boundsLayout);
    addPanel ("Bounds", _boundsContent);
    _northBoundsText = new QAngleControl ("North", AngleType::Latitude);
    _boundsLayout -> addWidget (_northBoundsText, 1, 1, 1, 2, Qt::AlignCenter);
    _westBoundsText = new QAngleControl ("West", AngleType::Longitude);
    _boundsLayout -> addWidget (_westBoundsText, 2, 0, 1, 2, Qt::AlignCenter);
    _eastBoundsText = new QAngleControl ("East", AngleType::Longitude);
    _boundsLayout -> addWidget (_eastBoundsText, 2, 2, 1, 2, Qt::AlignCenter);
    _southBoundsText = new QAngleControl ("South", AngleType::Latitude);
    _boundsLayout -> addWidget (_southBoundsText, 3, 1, 1, 2, Qt::AlignCenter);

    _rasterLayout = new QFormLayout();
    _rasterContent = new QWidget (_expander);
    _rasterContent -> setLayout (_rasterLayout);
    _rasterLayout->setContentsMargins (5, 0, 5, 0);
    _rasterLayout->setVerticalSpacing (0);
    addPanel ("Raster", _rasterContent);

    connect (_northBoundsText, &QAngleControl::valueChanged, this, &QRasterModule::updateBounds);
    connect (_westBoundsText, &QAngleControl::valueChanged, this, &QRasterModule::updateBounds);
    connect (_eastBoundsText, &QAngleControl::valueChanged, this, &QRasterModule::updateBounds);
    connect (_southBoundsText, &QAngleControl::valueChanged, this, &QRasterModule::updateBounds);

    setBounds (_bounds);

    _filenameLabel = new QLabel (this);
    _filenameLabel -> setMinimumSize (QSize (160, 80));
    QPushButton* selectFileButton = new QPushButton (this);
    selectFileButton -> setFixedSize (100, 20);
    selectFileButton -> setText ("Select image...");
    _rasterLayout -> addWidget (_filenameLabel);
    _rasterLayout -> addWidget (selectFileButton);
    connect (selectFileButton, &QAbstractButton::pressed, this, &QRasterModule::fileDialogRequested);

}

void QRasterModule::setRaster (const QImage& raster) {
    if (! raster.isNull()) {
        QImage p = raster.scaled (CalenhadServices::preferences() -> calenhad_globe_texture_height, CalenhadServices::preferences() -> calenhad_globe_texture_height);
        if (_raster) {
            delete _raster;
        }
        _raster = new QImage (p);
        QPixmap pixmap = QPixmap::fromImage (p).scaled (_filenameLabel -> size());
        _filenameLabel -> setPixmap (pixmap);
        _filenameLabel -> setToolTip (_filename);
        _expander -> setItemEnabled (_previewIndex, isComplete());
        invalidate();
    } else {

    }
}

QImage* QRasterModule::raster() {
    return _raster;
}

void QRasterModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName ();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    QPixmap pixmap (filename);
    openFile (filename);
}

void QRasterModule::openFile (const QString& filename) {
    QImage raster = QImage (filename);
    _filename = filename;
    setRaster (raster);
}

bool QRasterModule::isComplete() {

    if (_raster && isBoundsValid()) {
        return QModule::isComplete();
    } else {
        return false;
    }
}

bool QRasterModule::isBoundsValid() {
    return _northBoundsText -> isValid() && _southBoundsText -> isValid() && _eastBoundsText -> isValid() && _westBoundsText -> isValid();
}

void QRasterModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    QDomElement rasterElement = element.firstChildElement ("filename");
    QString filename = rasterElement.attribute ("filename");
    QDomElement boundsElement = element.firstChildElement ("bounds");
    bool ok = false;
    double north = boundsElement.attribute ("north").toDouble (&ok);
    double south = boundsElement.attribute ("south").toDouble (&ok);
    double west = boundsElement.attribute ("west").toDouble (&ok);
    double east = boundsElement.attribute ("east").toDouble (&ok);
    Bounds bounds;
    if (ok) {
        bounds = Bounds (north, south, east, west);
    } else {
        bounds = Bounds();
    }
    setBounds (bounds);
    openFile (filename);
}

void QRasterModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    QDomElement rasterElement = _document.createElement ("filename");
    rasterElement.setAttribute ("filename", _filename);
    QDomElement boundsElement = _document.createElement ("bounds");
    boundsElement.setAttribute ("north", _bounds.north());
    boundsElement.setAttribute ("south", _bounds.south());
    boundsElement.setAttribute ("west", _bounds.west());
    boundsElement.setAttribute ("east", _bounds.east());
    _element.appendChild (rasterElement);
    _element.appendChild (boundsElement);
}

Bounds QRasterModule::bounds() {
    return _bounds;
}

void QRasterModule::updateBounds() {
    _bounds = Bounds (_northBoundsText -> value(), _southBoundsText -> value(), _eastBoundsText -> value(), _westBoundsText -> value(), geoutils::Units::Degrees);
    invalidate();
}

void QRasterModule::setBounds (icosphere::Bounds bounds) {
    _bounds = bounds;

    _northBoundsText -> blockSignals (true);
    _westBoundsText -> blockSignals (true);
    _eastBoundsText -> blockSignals (true);
    _southBoundsText -> blockSignals (true);

    double north = qRadiansToDegrees (_bounds.north());
    _northBoundsText -> setValue (north);
    _northBoundsText -> refresh();

    double west = qRadiansToDegrees (_bounds.west());
    _westBoundsText -> setValue (west);
    _westBoundsText -> refresh();

    double east = qRadiansToDegrees (_bounds.east());
    _eastBoundsText -> setValue (east);
    _eastBoundsText -> refresh();

    double south = qRadiansToDegrees (_bounds.south());
    _southBoundsText -> setValue (south);
    _southBoundsText -> refresh();

    _northBoundsText -> blockSignals (false);
    _westBoundsText -> blockSignals (false);
    _eastBoundsText -> blockSignals (false);
    _southBoundsText -> blockSignals (false);
}
