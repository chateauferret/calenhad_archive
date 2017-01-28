//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QCylindersModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

QCylindersModule::QCylindersModule (QWidget* parent) : QModule (new noise::module::Cylinders(), parent) {

}

QCylindersModule::~QCylindersModule () {

}

void QCylindersModule::initialise() {
    QModule::initialise();
    _name = "New Cylinders";
    frequencySpin = logParameterControl ("Constant value");
    connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QCylindersModule::frequency() {
    return module() -> GetFrequency ();
}

void QCylindersModule::setFrequency (double value) {
    module() -> SetFrequency (value);
    emit (nodeChanged ("frequency", value));
    frequencySpin -> setValue (value);
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

ModuleType QCylindersModule::type() {
    return ModuleType::CYLINDERS;
}

QCylindersModule* QCylindersModule::addCopy (CalenhadModel* model)  {
    QCylindersModule* qm = QCylindersModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setFrequency (frequency());
    }
    return qm;
}

QString QCylindersModule::typeString () {
    return "Cylinders";
}
