//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QNoiseModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

QNoiseModule::QNoiseModule (noise::module::Module* m, QWidget* parent) : QModule (m, parent) {

}

QNoiseModule::~QNoiseModule () {

}

void QNoiseModule::initialise() {
    QModule::initialise();

    frequencySpin = logParameterControl ("Frequency");
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    lacunaritySpin = logParameterControl ("Lacunarity");
    _contentLayout -> addRow (tr ("Lacunarity"), lacunaritySpin);
    connect (lacunaritySpin, SIGNAL (valueChanged (double)), this, SLOT (setLacunarity (double)));
    if (hasPersistence()) {
        persistenceSpin = logParameterControl ("Persistence");
        _contentLayout -> addRow (tr ("Persistence"), persistenceSpin);
        connect (persistenceSpin, SIGNAL (valueChanged (double)), this, SLOT (setPersistence (double)));
    }

    octaveSpin = countParameterControl ("Number of octaves");
    connect (octaveSpin, SIGNAL (valueChanged (int)), this, SLOT (setOctaveCount (int)));
    _contentLayout -> addRow (tr ("Octaves"), octaveSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QNoiseModule::frequency() {
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

void QNoiseModule::setFrequency (double value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) { perlin -> SetFrequency (value); }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) { billow -> SetFrequency (value); }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) { rm -> SetFrequency (value); }
    if (perlin || billow || rm) {
        emit (nodeChanged ("frequency", value));
        frequencySpin -> setValue (value);
        return;
    }
}

double QNoiseModule::lacunarity() {
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

void QNoiseModule::setLacunarity (double value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) { perlin -> SetLacunarity (value); }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) { billow -> SetLacunarity (value); }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) { rm -> SetLacunarity (value); }
    if (perlin || billow || rm) {
        emit (nodeChanged ("lacunarity", value));
        lacunaritySpin -> setValue (value);
        return;
    }
}

double QNoiseModule::persistence() {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetPersistence();
    }
    return 0.0;
}

void QNoiseModule::setPersistence (double value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        perlin -> SetPersistence (value);
        persistenceSpin -> setValue (value);
        emit (nodeChanged ("persistence", value));
        return;
    }
}

int QNoiseModule::octaveCount () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetOctaveCount();
    }
    return 0;
}

void QNoiseModule::setOctaveCount (int value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        perlin -> SetOctaveCount (value);
        octaveSpin -> setValue (value);
        emit (nodeChanged ("octaveCount", value));
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

ModuleType QNoiseModule::type() {
    if (dynamic_cast<Perlin*> (_module)) { return ModuleType::PERLIN; }
    if (dynamic_cast<Billow*> (_module)) { return ModuleType::BILLOW; }
    if (dynamic_cast<RidgedMulti*> (_module)) { return ModuleType::RIDGEDMULTI; }
    return ModuleType::NONE;
}

QNoiseModule* QNoiseModule::addCopy (CalenhadModel* model)  {
    QNoiseModule* qm = nullptr;
    if (type() == ModuleType::PERLIN) { qm = newPerlinInstance(); }
    if (type() == ModuleType::BILLOW) { qm = newBillowInstance(); }
    if (type() == ModuleType::RIDGEDMULTI) { qm = newRidgedMultiInstance(); }
    if (qm) {
        qm -> setModel (model);
        qm -> setFrequency (frequency ());
        qm -> setLacunarity (lacunarity ());
        qm -> setOctaveCount (octaveCount ());
        qm -> setPersistence (persistence ());
    }
    return qm;
}

QString QNoiseModule::typeString () {
    if (dynamic_cast<Perlin*> (_module)) { return "Perlin"; }
    if (dynamic_cast<Billow*> (_module)) { return "Billow"; }
    if (dynamic_cast<RidgedMulti*> (_module)) { return "RidgedMulti"; }
    return "";
}
