//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QSpheresModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"
#include "../CalenhadServices.h"

QSpheresModule::QSpheresModule (QWidget* parent) : QModule (new noise::module::Spheres(), parent) {

}

QSpheresModule::~QSpheresModule() {

}

void QSpheresModule::initialise() {
    QModule::initialise();
    _name = "New Spheres";
    frequencySpin = logParameterControl ("Frequency");
    //connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    _isInitialised = true;
    emit initialised();
}

double QSpheresModule::frequency() {
    return module() -> GetFrequency ();
}

void QSpheresModule::setFrequency (double value) {
    if (value != module ()->GetFrequency ()) {
        module ()->SetFrequency (value);
        emit (nodeChanged ());
        frequencySpin->setValue (value);
    }
}

Spheres* QSpheresModule::module () {
    Spheres* spheres = dynamic_cast<Spheres*> (_module);
    return spheres;
}

QSpheresModule* QSpheresModule::newInstance () {
    QSpheresModule* qm = new QSpheresModule();
    qm -> initialise();
    return qm;
}

QString QSpheresModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_spheres;
}

QSpheresModule* QSpheresModule::clone () {
    QSpheresModule* qm = QSpheresModule::newInstance();
    if (qm) {
        qm -> setFrequency (frequency());
    }
    return qm;
}

void QSpheresModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double frequency = _model -> readParameter (element, "getFrequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }
}

void QSpheresModule::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    _model -> writeParameter (_element, "getFrequency", QString::number (frequency()));
}
