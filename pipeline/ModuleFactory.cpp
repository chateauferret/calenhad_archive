//
// Created by martin on 11/11/16.
//

#include <iostream>
#include "ModuleFactory.h"
#include "../libnoiseutils/diff.h"
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

QNode* ModuleFactory::createModule (const QString& type) {

    if (type == CalenhadServices::preferences() -> calenhad_module_abs) { return new QModule (type, new Abs()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_add ) { return new QModule (type, new Add()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_blend ) { return new QModule (type, new Blend()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_cache ) { return new QModule (type, new Cache()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_checkerboard ) { return new QModule (type, new Checkerboard()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_invert ) { return new QModule (type, new Invert()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_max ) { return new QModule (type, new Max()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_min ) { return new QModule (type, new Min()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_multiply ) { return new QModule (type, new Multiply()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_power ) { return new QModule (type, new Power()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_displace ) { return new QModule (type, new Displace()); }
    if (type == CalenhadServices::preferences() -> calenhad_module_diff) { return new QModule (type, new Diff()); }

    if (type == CalenhadServices::preferences() -> calenhad_module_cylinders) {
        QModule* qm = new QModule (type, new Cylinders());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_CYLINDERS_FREQUENCY, [=] (const double& value) { ((Cylinders*) qm -> module()) -> SetFrequency (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_spheres) {
        QModule* qm = new QModule (type, new Spheres());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_SPHERES_FREQUENCY, [=] (const double& value) { ((Spheres*) qm -> module()) -> SetFrequency (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_exponent) {
        QModule* qm = new QModule (type, new Exponent());
        qm -> addParameter ("Exponent", "exponent", noise::module::DEFAULT_EXPONENT, [=] (const double& value) { ((Exponent*) qm -> module()) -> SetExponent (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_translate) {
        QModule* qm = new QModule (type, new TranslatePoint());
        qm -> addParameter ("X", "x", noise::module::DEFAULT_TRANSLATE_POINT_X, [=] (const double& value) { ((TranslatePoint*) qm -> module()) -> SetXTranslation (value); });
        qm -> addParameter ("Y", "y", noise::module::DEFAULT_TRANSLATE_POINT_Y, [=] (const double& value) { ((TranslatePoint*) qm -> module()) -> SetYTranslation (value); });
        qm -> addParameter ("Z", "z", noise::module::DEFAULT_TRANSLATE_POINT_Z, [=] (const double& value) { ((TranslatePoint*) qm -> module()) -> SetZTranslation (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_rotate) {
        QModule* qm = new QModule (type, new RotatePoint());
        qm -> addParameter ("X", "x", noise::module::DEFAULT_ROTATE_X, [=] (const double& value) { ((RotatePoint*) qm -> module()) -> SetXAngle (value); });
        qm -> addParameter ("Y", "y", noise::module::DEFAULT_ROTATE_Y, [=] (const double& value) { ((RotatePoint*) qm -> module()) -> SetYAngle (value); });
        qm -> addParameter ("Z", "z", noise::module::DEFAULT_ROTATE_Z, [=] (const double& value) { ((RotatePoint*) qm -> module()) -> SetZAngle (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_clamp) {
        QModule* qm = new QModule (type, new Clamp());
        qm -> addParameter ("Lower bound", "lowerBound", noise::module::DEFAULT_CLAMP_LOWER_BOUND, [=] (const double& value) { Clamp* m = (Clamp*) qm -> module(); m -> SetBounds (value, m -> GetUpperBound()); });
        qm -> addParameter ("Upper bound", "upperBound", noise::module::DEFAULT_CLAMP_UPPER_BOUND, [=] (const double& value) { Clamp* m = (Clamp*) qm -> module(); m -> SetBounds (m -> GetLowerBound(), value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_constant) {
        QModule* qm = new QModule (type, new Const());
        qm -> addParameter ("Constant value", "constValue", noise::module::DEFAULT_CONST_VALUE, [=] (const double& value) { ((Const*) qm -> module()) -> SetConstValue (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_perlin) {
        QModule* qm = new QModule (type, new Perlin());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_PERLIN_FREQUENCY, [=] (const double& value) { ((Perlin*) qm -> module()) -> SetFrequency (value); });
        qm -> addParameter ("Lacunarity", "lacunarity", noise::module::DEFAULT_PERLIN_LACUNARITY, [=] (const double& value) { ((Perlin*) qm -> module()) -> SetLacunarity (value); });
        qm -> addParameter ("Persistence", "persistence", noise::module::DEFAULT_PERLIN_PERSISTENCE, [=] (const double& value) { ((Perlin*) qm -> module()) -> SetPersistence (value); });
        qm -> addParameter ("Octaves", "octaves", noise::module::DEFAULT_PERLIN_OCTAVE_COUNT, [=] (const double& value) { ((Perlin*) qm -> module()) -> SetOctaveCount ((int) (std::round (value))); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_billow) {
        QModule* qm = new QModule (type, new Billow());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_BILLOW_FREQUENCY, [=] (const double& value) { ((Billow*) qm -> module()) -> SetFrequency (value); });
        qm -> addParameter ("Lacunarity", "lacunarity", noise::module::DEFAULT_BILLOW_LACUNARITY, [=] (const double& value) { ((Billow*) qm -> module()) -> SetLacunarity (value); });
        qm -> addParameter ("Persistence", "persistence", noise::module::DEFAULT_BILLOW_PERSISTENCE, [=] (const double& value) { ((Billow*) qm -> module()) -> SetPersistence (value); });
        qm -> addParameter ("Octaves", "octaves", noise::module::DEFAULT_BILLOW_OCTAVE_COUNT, [=] (const double& value) { ((Billow*) qm -> module()) -> SetOctaveCount ((int) (std::round (value))); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_ridgedmulti) {
        QModule* qm = new QModule (type, new RidgedMulti());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_RIDGED_FREQUENCY, [=] (const double& value) { ((RidgedMulti*) qm -> module()) -> SetFrequency (value); });
        qm -> addParameter ("Lacunarity", "lacunarity", noise::module::DEFAULT_RIDGED_LACUNARITY, [=] (const double& value) { ((RidgedMulti*) qm -> module()) -> SetLacunarity (value); });
        qm -> addParameter ("Octaves", "octaves", noise::module::DEFAULT_RIDGED_OCTAVE_COUNT, [=] (const double& value) { ((RidgedMulti*) qm -> module()) -> SetOctaveCount ((int) (std::round (value))); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_scalebias) {
        QModule* qm = new QModule (type, new ScaleBias());
        qm -> addParameter ("Scale", "scale", noise::module::DEFAULT_SCALE, [=] (const double& value) { ((ScaleBias*) qm -> module()) -> SetScale (value); });
        qm -> addParameter ("Bias", "bias", noise::module::DEFAULT_BIAS, [=] (const double& value) { ((ScaleBias*) qm -> module()) -> SetBias (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_select) {
        QModule* qm = new QModule (type, new Select());
        qm -> addParameter ("Lower bound", "lowerBound", noise::module::DEFAULT_SELECT_LOWER_BOUND, [=] (const double& value) { Select* m = (Select*) qm -> module(); m -> SetBounds (value, m -> GetUpperBound()); });
        qm -> addParameter ("Upper bound", "upperBound", noise::module::DEFAULT_SELECT_UPPER_BOUND, [=] (const double& value) { Select* m = (Select*) qm -> module(); m -> SetBounds (m -> GetLowerBound(), value); });
        qm -> addParameter ("Falloff", "falloff", noise::module::DEFAULT_SELECT_EDGE_FALLOFF, [=] (const double& value) { ((Select*) qm -> module()) -> SetEdgeFalloff (value); });
        return qm;
    }
    if (type == CalenhadServices::preferences() -> calenhad_module_turbulence) {
        QModule* qm = new QModule (type, new Turbulence());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_TURBULENCE_FREQUENCY, [=] (const double& value) { ((Turbulence*) qm -> module()) -> SetFrequency (value); });
        qm -> addParameter ("Power", "power", noise::module::DEFAULT_TURBULENCE_POWER, [=] (const double& value) { ((Turbulence*) qm -> module()) -> SetPower (value); });
        qm -> addParameter ("Power", "power", noise::module::DEFAULT_TURBULENCE_ROUGHNESS, [=] (const double& value) { ((Turbulence*) qm -> module()) -> SetRoughness (value); });
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_voronoi) {
        QModule* qm = new QModule (type, new Voronoi());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_VORONOI_FREQUENCY, [=] (const double& value) { ((Voronoi*) qm -> module()) -> SetFrequency (value); });
        qm -> addParameter ("Displacement", "displacement", noise::module::DEFAULT_VORONOI_DISPLACEMENT, [=] (const double& value) { ((Voronoi*) qm -> module()) -> SetDisplacement (value); });
        qm -> addParameter ("Enable distance", "enableDistance", 1.0, [=] (const double& value) { ((Voronoi*) qm -> module()) -> EnableDistance (value > 0.0); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_scalepoint) {
        QModule* qm = new QModule (type, new ScalePoint());
        qm -> addParameter ("X", "x", noise::module::DEFAULT_SCALE_POINT_X, [=] (const double& value) { ((ScalePoint*) qm -> module()) -> SetXScale (value); });
        qm -> addParameter ("Y", "y", noise::module::DEFAULT_SCALE_POINT_Y, [=] (const double& value) { ((ScalePoint*) qm -> module()) -> SetYScale (value); });
        qm -> addParameter ("Z", "z", noise::module::DEFAULT_SCALE_POINT_Z, [=] (const double& value) { ((ScalePoint*) qm -> module()) -> SetZScale (value); });
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_icospheremap) { QIcosphereMap* qm = new QIcosphereMap(); return qm; }
    if (type == CalenhadServices::preferences() -> calenhad_module_altitudemap) { QAltitudeMap* qm = new QAltitudeMap(); return qm; }
    if (type == CalenhadServices::preferences() -> calenhad_nodegroup) { QNodeGroup* group = new QNodeGroup(); return group; }
    return nullptr;
}

QNode* ModuleFactory::clone (QNode* other) {
    QNode* n = createModule (other -> nodeType());
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
