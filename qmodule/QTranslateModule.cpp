//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QTranslateModule.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../CalenhadServices.h"

using namespace calenhad::qmodule;
using namespace noise::module;

QTranslateModule::QTranslateModule (QWidget* parent) : QModule (new TranslatePoint(), parent) {

}

QTranslateModule::~QTranslateModule() {

}

void QTranslateModule::initialise() {
    QModule::initialise();
    _name = "New Translation";
    dXSpin = addParameter ("Translate X", "dX");
    //connect (dXSpin, SIGNAL (valueChanged (double)), this, SLOT (setDX (double)));
    _contentLayout -> addRow (tr ("X"), dXSpin);
    dYSpin = addParameter ("Translate Y", "dY");
    //connect (dYSpin, SIGNAL (valueChanged (double)), this, SLOT (setDY (double)));
    _contentLayout -> addRow (tr ("Y"), dYSpin);
    dZSpin = addParameter ("Translate Z", "dZ");
    //connect (dZSpin, SIGNAL (valueChanged (double)), this, SLOT (setDZ (double)));
    _contentLayout -> addRow (tr ("Z"), dZSpin);
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
    emit (nodeChanged());
    dXSpin -> setValue (value);
}


void QTranslateModule::setDY (double value) {
    module() -> SetYTranslation (value);
    emit (nodeChanged());
    dYSpin -> setValue (value);
}

void QTranslateModule::setDZ (double value) {
    module() -> SetZTranslation (value);
    emit (nodeChanged());
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

QString QTranslateModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_translate;
}

QTranslateModule* QTranslateModule::clone () {
    QTranslateModule* qm = QTranslateModule::newInstance();
    if (qm) {
        qm -> setDX (dX());
        qm -> setDY (dY());
        qm -> setDZ (dZ());
    }
    return qm;
}

void QTranslateModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double x = _model -> readParameter (element, "x").toDouble (&ok);
    if (ok) { setDX (x); }

    double y = _model -> readParameter (element, "y").toDouble (&ok);
    if (ok) { setDY (y); }

    double z = _model -> readParameter (element, "z").toDouble (&ok);
    if (ok) { setDZ (z); }
}

void QTranslateModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "x", QString::number (dX()));
    _model -> writeParameter (_element, "y", QString::number (dY()));
    _model -> writeParameter (_element, "z", QString::number (dZ()));
}
