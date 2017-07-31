//
// Created by martin on 26/11/16.
//

#include <libnoise/module/spheres.h>
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "QSpheresModule.h"


using namespace noise::module;
using namespace calenhad::qmodule;

QSpheresModule::QSpheresModule (QWidget* parent) : QModule (new noise::module::Spheres(), parent) {

}

QSpheresModule::~QSpheresModule() {

}

void QSpheresModule::initialise() {
    QModule::initialise();
    _name = "New Spheres";
    frequencySpin = addParameter ("Frequency", "frequency");
    //connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);

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

void QSpheresModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "getFrequency", QString::number (frequency()));
}
