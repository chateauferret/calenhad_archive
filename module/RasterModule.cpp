//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include "RasterModule.h"
#include "../CalenhadServices.h"
#include "../controls/QAngleControl.h"
#include "../preferences/PreferencesService.h"
#include "mapping/CalenhadMapWidget.h"
#include "RasterFileModule.h"


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;
using namespace icosphere;
using namespace calenhad::controls;
using namespace geoutils;

RasterModule::RasterModule (const QString& type) : Module (type, nullptr),
                                           _raster (nullptr) {
}

RasterModule::~RasterModule() {
    delete _raster;
}

/// Initialise a QRasterModule ready for use. Creates the UI.
void RasterModule::initialise() {
    Module::initialise();
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



    connect (_northBoundsText, &QAngleControl::valueChanged, this, &RasterModule::updateBounds);
    connect (_westBoundsText, &QAngleControl::valueChanged, this, &RasterModule::updateBounds);
    connect (_eastBoundsText, &QAngleControl::valueChanged, this, &RasterModule::updateBounds);
    connect (_southBoundsText, &QAngleControl::valueChanged, this, &RasterModule::updateBounds);

    setBounds (_bounds);


}


QImage* RasterModule::raster() {
    return _raster;
}

bool RasterModule::isComplete() {

    if (_raster && isBoundsValid()) {
        return Module::isComplete();
    } else {
        return false;
    }
}

bool RasterModule::isBoundsValid() {
    return _northBoundsText -> isValid() && _southBoundsText -> isValid() && _eastBoundsText -> isValid() && _westBoundsText -> isValid();
}

void RasterModule::inflate (const QDomElement& element) {
    Module::inflate (element);

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
}

void RasterModule::serialize (QDomElement& element) {
    Module::serialize (element);
    QDomElement boundsElement = _document.createElement ("bounds");
    boundsElement.setAttribute ("north", _bounds.north());
    boundsElement.setAttribute ("south", _bounds.south());
    boundsElement.setAttribute ("west", _bounds.west());
    boundsElement.setAttribute ("east", _bounds.east());
    _element.appendChild (boundsElement);
}

Bounds RasterModule::bounds() {
    return _bounds;
}

void RasterModule::updateBounds() {
    _bounds = Bounds (_northBoundsText -> value(), _southBoundsText -> value(), _eastBoundsText -> value(), _westBoundsText -> value(), geoutils::Units::Degrees);
    invalidate();
}

void RasterModule::setBounds (icosphere::Bounds bounds) {
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

QString RasterModule::glsl () {
   return  "raster (c, %index, %bounds, %0)";
}
