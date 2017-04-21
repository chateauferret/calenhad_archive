//
// Created by martin on 11/11/16.
//

#include <iostream>
#include "ModuleFactory.h"
#include "../libnoiseutils/diff.h"
#include "../qmodule/QClampModule.h"
#include "../qmodule/QConstModule.h"
#include "../qmodule/QNoiseModule.h"
#include "../qmodule/QCylindersModule.h"
#include "../qmodule/QSpheresModule.h"
#include "../qmodule/QExponentModule.h"
#include "../qmodule/QTranslateModule.h"
#include "../qmodule/QScaleBiasModule.h"
#include "../qmodule/QSelectModule.h"
#include "../qmodule/QTurbulenceModule.h"
#include "../qmodule/QVoronoiModule.h"
#include "../qmodule/QRotateModule.h"
#include "../qmodule/QScaleModule.h"
#include "../qmodule/QBasicModule.h"
#include "../qmodule/QIcosphereMap.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"
#include "../qmodule/QAltitudeMap.h"


using namespace noise::module;

ModuleFactory::ModuleFactory() {

}

ModuleFactory:: ~ModuleFactory() {

}

QModule* ModuleFactory::createModule (const QString& type, CalenhadModel* model) {



/*
    Name		Inputs   Ctrls	Outputs	                    Parameters

    Curve			1	0	    1	controlpoints           ControlPoint* controlPoints { double inputValue, double outputValue }
    Terrace			1	0	    1	controlpoints           ControlPoint* controlPoints { double inputValue, double outputValue }, bool isTerracesInverted

    Populate the default module name

*/
    if (type == CalenhadServices::preferences() -> calenhad_module_abs ||
     type == CalenhadServices::preferences() -> calenhad_module_add ||
     type == CalenhadServices::preferences() -> calenhad_module_blend ||
     type == CalenhadServices::preferences() -> calenhad_module_cache||
     type == CalenhadServices::preferences() -> calenhad_module_checkerboard ||
     type == CalenhadServices::preferences() -> calenhad_module_invert ||
     type == CalenhadServices::preferences() -> calenhad_module_max ||
     type == CalenhadServices::preferences() -> calenhad_module_min ||
     type == CalenhadServices::preferences() -> calenhad_module_multiply ||
     type == CalenhadServices::preferences() -> calenhad_module_power ||
     type == CalenhadServices::preferences() -> calenhad_module_displace ||
     type == CalenhadServices::preferences() -> calenhad_module_diff) { return QBasicModule::newInstance (type); }

    if (type == CalenhadServices::preferences() -> calenhad_module_cylinders) { return QCylindersModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_spheres) { return QSpheresModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_exponent) { return QExponentModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_translate) { return QTranslateModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_rotate) { return QRotateModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_clamp) { return QClampModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_constant) { return QConstModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_perlin) { return QNoiseModule::newPerlinInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_billow) { return QNoiseModule::newBillowInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_ridgedmulti) { return QNoiseModule::newRidgedMultiInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_scalebias) { return QScaleBiasModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_select) { return QSelectModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_turbulence) { return QTurbulenceModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_voronoi) { return QVoronoiModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_scalepoint) { return QScaleModule::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_icospheremap) { return QIcosphereMap::newInstance(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_altitudemap) { return QAltitudeMap::newInstance(); }

}

    void ModuleFactory::setSeed (const int& seed) {
        _seed = seed;
        emit seedChanged (seed);
    }

    void ModuleFactory::setNoiseQuality (const noise::NoiseQuality& noiseQuality) {
        _noiseQuality = noiseQuality;
        emit noiseQualityChanged (noiseQuality);
    }

    int ModuleFactory::seed () {
        return _seed;
    }

    noise::NoiseQuality ModuleFactory::noiseQuality () {
        return _noiseQuality;
    }
