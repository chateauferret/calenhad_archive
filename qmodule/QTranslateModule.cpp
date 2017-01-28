//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QTranslateModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"

QTranslateModule::QTranslateModule (QWidget* parent) : QModule (new TranslatePoint(), parent) {

}

QTranslateModule::~QTranslateModule() {

}

void QTranslateModule::initialise() {
    QModule::initialise();
    _name = "New Translation";
    dXSpin = logParameterControl ("Translate X");
    connect (dXSpin, SIGNAL (valueChanged (double)), this, SLOT (setDX (double)));
    _contentLayout -> addRow (tr ("X"), dXSpin);
    dYSpin = logParameterControl ("Translate Y");
    connect (dYSpin, SIGNAL (valueChanged (double)), this, SLOT (setDY (double)));
    _contentLayout -> addRow (tr ("Y"), dYSpin);
    dZSpin = logParameterControl ("Translate Z");
    connect (dZSpin, SIGNAL (valueChanged (double)), this, SLOT (setDZ (double)));
    _contentLayout -> addRow (tr ("Z"), dZSpin);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QTranslateModule::dX() {
    return module() -> GetXTranslation();
}

double QTranslateModule::dY() {
    return module() -> GetYTranslation();
}

double QTranslateModule::dZ() {
    return module() -> GetZTranslation();
}


void QTranslateModule::setDX (double value) {
    module() -> SetXTranslation (value);
    emit (nodeChanged ("dX", value));
    dXSpin -> setValue (value);
}


void QTranslateModule::setDY (double value) {
    module() -> SetYTranslation (value);
    emit (nodeChanged ("dY", value));
    dYSpin -> setValue (value);
}

void QTranslateModule::setDZ (double value) {
    module() -> SetZTranslation (value);
    emit (nodeChanged ("xAngle", value));
    dZSpin -> setValue (value);
}

TranslatePoint* QTranslateModule::module () {
    TranslatePoint* t = dynamic_cast<TranslatePoint*> (_module);
    return t;
}

QTranslateModule* QTranslateModule::newInstance () {
    QTranslateModule* qm = new QTranslateModule();
    qm -> initialise();
    return qm;
}

ModuleType QTranslateModule::type() {
    return ModuleType::TRANSLATE;
}

QTranslateModule* QTranslateModule::addCopy (CalenhadModel* model) {
    QTranslateModule* qm = QTranslateModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setDX (dX());
        qm -> setDY (dY());
        qm -> setDZ (dZ());
    }
    return qm;
}

QString QTranslateModule::typeString () {
    return "Translate";
}
