//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QDomElement>
#include "preferences/preferences.h"
#include "QNoiseModule.h"
#include "../CalenhadServices.h"
#include "../pipeline/CalenhadModel.h"

using namespace calenhad::qmodule;
using namespace noise::module;

QNoiseModule::QNoiseModule (noise::module::Module* m, QWidget* parent) : QModule (m, parent) {

}

QNoiseModule::~QNoiseModule () {

}

void QNoiseModule::initialise() {
    QModule::initialise();

    frequencySpin = addParameter ("Frequency", "frequency");
    frequencySpin -> setValue (getFrequency());
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);

    lacunaritySpin = addParameter ("Lacunarity", "lacunarity");
    lacunaritySpin -> setValue (getLacunarity());
    _contentLayout -> addRow (tr ("Lacunarity"), lacunaritySpin);


    if (hasPersistence()) {
        persistenceSpin = addParameter ("Persistence", "persistence");
        persistenceSpin -> setValue (getPersistence());
        _contentLayout -> addRow (tr ("Persistence"), persistenceSpin);

    }

    octaveSpin = countParameterControl ("Number of octaves");
    octaveSpin -> setValue (getOctaveCount());

    _contentLayout -> addRow (tr ("Octaves"), octaveSpin);

    setFrequency (getFrequency ());
    setLacunarity (getLacunarity ());
    setOctaveCount (getOctaveCount ());
    setPersistence (getPersistence ());
}

double QNoiseModule::getFrequency () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetFrequency ();
    }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) {
        return billow -> GetFrequency();
    }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) {
        return rm -> GetFrequency();
    }
    return 0.0;
}

void QNoiseModule::setFrequency (const double& value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) { perlin -> SetFrequency (value); }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) { billow -> SetFrequency (value); }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) { rm -> SetFrequency (value); }
    if (perlin || billow || rm) {
        emit (nodeChanged());
        frequencySpin -> setValue (value);
    }
}

double QNoiseModule::getLacunarity() {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetLacunarity();
    }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) {
        return billow -> GetLacunarity();
    }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) {
        return rm -> GetLacunarity();
    }
    return 0.0;
}

void QNoiseModule::setLacunarity (const double& value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) { perlin -> SetLacunarity (value); }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) { billow -> SetLacunarity (value); }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) { rm -> SetLacunarity (value); }
    if (perlin || billow || rm) {
        emit (nodeChanged());
        lacunaritySpin -> setValue (value);
    }
}

double QNoiseModule::getPersistence () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetPersistence();
    }
    return 0.0;
}

void QNoiseModule::setPersistence (const double& value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        perlin -> SetPersistence (value);
        //persistenceSpin -> setValue (value);
        emit (nodeChanged());
        persistenceSpin -> setValue (value);
        return;
    }
}

int QNoiseModule::getOctaveCount () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetOctaveCount();
    }
    return 0;
}

void QNoiseModule::setOctaveCount (const int& value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        perlin -> SetOctaveCount (value);
        emit (nodeChanged());
        octaveSpin -> setValue (value);
        return;
    }
}

bool QNoiseModule::hasPersistence () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    return perlin != nullptr;
}

QNoiseModule* QNoiseModule::newRidgedMultiInstance() {
    RidgedMulti* rm = new RidgedMulti();
    rm -> SetSeed (seed);
    rm -> SetNoiseQuality (noiseQuality);
    Module* m = rm;
    return newInstance (m);
}

QNoiseModule* QNoiseModule::newBillowInstance() {
    Billow* b = new Billow();
    b -> SetSeed (seed);
    b -> SetNoiseQuality (noiseQuality);
    Module* m = b;
    return newInstance (m);
}

QNoiseModule* QNoiseModule::newPerlinInstance() {
    Perlin* p = new Perlin ();
    p -> SetSeed (seed);
    p -> SetNoiseQuality (noiseQuality);
    Module* m = p;
    return newInstance (m);
}

QNoiseModule* QNoiseModule::newInstance (Module* m) {
    QNoiseModule* qm = new QNoiseModule (m);
    qm -> initialise();
    return qm;
}

QString QNoiseModule::nodeType () {
    if (dynamic_cast<Perlin*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_perlin; }
    if (dynamic_cast<Billow*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_billow; }
    if (dynamic_cast<RidgedMulti*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_ridgedmulti; }
    return QString::null;
}

QNoiseModule* QNoiseModule::clone() {
    QNoiseModule* qm = nullptr;
    if (nodeType () == CalenhadServices::preferences() -> calenhad_module_perlin) { qm = newPerlinInstance(); }
    if (nodeType () == CalenhadServices::preferences() -> calenhad_module_billow) { qm = newBillowInstance(); }
    if (nodeType () == CalenhadServices::preferences() -> calenhad_module_ridgedmulti) { qm = newRidgedMultiInstance(); }
    if (qm) {
        qm -> setFrequency (getFrequency ());
        qm -> setLacunarity (getLacunarity ());
        qm -> setOctaveCount (getOctaveCount ());
        qm -> setPersistence (getPersistence ());
    }
    return qm;
}

void QNoiseModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double frequency = _model -> readParameter (element, "frequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }

    double lacunarity = _model -> readParameter (element, "lacunarity").toDouble (&ok);
    if (ok) { setLacunarity (lacunarity); }

    int octaves = _model -> readParameter (element, "octaves").toInt (&ok);
    if (ok) { setOctaveCount (octaves); }

    if (hasPersistence()) {
        double persistence = _model -> readParameter (element, "persistence").toDouble (&ok);
        if (ok) { setPersistence (persistence); }
    }
}

void QNoiseModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "frequency", QString::number (getFrequency ()));
    _model -> writeParameter (_element, "lacunarity", QString::number (getLacunarity ()));
    _model -> writeParameter (_element, "octaves", QString::number (getOctaveCount ()));
    if (hasPersistence ()) {
        _model -> writeParameter (_element, "persistence", QString::number (getPersistence ()));
    }
}