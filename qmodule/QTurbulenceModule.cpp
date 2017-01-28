//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QTurbulenceModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"


QTurbulenceModule::QTurbulenceModule (QWidget* parent)  : QModule (new Turbulence(), parent) {

}

QTurbulenceModule::~QTurbulenceModule() {

}

void QTurbulenceModule::initialise() {
    QModule::initialise();
    _name = "New Turbulence";
    module() -> SetSeed (seed);
    frequencySpin = logParameterControl ("Frequency");
    connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    powerSpin = logParameterControl ("Power");
    connect (powerSpin, SIGNAL (valueChanged (double)), this, SLOT (setPower (double)));
    _contentLayout -> addRow (tr ("Power"), powerSpin);
    roughnessSpin = logParameterControl ("Roughness");
    connect (roughnessSpin, SIGNAL (valueChanged (double)), this, SLOT (setRoughness (double)));
    _contentLayout -> addRow (tr ("Roughness"), roughnessSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QTurbulenceModule::frequency() {
    return module() -> GetFrequency();
}

double QTurbulenceModule::power() {
    return module() -> GetPower();
}

double QTurbulenceModule::roughness() {
    return module() -> GetRoughnessCount();
}


void QTurbulenceModule::setFrequency (double value) {
    module() -> SetFrequency (value);
    emit (nodeChanged ("frequency", value));
    frequencySpin -> setValue (value);
}

void QTurbulenceModule::setPower (double value) {
    module() -> SetPower (value);
    emit (nodeChanged ("power", value));
    powerSpin -> setValue (value);
}

void QTurbulenceModule::setRoughness (double value) {
    module() -> SetRoughness (value);
    emit (nodeChanged ("roughness", value));
    roughnessSpin -> setValue (value);
}

Turbulence* QTurbulenceModule::module () {
    Turbulence* t = dynamic_cast<Turbulence*> (_module);
    return t;
}

QTurbulenceModule* QTurbulenceModule::newInstance () {
    QTurbulenceModule* qm = new QTurbulenceModule();
    qm -> initialise();
    return qm;
}

ModuleType QTurbulenceModule::type() {
    return ModuleType::TURBULENCE;
}

QTurbulenceModule* QTurbulenceModule::addCopy (CalenhadModel* model) {
    QTurbulenceModule* qm = QTurbulenceModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setFrequency (frequency());
        qm -> setPower (power());
        qm -> setRoughness (roughness());
    }
    return qm;
}

QString QTurbulenceModule::typeString () {
    return "Turbulence";
}
