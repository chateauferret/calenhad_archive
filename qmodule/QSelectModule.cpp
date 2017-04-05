//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QSelectModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"

QSelectModule::QSelectModule (QWidget* parent) : QRangeModule (new noise::module::Select(), parent) {

}

QSelectModule::~QSelectModule() {

}

void QSelectModule::initialise() {
    QRangeModule::initialise();
    _name = "New Select";
    falloffSpin = noiseValueParamControl ("Falloff");
    connect (falloffSpin, SIGNAL (valueChanged (double)), this, SLOT (setFalloff (double)));
    _contentLayout -> addRow (tr ("Falloff"), falloffSpin);
    _isInitialised = true;
    emit initialised();
}

double QSelectModule::falloff() {
    return module() -> GetEdgeFalloff();
}

double QSelectModule::lowerBound() {
    return module() -> GetLowerBound();
}

double QSelectModule::upperBound() {
    return module() -> GetUpperBound();
}

void QSelectModule::setBounds (double lowerBound, double upperBound) {
    module() -> SetBounds (lowerBound, upperBound);
}

void QSelectModule::setFalloff (double value) {
    module() -> SetEdgeFalloff (value);
    emit (nodeChanged ("falloff", value));
    falloffSpin -> setValue (value);
}

Select* QSelectModule::module () {
    Select* r = dynamic_cast<Select*> (_module);
    return r;
}

QSelectModule* QSelectModule::newInstance () {
    QSelectModule* qm = new QSelectModule();
    qm -> initialise();
    return qm;
}

QString QSelectModule::moduleType() {
    return Calenhad::preferences -> calenhad_module_select;
}

QSelectModule* QSelectModule::addCopy (CalenhadModel* model) {
    QSelectModule* qm = QSelectModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setFalloff (falloff());
        qm -> setLowerBound (lowerBound());
        qm -> setUpperBound (upperBound());
    }
    return qm;
}



void QSelectModule::inflate (const QDomElement& element, MessageFactory* messages) {
    QRangeModule::inflate (element, messages);
}

void QSelectModule::serialise (QDomDocument& doc, MessageFactory* messages) {
    QRangeModule::serialise (doc, messages);
}