//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include "StructuredGrid.h"
#include "src/CalenhadServices.h"
#include "../controls/QAngleControl.h"
#include "../preferences/PreferencesService.h"
#include "mapping/CalenhadMapWidget.h"
#include "RasterModule.h"
#include "../messages/QNotificationHost.h"
#include <QtMath>



using namespace calenhad::module;
using namespace calenhad;
using namespace calenhad::preferences;
using namespace grid;
using namespace calenhad::controls;
using namespace geoutils;

StructuredGrid::StructuredGrid (const QString& type) : Module (type, nullptr),
                                                       _buffer (new CubicSphere (CalenhadServices::gridSize())) {
}

StructuredGrid::~StructuredGrid() {
    delete _buffer;
}

CubicSphere *StructuredGrid::buffer() const {
    return _buffer;
}

bool StructuredGrid::isComplete() {
    return Module::isComplete();
}

void StructuredGrid::inflate (const QDomElement& element) {
    Module::inflate (element);
}

void StructuredGrid::serialize (QDomElement& element) {
    Module::serialize (element);
}

QString StructuredGrid::glsl() {
   return  "grid (pos, %gridIndex, %gridSize)";
}

int StructuredGrid::rasterSize() {
    return _buffer -> size();
}

void StructuredGrid::setRasterSize (const int& depth) {
    if (depth >= 6 && depth < 13) {  // to do: externalise
        delete _buffer;
        _buffer = new CubicSphere (depth);
    } else {
        CalenhadServices::messages() -> message ("Raster size out of range", "Needs to be between 6 and 12 inclusive");
    }
}