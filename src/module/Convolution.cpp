//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include "Convolution.h"
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

Convolution::Convolution (const QString& type) : Module (type, nullptr),
                                                 _raster (nullptr) {
}

Convolution::~Convolution() {
    delete _raster;
}

bool Convolution::isComplete() {

    if (_raster) {
        return Module::isComplete();
    } else {
        return false;
    }
}

void Convolution::inflate (const QDomElement& element) {
    Module::inflate (element);


}

void Convolution::serialize (QDomElement& element) {
    Module::serialize (element);
}



QString Convolution::glsl () {
   return  "raster (c, %index, %bounds, %0)";
}
