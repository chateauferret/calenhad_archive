//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include "RasterModule.h"
#include "src/CalenhadServices.h"
#include "../controls/QAngleControl.h"
#include "../preferences/PreferencesService.h"
#include "mapping/CalenhadMapWidget.h"
#include "RasterFileModule.h"
#include <QtMath>


using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;
using namespace grid;
using namespace calenhad::controls;
using namespace geoutils;

RasterModule::RasterModule (const QString& type) : Module (type, nullptr),
                                           _raster (nullptr) {
}

RasterModule::~RasterModule() {
    delete _raster;
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

void RasterModule::setBounds (grid::Bounds bounds) {
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
