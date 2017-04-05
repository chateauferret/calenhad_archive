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

QSpheresModule::QSpheresModule (QWidget* parent) : QModule (new noise::module::Spheres(), parent) {

}

QSpheresModule::~QSpheresModule() {

}

void QSpheresModule::initialise() {
    QModule::initialise();
    _name = "New Spheres";
    frequencySpin = logParameterControl ("Frequency");
    connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    _isInitialised = true;
    emit initialised();
}

double QSpheresModule::frequency() {
    return module() -> GetFrequency ();
}

void QSpheresModule::setFrequency (double value) {
    module() -> SetFrequency (value);
    emit (nodeChanged ("frequency", value));
    frequencySpin -> setValue (value);
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

QString QSpheresModule::moduleType () {
    return Calenhad::preferences -> calenhad_module_spheres;
}

QSpheresModule* QSpheresModule::addCopy (CalenhadModel* model) {
    QSpheresModule* qm = QSpheresModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setFrequency (frequency());
    }
    return qm;
}

void QSpheresModule::inflate (const QDomElement& element, MessageFactory* messages) {
    QModule::inflate (element, messages);
    bool ok;

    double frequency = _model -> readParameter (element, "frequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }
}

void QSpheresModule::serialise (QDomDocument& doc, MessageFactory* messages) {
    QModule::serialise (doc, messages);
    _model -> writeParameter (_element, "frequency", QString::number (frequency()));
}
