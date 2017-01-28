//
// Created by martin on 27/11/16.
//

#include "QConstModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

using namespace noise::module;
QConstModule::QConstModule (QWidget* parent) : QModule (new Const(), parent) {

}

void QConstModule::initialise() {
    QModule::initialise();
    _name = "New Constant Value";
    constValueSpin = noiseValueParamControl ("Constant value");
    connect (constValueSpin, SIGNAL (valueChanged (double)), this, SLOT (setConstValue (double)));
    _contentLayout -> addRow (tr ("Constant value"), constValueSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

void QConstModule::setConstValue (double value) {
    module() -> SetConstValue (value);
    constValueSpin -> setValue (value);
    emit nodeChanged ("constValue", value);
}

double QConstModule::constValue () {
    return module() -> GetConstValue();
}

Const* QConstModule::module() {
    return dynamic_cast<Const*> (_module);
}

QConstModule* QConstModule::newInstance() {
    QConstModule* qm = new QConstModule();
    qm -> initialise();
    return qm;
}

ModuleType QConstModule::type() {
    return ModuleType::CONSTANT;
}

QConstModule* QConstModule::addCopy (CalenhadModel* model)  {
    QConstModule* qm = QConstModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setConstValue (constValue ());
    }
    return qm;
}

QString QConstModule::typeString () {
    return "Constant";
}
