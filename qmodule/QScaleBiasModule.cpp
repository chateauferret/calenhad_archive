//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QScaleBiasModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"

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
    emit initialised();
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

QString QScaleBiasModule::moduleType () {
    return Calenhad::preferences -> calenhad_module_scalebias;
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


void QScaleBiasModule::inflate (const QDomElement& element, MessageFactory* messages) {
    QModule::inflate (element, messages);
    bool ok;

    double scale = _model -> readParameter (element, "scale").toDouble (&ok);
    if (ok) { setScale (scale); }

    double bias = _model -> readParameter (element, "bias").toDouble (&ok);
    if (ok) { setBias (bias); }

}

void QScaleBiasModule::serialise (QDomDocument& doc, MessageFactory* messages) {
    QModule::serialise (doc, messages);
    _model -> writeParameter (_element, "scale", QString::number (scale()));
    _model -> writeParameter (_element, "bias", QString::number (bias()));
}

