//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QExponentModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

QExponentModule::QExponentModule (QWidget* parent) : QModule (new noise::module::Exponent(), parent) {

}

QExponentModule::~QExponentModule() {

}

void QExponentModule::initialise() {
    QModule::initialise();
    _name = "New Exponent";
    exponentSpin = logParameterControl ("Exponent");
    connect (exponentSpin, SIGNAL (valueChanged (double)), this, SLOT (setExponent (double)));
    _contentLayout -> addRow (tr ("Exponent"), exponentSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QExponentModule::exponent() {
    return module() -> GetExponent();
}

void QExponentModule::setExponent (double value) {
    module() -> SetExponent (value);
    emit (nodeChanged ("exponent", value));
    exponentSpin -> setValue (value);
}

Exponent* QExponentModule::module () {
    Exponent* e = dynamic_cast<Exponent*> (_module);
    return e;
}

QExponentModule* QExponentModule::newInstance() {
    QExponentModule* qm = new QExponentModule();
    qm -> initialise();
    return qm;
}

ModuleType QExponentModule::type() {
    return ModuleType::EXPONENT;
}

QExponentModule* QExponentModule::addCopy (CalenhadModel* model) {
    QExponentModule* qm = QExponentModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setExponent (exponent());
    }
    return qm;
}

QString QExponentModule::typeString () {
    return "Exponent";
}
