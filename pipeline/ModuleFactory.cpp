//
// Created by martin on 11/11/16.
//

#include <iostream>
#include "ModuleFactory.h"
#include "../libnoiseutils/diff.h"
//#include "../qmodule/QIcosphereMap.h"
#include "../qmodule/QNodeGroup.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../qmodule/QAltitudeMap.h"
#include "../CalenhadServices.h"
#include "../exprtk/ExpressionWidget.h"
#include <QPixmap>
#include <QList>
#include "../qmodule/RangeFinder.h"

using namespace noise::module;
using namespace calenhad;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::expressions;

ModuleFactory::ModuleFactory() : _codes (nullptr) {
    for (QString type : types()) {
        QString icon = type.toLower();
        icon.replace (" ", "");
        QPixmap* pixmap = new QPixmap (":/appicons/tools/" + icon + ".png");
        _icons.insert (type, pixmap);
        _codes = new QMap<QString, QString>();
        provideCodes();
        provideParamNames();
    }
}

ModuleFactory:: ~ModuleFactory() {
    for (QPixmap* icon : _icons) { delete icon; }
    if (_codes) { delete _codes; }
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
            << CalenhadServices::preferences() -> calenhad_module_clamp
            << CalenhadServices::preferences() -> calenhad_module_constant
            << CalenhadServices::preferences() -> calenhad_module_cylinders
            << CalenhadServices::preferences() -> calenhad_module_diff
            << CalenhadServices::preferences() -> calenhad_module_displace
            << CalenhadServices::preferences() -> calenhad_module_exponent
         // << CalenhadServices::preferences() -> calenhad_module_icospheremap
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

    if (type == CalenhadServices::preferences() -> calenhad_module_abs) { return new QModule (type, new AbsRangeFinder(), 1); }
    if (type == CalenhadServices::preferences() -> calenhad_module_invert) { return new QModule (type, new InvertRangeFinder(), 1); }
    if (type == CalenhadServices::preferences() -> calenhad_module_add) { return new QModule (type, new AddRangeFinder(), 1); }
    if (    type == CalenhadServices::preferences() -> calenhad_module_max) { return new QModule (type, new MaxRangeFinder(), 1); }
    if (    type == CalenhadServices::preferences() -> calenhad_module_min) { return new QModule (type, new MinRangeFinder(), 1); }
    if (    type == CalenhadServices::preferences() -> calenhad_module_multiply) { return new QModule (type, new MultiplyRangeFinder(), 1); }
    if (    type == CalenhadServices::preferences() -> calenhad_module_power) { return new QModule (type, new PowerRangeFinder(), 2); }
    if (    type == CalenhadServices::preferences() -> calenhad_module_diff) { return new QModule (type, new DiffRangeFinder(), 2); }

    if (type == CalenhadServices::preferences() -> calenhad_module_blend) { return new QModule (type, new OrRangeFinder(), 3); }
    if (type == CalenhadServices::preferences() -> calenhad_module_displace) { return new QModule (type, new PassThroughRangeFinder(), 4); }

    if (type == CalenhadServices::preferences() -> calenhad_module_cylinders) {
        QModule* qm = new QModule (type, new UnitRangeFinder());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_CYLINDERS_FREQUENCY, new AcceptPositive());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_spheres) {
        QModule* qm = new QModule (type, new UnitRangeFinder());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_SPHERES_FREQUENCY, new AcceptPositive());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_exponent) {
        QModule* qm = new QModule (type, new ExponentRangeFinder(), 1);
        qm -> addParameter ("Exponent", "exponent", noise::module::DEFAULT_EXPONENT, new AcceptAnyRubbish());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_translate) {
        QModule* qm = new QModule (type, new PassThroughRangeFinder(), 1);
        qm -> addParameter ("X", "x", noise::module::DEFAULT_TRANSLATE_POINT_X, new PreferNoiseValue());
        qm -> addParameter ("Y", "y", noise::module::DEFAULT_TRANSLATE_POINT_Y, new PreferNoiseValue());
        qm -> addParameter ("Z", "z", noise::module::DEFAULT_TRANSLATE_POINT_Z, new PreferNoiseValue());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_rotate) {
        QModule* qm = new QModule (type, new PassThroughRangeFinder(),  1);
        qm -> addParameter ("X", "x", noise::module::DEFAULT_ROTATE_X, new AcceptAngleDegrees());
        qm -> addParameter ("Y", "y", noise::module::DEFAULT_ROTATE_Y, new AcceptAngleDegrees());
        qm -> addParameter ("Z", "z", noise::module::DEFAULT_ROTATE_Z, new AcceptAngleDegrees());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_clamp) {
        QModule* qm = new QModule (type, new BoundsRangeFinder ("lowerBound", "upperBound"), 1);
        qm -> addParameter ("Lower bound", "lowerBound", noise::module::DEFAULT_CLAMP_LOWER_BOUND, new AcceptNoiseValue());
        qm -> addParameter ("Upper bound", "upperBound", noise::module::DEFAULT_CLAMP_UPPER_BOUND, new AcceptNoiseValue());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_constant) {
        QModule* qm = new QModule (type, new BoundsRangeFinder ("constValue", "constValue"));
        qm -> addParameter ("Constant value", "constValue", noise::module::DEFAULT_CONST_VALUE, new AcceptNoiseValue());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_perlin) {
        QModule* qm = new QModule (type, new UnitRangeFinder());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_PERLIN_FREQUENCY, new AcceptPositive());
        qm -> addParameter ("Lacunarity", "lacunarity", noise::module::DEFAULT_PERLIN_LACUNARITY, new AcceptRange (1.5, 3.5));
        qm -> addParameter ("Persistence", "persistence", noise::module::DEFAULT_PERLIN_PERSISTENCE, new AcceptRange (0, 1));
        qm -> addParameter ("Octaves", "octaves", noise::module::DEFAULT_PERLIN_OCTAVE_COUNT, new PreferInteger());
        qm -> addParameter ("Seed", "seed", noise::module::DEFAULT_PERLIN_SEED, new AcceptInteger());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_billow) {
        QModule* qm = new QModule (type, new UnitRangeFinder());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_BILLOW_FREQUENCY, new AcceptPositive());
        qm -> addParameter ("Lacunarity", "lacunarity", noise::module::DEFAULT_BILLOW_LACUNARITY, new AcceptRange (1.5, 3.5));
        qm -> addParameter ("Persistence", "persistence", noise::module::DEFAULT_BILLOW_PERSISTENCE, new AcceptRange (0, 1));
        qm -> addParameter ("Octaves", "octaves", noise::module::DEFAULT_BILLOW_OCTAVE_COUNT, new PreferInteger());
        qm -> addParameter ("Seed", "seed", noise::module::DEFAULT_PERLIN_SEED, new AcceptInteger());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_ridgedmulti) {
        QModule* qm = new QModule (type, new UnitRangeFinder());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_RIDGED_FREQUENCY, new AcceptPositive());
        qm -> addParameter ("Lacunarity", "lacunarity", noise::module::DEFAULT_RIDGED_LACUNARITY, new AcceptRange (1.5, 3.5));
        qm -> addParameter ("Octaves", "octaves", noise::module::DEFAULT_RIDGED_OCTAVE_COUNT, new PreferInteger());
        qm -> addParameter ("Seed", "seed", noise::module::DEFAULT_PERLIN_SEED, new AcceptInteger());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_scalebias) {
        QModule* qm = new QModule (type, new ScaleBiasRangeFinder(), 1);
        qm -> addParameter ("Scale", "scale", noise::module::DEFAULT_SCALE, new PreferNoiseValue());
        qm -> addParameter ("Bias", "bias", noise::module::DEFAULT_BIAS, new PreferNoiseValue());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_select) {
        QModule* qm = new QModule (type, new OrRangeFinder(), 3);
        qm -> addParameter ("Lower bound", "lowerBound", noise::module::DEFAULT_SELECT_LOWER_BOUND, new AcceptNoiseValue());
        qm -> addParameter ("Upper bound", "upperBound", noise::module::DEFAULT_SELECT_UPPER_BOUND, new AcceptNoiseValue());
        qm -> addParameter ("Falloff", "falloff", noise::module::DEFAULT_SELECT_EDGE_FALLOFF);
        return qm;
    }
    if (type == CalenhadServices::preferences() -> calenhad_module_turbulence) {
        QModule* qm = new QModule (type, new PassThroughRangeFinder(), 1);
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_TURBULENCE_FREQUENCY, new AcceptPositive());
        qm -> addParameter ("Power", "power", noise::module::DEFAULT_TURBULENCE_POWER, new AcceptAnyRubbish());
        qm -> addParameter ("Roughness", "roughness", noise::module::DEFAULT_TURBULENCE_ROUGHNESS, new AcceptAnyRubbish());
        qm -> addParameter ("Seed", "seed", noise::module::DEFAULT_PERLIN_SEED, new AcceptInteger());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_voronoi) {
        QModule* qm = new QModule (type, new UnitRangeFinder());
        qm -> addParameter ("Frequency", "frequency", noise::module::DEFAULT_VORONOI_FREQUENCY, new AcceptPositive());
        qm -> addParameter ("Displacement", "displacement", noise::module::DEFAULT_VORONOI_DISPLACEMENT, new PreferNoiseValue());
        qm -> addParameter ("Enable distance", "enableDistance", 1.0, new AcceptAnyRubbish);
        qm -> addParameter ("Seed", "seed", noise::module::DEFAULT_PERLIN_SEED, new AcceptInteger());
        return qm;
    }

    if (type == CalenhadServices::preferences() -> calenhad_module_scalepoint) {
        QModule* qm = new QModule (type, new PassThroughRangeFinder(), 1);
        qm -> addParameter ("X", "x", noise::module::DEFAULT_SCALE_POINT_X, new PreferNoiseValue());
        qm -> addParameter ("Y", "y", noise::module::DEFAULT_SCALE_POINT_Y, new PreferNoiseValue());
        qm -> addParameter ("Z", "z", noise::module::DEFAULT_SCALE_POINT_Z, new PreferNoiseValue());
        return qm;
    }

    //if (type == CalenhadServices::preferences() -> calenhad_module_icospheremap) { QIcosphereMap* qm = new QIcosphereMap(); return qm; }
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

