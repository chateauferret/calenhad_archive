//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QScaleModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

QScaleModule::QScaleModule (QWidget* parent) : QModule (new noise::module::ScalePoint(), parent) {

}

QScaleModule::~QScaleModule() {

}

void QScaleModule::initialise() {
    QModule::initialise();
    _name = "New Scale";
    scaleXSpin = logParameterControl ("Scale X");
    connect (scaleXSpin, SIGNAL (valueChanged (double)), this, SLOT (setScaleX (double)));
    _contentLayout -> addRow ("Scale X", scaleXSpin);
    scaleYSpin = logParameterControl ("Scale Y");
    connect (scaleYSpin, SIGNAL (valueChanged (double)), this, SLOT (setScaleY (double)));
    _contentLayout -> addRow ("Scale Y", scaleYSpin);
    scaleZSpin = logParameterControl ("Scale Z");
    connect (scaleZSpin, SIGNAL (valueChanged (double)), this, SLOT (setScaleZ (double)));
    _contentLayout -> addRow ("Scale Z", scaleZSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QScaleModule::scaleX() {
    return module() -> GetXScale();
}

double QScaleModule::scaleY() {
    return module() -> GetYScale();
}

double QScaleModule::scaleZ() {
    return module() -> GetZScale();
}


void QScaleModule::setScaleX (double value) {
    module() -> SetXScale (value);
    emit (nodeChanged ("dX", value));
    scaleXSpin -> setValue (value);
}


void QScaleModule::setScaleY (double value) {
    module() -> SetYScale (value);
    emit (nodeChanged ("dY", value));
    scaleYSpin -> setValue (value);
}

void QScaleModule::setScaleZ (double value) {
    module() -> SetZScale (value);
    emit (nodeChanged ("xAngle", value));
    scaleZSpin -> setValue (value);
}

ScalePoint* QScaleModule::module () {
    ScalePoint* t = dynamic_cast<ScalePoint*> (_module);
    return t;
}

ModuleType QScaleModule::type() {
    return ModuleType::SCALEPOINT;
}

QScaleModule* QScaleModule::newInstance() {
    QScaleModule* sm = new QScaleModule();
    sm -> initialise();
    return sm;
}

QScaleModule* QScaleModule::addCopy (CalenhadModel* model) {
    QScaleModule* qm = QScaleModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setScaleX (scaleX());
        qm -> setScaleY (scaleY());
        qm -> setScaleZ (scaleZ());
    }
    return qm;
}

QString QScaleModule::typeString () {
    return "Scale";
}
