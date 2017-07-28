//
// Created by martin on 27/11/16.
//
#include <iostream>
#include "../preferences/PreferencesService.h"
#include "../CalenhadServices.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "QConstModule.h"
#include "../exprtk/ExpressionWidget.h"
#include "../nodeedit/QNodeBlock.h"

using namespace noise::module;
using namespace calenhad::qmodule;
using namespace calenhad::expressions;
using namespace calenhad::nodeedit;

QConstModule::QConstModule (QWidget* parent) : QModule (new Const(), parent) {

}

void QConstModule::initialise() {
    QModule::initialise();
    _name = "New Constant Value";
    _constValueWidget = addParameter ("Constant value");
    setConstValue ("0.01");
    _isInitialised = true;

    emit initialised();
}

void QConstModule::valueReady (const double& value) {
    QObject* s = sender();
    if (s == _constValueWidget) {
        module() -> SetConstValue (value);
        emit nodeChanged();
    }
}

void QConstModule::setConstValue (QString exp) {
    _constValueWidget -> setText (exp);
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

QString QConstModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_constant;
}

QConstModule* QConstModule::clone () {
    QConstModule* qm = QConstModule::newInstance();
    if (qm) {
        qm -> setConstValue (QString::number (constValue()));
    }
    return qm;
}

void QConstModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;
    QString constValue = _model -> readParameter (element, "constValue");
    setConstValue (constValue);

}

void QConstModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "constValue", _constValueWidget -> text());
}
