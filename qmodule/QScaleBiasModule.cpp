//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QScaleBiasModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

QScaleBiasModule::QScaleBiasModule (QWidget* parent) : QModule (new noise::module::ScaleBias(), parent) {

}

QScaleBiasModule::~QScaleBiasModule() {

}

void QScaleBiasModule::initialise() {
    QModule::initialise();
    _name = "New Scale and Bias";
    scaleSpin = logParameterControl ("Scale");
    connect (scaleSpin, SIGNAL (valueChanged (double)), this, SLOT (setScale (double)));
    _contentLayout -> addRow (tr ("Scale"), scaleSpin);
    biasSpin = logParameterControl ("Bias");
    connect (biasSpin, SIGNAL (valueChanged (double)), this, SLOT (setBias (double)));
    _contentLayout -> addRow (tr ("Bias"), biasSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QScaleBiasModule::scale() {
    return module() -> GetScale();
}

double QScaleBiasModule::bias() {
    return module() -> GetBias();
}

void QScaleBiasModule::setScale (double value) {
    module() -> SetScale (value);
    emit (nodeChanged ("scale", value));
    scaleSpin -> setValue (value);
}

void QScaleBiasModule::setBias (double value) {
    module() -> SetBias (value);
    emit (nodeChanged ("bias", value));
    biasSpin -> setValue (value);
}

ScaleBias* QScaleBiasModule::module () {
    ScaleBias* t = dynamic_cast<ScaleBias*> (_module);
    return t;
}

QScaleBiasModule* QScaleBiasModule::newInstance() {
    QScaleBiasModule* sm = new QScaleBiasModule();
    sm -> initialise();
    return sm;
}

ModuleType QScaleBiasModule::type() {
    return ModuleType::SCALEBIAS;
}


QScaleBiasModule* QScaleBiasModule::addCopy (CalenhadModel* model) {
    QScaleBiasModule* qm = QScaleBiasModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setScale (scale());
        qm -> setBias (bias());
    }
    return qm;
}

QString QScaleBiasModule::typeString() {
    return "ScaleBias";
}
