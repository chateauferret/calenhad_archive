//
// Created by martin on 26/11/16.
//

#include "QRotateModule.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "qwt/qwt_counter.h"
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::qmodule;

QRotateModule::QRotateModule (noise::module::RotatePoint* m, QWidget* parent) : QModule (m, parent) {

}

QRotateModule::~QRotateModule() {

}

void QRotateModule::initialise() {
    QModule::initialise();
    _name = "New Rotation";
    QWidget* w = new QWidget();
    _contentLayout -> setMargin (0);
    _contentLayout -> addWidget (w);

    xAngleSpin = angleParameterControl ("Rotate X", "xAngle");
    _contentLayout -> addRow ("X axis", xAngleSpin);
    yAngleSpin = angleParameterControl ("Rotate Y", "yAngle");
    _contentLayout -> addRow ("Y axis", yAngleSpin);
    zAngleSpin = angleParameterControl ("Rotate Z", "zAngle");
    _contentLayout -> addRow ("Z axis", zAngleSpin);

}

double QRotateModule::xAngle() {
    return module() -> GetXAngle();
}

double QRotateModule::yAngle() {
    return module() -> GetYAngle();
}

double QRotateModule::zAngle() {
    return module() -> GetZAngle();
}


void QRotateModule::setXAngle (double value) {
    module() -> SetXAngle (value);
    emit (nodeChanged());
    xAngleSpin -> setValue (value);
}

void QRotateModule::setYAngle (double value) {
    module() -> SetYAngle (value);
    emit (nodeChanged());
    yAngleSpin -> setValue (value);
}

void QRotateModule::setZAngle (double value) {
    module() -> SetZAngle (value);
    emit (nodeChanged());
    zAngleSpin -> setValue (value);
    return;
}

RotatePoint* QRotateModule::module () {
    RotatePoint* r = dynamic_cast<RotatePoint*> (_module);
    return r;
}

QString QRotateModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_rotate;
}

QRotateModule* QRotateModule::newInstance () {
    RotatePoint* m = new RotatePoint();
    QRotateModule* qm = new QRotateModule (m);
    qm -> initialise();
    return qm;
}

QRotateModule* QRotateModule::clone () {
    QRotateModule* qm = QRotateModule::newInstance();
    if (qm) {
        qm -> setXAngle (xAngle());
        qm -> setYAngle (yAngle());
        qm -> setZAngle (zAngle());
    }
    return qm;
}


void QRotateModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double x = _model -> readParameter (element, "x").toDouble (&ok);
    if (ok) { setXAngle (x); }

    double y = _model -> readParameter (element, "y").toDouble (&ok);
    if (ok) { setYAngle (y); }

    double z = _model -> readParameter (element, "z").toDouble (&ok);
    if (ok) { setZAngle (z); }
}

void QRotateModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "x", QString::number (xAngle()));
    _model -> writeParameter (_element, "y", QString::number (yAngle()));
    _model -> writeParameter (_element, "z", QString::number (zAngle()));
}
