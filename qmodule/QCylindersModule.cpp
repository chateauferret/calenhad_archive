//
// Created by martin on 26/11/16.
//
#include "QCylindersModule.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../CalenhadServices.h"

using namespace calenhad::qmodule;
using namespace calenhad::controls;
using namespace noise::module;

QCylindersModule::QCylindersModule (QWidget* parent) : QModule (new noise::module::Cylinders(), parent) {

}

QCylindersModule::~QCylindersModule () {

}

void QCylindersModule::initialise() {
    QModule::initialise();
    _name = "New Cylinders";
    frequencySpin = parameterControl ("Frequency", "frequency");
    //connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    frequencySpin -> setValue (1.0);
    _isInitialised = true;
    emit initialised();
}

double QCylindersModule::frequency() {
    return module() -> GetFrequency ();
}

void QCylindersModule::setFrequency (double value) {
    if (value != module() -> GetFrequency()) {
        module ()->SetFrequency (value);
        emit (nodeChanged());
        frequencySpin->setValue (value);
    }
}


Cylinders* QCylindersModule::module () {
    Cylinders* cylinders = dynamic_cast<Cylinders*> (_module);
    return cylinders;
}

QCylindersModule* QCylindersModule::newInstance () {
    QCylindersModule* qm = new QCylindersModule();
    qm -> initialise();
    return qm;
}

QString QCylindersModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_cylinders;
}

QCylindersModule* QCylindersModule::clone () {
    QCylindersModule* qm = QCylindersModule::newInstance();
    if (qm) {
        qm -> setFrequency (frequency());
    }
    return qm;
}


void QCylindersModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double frequency = _model -> readParameter (element, "getFrequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }
}

void QCylindersModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "getFrequency", QString::number (frequency()));
}
