//
// Created by martin on 11/11/16.
//

#include <iostream>
#include "ModuleFactory.h"
#include "../qmodule/QNode.h"


using namespace noise::module;

ModuleFactory::ModuleFactory() {
    makeRepository();
}

ModuleFactory:: ~ModuleFactory() {
    for (QModule* module : _repository.values ()) {
        delete module;
    }
}

QModule* ModuleFactory::createModule (const ModuleType& moduleType, CalenhadModel* model) {

    QModule* m = _repository.find (moduleType).value();
    if (m) {
        QModule* newModule = m -> addCopy (model);
        return newModule;
    } else {
        std::cout << "No module found\n";
        return nullptr;
    }
}

/*
    Name		Inputs   Ctrls	Outputs	                    Parameters

    Curve			1	0	    1	controlpoints           ControlPoint* controlPoints { double inputValue, double outputValue }
    Terrace			1	0	    1	controlpoints           ControlPoint* controlPoints { double inputValue, double outputValue }, bool isTerracesInverted

    Populate the default module name

*/

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

void ModuleFactory::makeRepository() {
    for (QModule* module : _repository.values()) { delete module; }
    for (QModule* module : _roster) { delete module; }
    _repository.clear();
    _repository.insert (ModuleType::CYLINDERS, QCylindersModule::newInstance());
    _repository.insert (ModuleType::SPHERES, QSpheresModule::newInstance());
    _repository.insert (ModuleType::EXPONENT, QExponentModule::newInstance());
    _repository.insert (ModuleType::TRANSLATE, QTranslateModule::newInstance());
    _repository.insert (ModuleType::ROTATE, QRotateModule::newInstance());
    _repository.insert (ModuleType::CLAMP, QClampModule::newInstance ());
    _repository.insert (ModuleType::CONSTANT, QConstModule::newInstance ());
    _repository.insert (ModuleType::ADD, QBasicModule::newInstance (ModuleType::ADD));
    _repository.insert (ModuleType::ABS, QBasicModule::newInstance (ModuleType::ABS));
    _repository.insert (ModuleType::BLEND,QBasicModule::newInstance (ModuleType::BLEND));
    _repository.insert (ModuleType::CACHE, QBasicModule::newInstance (ModuleType::CACHE));
    _repository.insert (ModuleType::CHECKERBOARD, QBasicModule::newInstance (ModuleType::CHECKERBOARD));
    _repository.insert (ModuleType::INVERT, QBasicModule::newInstance (ModuleType::INVERT));
    _repository.insert (ModuleType::MAX, QBasicModule::newInstance (ModuleType::MAX));
    _repository.insert (ModuleType::MIN, QBasicModule::newInstance (ModuleType::MIN));
    _repository.insert (ModuleType::MULTIPLY, QBasicModule::newInstance (ModuleType::MULTIPLY));
    _repository.insert (ModuleType::POWER, QBasicModule::newInstance (ModuleType::POWER));
    _repository.insert (ModuleType::DISPLACE, QBasicModule::newInstance (ModuleType::DISPLACE));
    _repository.insert (ModuleType::DIFF, QBasicModule::newInstance (ModuleType::DIFF));
    _repository.insert (ModuleType::PERLIN, QNoiseModule::newPerlinInstance());
    _repository.insert (ModuleType::BILLOW, QNoiseModule::newBillowInstance());
    _repository.insert (ModuleType::RIDGEDMULTI, QNoiseModule::newRidgedMultiInstance());
    _repository.insert (ModuleType::SCALEBIAS, QScaleBiasModule::newInstance());
    _repository.insert (ModuleType::SELECT, QSelectModule::newInstance());
    _repository.insert (ModuleType::TURBULENCE, QTurbulenceModule::newInstance());
    _repository.insert (ModuleType::VORONOI, QVoronoiModule::newInstance());
    _repository.insert (ModuleType::SCALEPOINT, QScaleModule::newInstance());

}

