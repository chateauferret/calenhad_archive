//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QTurbulenceModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"


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
    emit initialised();
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
    emit (nodeChanged ("getFrequency", value));
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

QString QTurbulenceModule::moduleType () {
    return CalenhadServices::preferences() -> calenhad_module_turbulence;
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

void QTurbulenceModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double frequency = _model -> readParameter (element, "getFrequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }

    double power = _model -> readParameter (element, "power").toDouble (&ok);
    if (ok) { setPower (power); }

    double roughness = _model -> readParameter (element, "roughness").toDouble (&ok);
    if (ok) { setRoughness (roughness); }
}

void QTurbulenceModule::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    _model -> writeParameter (_element, "getFrequency", QString::number (frequency()));
    _model -> writeParameter (_element, "power", QString::number (power()));
    _model -> writeParameter (_element, "roughness", QString::number (roughness()));
}
