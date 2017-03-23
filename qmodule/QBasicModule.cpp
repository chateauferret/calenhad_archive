//
// Created by martin on 07/12/16.
//

#include <iostream>
#include "QBasicModule.h"
#include "../libnoiseutils/diff.h"

QBasicModule::QBasicModule (Module* m, QWidget* parent) : QModule (m, parent) {

}

QBasicModule::~QBasicModule() {

}


QBasicModule* QBasicModule::newInstance (const ModuleType& type) {
    QBasicModule* qm = nullptr;
    if (type == ModuleType::ADD) { Add* m = new Add(); qm = new QBasicModule (m); }
    if (type == ModuleType::ABS) { Abs* m = new Abs(); qm = new QBasicModule (m); }
    if (type == ModuleType::BLEND) { Blend* m = new Blend(); qm = new QBasicModule (m); }
    if (type == ModuleType::CACHE) { noise::module::Cache* m = new noise::module::Cache(); }
    if (type == ModuleType::CHECKERBOARD) { Checkerboard* m = new Checkerboard(); qm = new QBasicModule (m); }
    if (type == ModuleType::INVERT) { Invert* m = new Invert(); qm = new QBasicModule (m); }
    if (type == ModuleType::MAX) { Max* m = new Max(); qm = new QBasicModule (m); }
    if (type == ModuleType::MIN) { Min* m = new Min(); qm = new QBasicModule (m); }
    if (type == ModuleType::MULTIPLY) { Multiply* m = new Multiply(); qm = new QBasicModule (m); }
    if (type == ModuleType::POWER) { Power* m = new Power(); qm = new QBasicModule (m); }
    if (type == ModuleType::DISPLACE) { Displace* m = new Displace(); qm = new QBasicModule (m); }
    if (type == ModuleType::DIFF) { Diff* m = new Diff(); qm = new QBasicModule (m); }
    if (qm) {
        qm -> initialise();
    }
    return qm;
}

QBasicModule* QBasicModule::addCopy (CalenhadModel* model) {
    QBasicModule* qm = newInstance (type());
    if (qm) {
        qm -> setModel (model);
    }
    return qm;
}


QString QBasicModule::typeString() {
    if (dynamic_cast<Add*> (_module)) { return "Addition"; }
    if (dynamic_cast<Abs*> (_module)) { return "Absolute"; }
    if (dynamic_cast<Blend*> (_module)) { return "Blend"; }
    if (dynamic_cast<noise::module::Cache*> (_module)) { return "Cache"; }
    if (dynamic_cast<Checkerboard*> (_module)) { return "Checkerboard"; }
    if (dynamic_cast<Invert*> (_module)) { return "Invert"; }
    if (dynamic_cast<Max*> (_module)) { return "Maximum"; }
    if (dynamic_cast<Min*> (_module)) { return "Minimum"; }
    if (dynamic_cast<Multiply*> (_module)) { return "Multiply"; }
    if (dynamic_cast<Power*> (_module)) { return "Power"; }
    if (dynamic_cast<Displace*> (_module)) { return "Displace"; }
    if (dynamic_cast<Diff*> (_module)) { return "Difference"; }
    return "";
}

ModuleType QBasicModule::type() {
    if (dynamic_cast<Add*> (_module)) { return ModuleType::ADD; }
    if (dynamic_cast<Abs*> (_module)) { return ModuleType::ABS; }
    if (dynamic_cast<Blend*> (_module)) { return ModuleType::BLEND; }
    if (dynamic_cast<noise::module::Cache*> (_module)) { return ModuleType::CACHE; }
    if (dynamic_cast<Checkerboard*> (_module)) { return ModuleType::CHECKERBOARD; }
    if (dynamic_cast<Invert*> (_module)) { return ModuleType::INVERT; }
    if (dynamic_cast<Max*> (_module)) { return ModuleType::MAX; }
    if (dynamic_cast<Min*> (_module)) { return ModuleType::MIN; }
    if (dynamic_cast<Multiply*> (_module)) { return ModuleType::MULTIPLY; }
    if (dynamic_cast<Power*> (_module)) { return ModuleType::POWER; }
    if (dynamic_cast<Displace*> (_module)) { return ModuleType::DISPLACE; }
    if (dynamic_cast<Diff*> (_module)) { return ModuleType::DIFF; }
    return ModuleType::NONE;
}

void QBasicModule::initialise () {
    QModule::initialise ();
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}
