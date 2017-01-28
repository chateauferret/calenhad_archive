//
// Created by martin on 26/11/16.
//

#include <iostream>
#include "QClampModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"


using namespace noise::module;
QClampModule::QClampModule (QWidget* parent) : QRangeModule (new Clamp(), parent) {

}

Clamp* QClampModule::module () {
    return dynamic_cast<Clamp*> (_module);
}

QClampModule* QClampModule::newInstance() {
    QClampModule* qm = new QClampModule();
    qm -> initialise();
    return qm;
}

ModuleType QClampModule::type() {
    return ModuleType::CLAMP;
}

QClampModule* QClampModule::addCopy (CalenhadModel* model)  {
    QClampModule* qm = QClampModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setUpperBound (upperBound());
        qm -> setLowerBound (lowerBound());
    }
    return qm;
}

double QClampModule::upperBound() {
    return module() -> GetUpperBound();
}

double QClampModule::lowerBound() {
    return module() -> GetLowerBound();
}

void QClampModule::setBounds (double lowerBound, double upperBound) {
    module() -> SetBounds (lowerBound, upperBound);
}

QString QClampModule::typeString () {
    return "Clamp";
}

void QClampModule::initialise () {
    QRangeModule::initialise ();
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}