QStringList ModuleFactory::paramNames() {
    return _paramNames;
}

void ModuleFactory::provideParamNames() {
    _paramNames << "frequency"
                << "lacunarity"
                << "persistence"
                << "octaves"
                << "seed"
                << "lowerBound"
                << "upperBound"
                << "enableDistance"
                << "constValue"
                << "falloff"
                << "x" << "y" << "z"
                << "scale" << "bias"
                << "displacement"
                << "enableDistance"
                << "exponent"
                << "roughness";
}

QMap<QString, QString>* ModuleFactory::codes() {
    return _codes;
};

// Create code snippets to be inserted into the compute shader to realise each module's function.
// %n - will be replaced with the module's name which will serve as the method name representing it in the generated shader code.
// %0, %1, etc - will be replaced with calls to the module outputs serving the corresponding port as input.
// %frequency, %lacunarity etc - will be replaced with the value of that parameter, for parameters named in calenhad::graph::Graph::_params.
void ModuleFactory::provideCodes() {
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_perlin, "float %n (vec3 v) { return perlin (v, %frequency, %lacunarity, %persistence, %octaves, %seed); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_billow, "float %n (vec3 v) { return billow (v,  %frequency, %lacunarity, %persistence, %octaves, %seed); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_ridgedmulti,
                     // "float %n (vec3 v) { return ridgedmulti (v, %frequency, %lacunarity, %octaves, %seed, %exponent, %offset, %gain, %sharpness); }\n");
                      "float %n (vec3 v) { return ridgedmulti (v, %frequency, %lacunarity, %octaves, %seed, 1.0, 1.0, 2.0, 2.0); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_voronoi, "float %n (vec3 v) { return voronoi (v, %frequency, %displacement, %enableDistance, %seed); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_add, "float %n (vec3 v) { return %0 (v) + %1 (v); }\n" );
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_multiply, "float %n (vec3 v) { return %0 (v) * %1 (v); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_diff, "float %n (vec3 v) { return %0 (v) - %1 (v); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_exponent, "float %n (vec3 v) { return pow (abs ((%0 (v) + 1.0) / 2.0), %exponent) * 2.0 - 1.0; }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_cylinders, "float %n (vec3 v) { return cylinders (v, %frequency); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_spheres, "float %n (vec3 v) { return spheres (v, %frequency); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_translate, "float %n (vec3 v) { return %0 (v + vec3 (%x, %y, %z)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_rotate, "float %n (vec3 v) { return %0 (rotate (v, vec3 (%x, %y, %z))); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_clamp, "float %n  (vec3 v) { return clamp (%0 (v), float (%lowerBound), float (%upperBound)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_constant, "float %n (vec3 v) { return %constValue; }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_abs, "float %n (vec3 v) { return abs (%0 (v)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_blend, "float %n (vec3 v) { return mix (%0 (v), %1 (v), %2 (v)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_invert, "float %n (vec3 v) { return - %0 (v); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_max, "float %n (vec3 v) { return max (%0 (v), %1 (v)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_min, "float %n (vec3 v) { return min (%0 (v), %1 (v)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_power, "float %n (vec3 v) { return pow (%0 (v), %1 (v)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_displace, "float %n (vec3 v) { return %0 (v + vec3 (%1 (v), %2 (v), %3 (v))); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_scalebias, "float %n (vec3 v) { return %0 (v) * %scale + %bias; }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_select, "float %n (vec3 v) { return select (%2 (v), %1 (v), %0 (v), %lowerBound, %upperBound, %falloff); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_turbulence, "float %n (vec3 v) { return %0 (turbulence (v, %frequency, %power, %roughness, %seed)); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_voronoi, "float %n (vec3 v) { return voronoi (v, %frequency, %displacement, %enableDistance, %seed); }\n");
    _codes -> insert (CalenhadServices::preferences() -> calenhad_module_scalepoint, "float %n (vec3 v) { return %0 (v * vec3 (%x, %y, %z)); }\n");
    // altitudemap generates code in its own methods - do not inject here


}
