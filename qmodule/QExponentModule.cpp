//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QExponentModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"

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
    emit initialised();
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

QString QExponentModule::moduleType () {
    return CalenhadServices::preferences() -> calenhad_module_exponent;
}

QExponentModule* QExponentModule::addCopy (CalenhadModel* model) {
    QExponentModule* qm = QExponentModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setExponent (exponent());
    }
    return qm;
}


void QExponentModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double exp = _model -> readParameter (element, "exponent").toDouble (&ok);
    if (ok) { setExponent (exp); }
}

void QExponentModule::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    _model -> writeParameter (_element, "exponent", QString::number (exponent()));
}
