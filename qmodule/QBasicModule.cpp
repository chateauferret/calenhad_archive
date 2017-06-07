//
// Created by martin on 07/12/16.
//

#include <iostream>
#include "QBasicModule.h"
#include "../libnoiseutils/diff.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"
#include "../CalenhadServices.h"

QBasicModule::QBasicModule (Module* m, QWidget* parent) : QModule (m, parent) {

}

QBasicModule::~QBasicModule() {

}

QBasicModule* QBasicModule::newInstance (const QString& type) {
    QBasicModule* qm = nullptr;
    if (type == CalenhadServices::preferences() -> calenhad_module_add) { Add* m = new Add(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_abs) { Abs* m = new Abs(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_blend) { Blend* m = new Blend(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_cache) { noise::module::Cache* m = new noise::module::Cache(); }
    if (type == CalenhadServices::preferences() -> calenhad_module_checkerboard) { Checkerboard* m = new Checkerboard(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_invert) { Invert* m = new Invert(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_max) { Max* m = new Max(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_min) { Min* m = new Min(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_multiply) { Multiply* m = new Multiply(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_power) { Power* m = new Power(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_displace) { Displace* m = new Displace(); qm = new QBasicModule (m); }
    if (type == CalenhadServices::preferences() -> calenhad_module_diff) { Diff* m = new Diff(); qm = new QBasicModule (m); }
    if (qm) {
        qm -> initialise();
    }
    return qm;
}

QBasicModule* QBasicModule::clone () {
    return newInstance (nodeType ());
}


QString QBasicModule::nodeType () {
    if (dynamic_cast<Add*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_add; }
    if (dynamic_cast<Abs*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_abs; }
    if (dynamic_cast<Blend*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_blend; }
    if (dynamic_cast<noise::module::Cache*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_cache; }
    if (dynamic_cast<Checkerboard*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_checkerboard; }
    if (dynamic_cast<Invert*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_invert; }
    if (dynamic_cast<Max*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_max; }
    if (dynamic_cast<Min*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_min; }
    if (dynamic_cast<Multiply*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_multiply; }
    if (dynamic_cast<Power*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_power; }
    if (dynamic_cast<Displace*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_displace; }
    if (dynamic_cast<Diff*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_diff; }
    return "";
}

void QBasicModule::initialise () {
    QModule::initialise ();
    _isInitialised = true;
    emit initialised();
}

void QBasicModule::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    _element.setAttribute ("nodeType", nodeType ());

}

void QBasicModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    // element nodeType is done in Node class - to do
}

bool QBasicModule::hasParameters () {
    return false;
}
