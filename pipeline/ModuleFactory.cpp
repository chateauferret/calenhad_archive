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
#include "../qmodule/QNodeGroup.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../qmodule/QAltitudeMap.h"
#include "../CalenhadServices.h"
#include "../exprtk/ExpressionWidget.h"
#include <QPixmap>
#include <QList>


using namespace noise::module;
using namespace calenhad;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::expressions;

ModuleFactory::ModuleFactory() {
    for (QString type : types()) {
        QString icon = type.toLower();
        icon.replace (" ", "");
        QPixmap* pixmap = new QPixmap (":/appicons/tools/" + icon + ".png");
        _icons.insert (type, pixmap);
    }
}

ModuleFactory:: ~ModuleFactory() {
    for (QPixmap* icon : _icons) { delete icon; }
}

QPixmap* ModuleFactory::getIcon (const QString& type) {
   return _icons.value (type);

}

QStringList ModuleFactory::types () {
    QStringList list;
    list    << CalenhadServices::preferences() -> calenhad_module_abs
            << CalenhadServices::preferences() -> calenhad_module_add
            << CalenhadServices::preferences() -> calenhad_module_altitudemap
            << CalenhadServices::preferences() -> calenhad_module_billow
            << CalenhadServices::preferences() -> calenhad_module_blend
            << CalenhadServices::preferences() -> calenhad_module_cache
            << CalenhadServices::preferences() -> calenhad_module_checkerboard
            << CalenhadServices::preferences() -> calenhad_module_clamp
            << CalenhadServices::preferences() -> calenhad_module_constant
            << CalenhadServices::preferences() -> calenhad_module_cylinders
            << CalenhadServices::preferences() -> calenhad_module_diff
            << CalenhadServices::preferences() -> calenhad_module_displace
            << CalenhadServices::preferences() -> calenhad_module_exponent
            << CalenhadServices::preferences() -> calenhad_module_icospheremap
            << CalenhadServices::preferences() -> calenhad_module_invert
            << CalenhadServices::preferences() -> calenhad_module_max
            << CalenhadServices::preferences() -> calenhad_module_min
            << CalenhadServices::preferences() -> calenhad_module_multiply
            << CalenhadServices::preferences() -> calenhad_nodegroup
            << CalenhadServices::preferences() -> calenhad_module_power
            << CalenhadServices::preferences() -> calenhad_module_perlin
            << CalenhadServices::preferences() -> calenhad_module_scalepoint
            << CalenhadServices::preferences() -> calenhad_module_spheres
            << CalenhadServices::preferences() -> calenhad_module_ridgedmulti
            << CalenhadServices::preferences() -> calenhad_module_rotate
            << CalenhadServices::preferences() -> calenhad_module_scalebias
            << CalenhadServices::preferences() -> calenhad_module_select
            << CalenhadServices::preferences() -> calenhad_module_translate
            << CalenhadServices::preferences() -> calenhad_module_turbulence
            << CalenhadServices::preferences() -> calenhad_module_voronoi;

    return list;
}

QNode* ModuleFactory::createModule (const QString& type, CalenhadModel* model) {

    if (type == CalenhadServices::preferences() -> calenhad_module_abs ||
     type == CalenhadServices::preferences() -> calenhad_module_add ||
     type == CalenhadServices::preferences() -> calenhad_module_blend ||
     type == CalenhadServices::preferences() -> calenhad_module_cache ||
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
    if (type == CalenhadServices::preferences() -> calenhad_module_constant) {
        QModule* qm = new QModule (CalenhadServices::preferences() -> calenhad_module_constant, new Const());
        qm -> addParameter ("Constant value", "constValue", 0.01, [=] (const double& value) {
            ((Const*) qm -> module()) -> SetConstValue (value);
        });
        return qm;
    }
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
    if (type == CalenhadServices::preferences() -> calenhad_nodegroup) { QNodeGroup* group = new QNodeGroup(); return group; }
    return nullptr;
}

QNode* ModuleFactory::clone (QNode* other) {
    QNode* n = createModule (other -> nodeType(), other -> model());
    for (QString key : n -> parameters()) {
        n -> setParameter (key, other -> parameter (key));
    }
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
