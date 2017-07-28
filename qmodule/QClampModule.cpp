//
// Created by martin on 26/11/16.

#include "../preferences/PreferencesService.h"
#include "../CalenhadServices.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "QClampModule.h"

using namespace noise::module;
using namespace calenhad::qmodule;

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

QString QClampModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_clamp;
}

QClampModule* QClampModule::clone () {
    QClampModule* qm = QClampModule::newInstance();
    if (qm) {
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

void QClampModule::initialise () {
    QRangeModule::initialise ();
    _isInitialised = true;
    emit initialised();
}


void QClampModule::inflate (const QDomElement& element) {
    QRangeModule::inflate (element);
}

void QClampModule::serialize (QDomDocument& doc) {
    QRangeModule::serialize (doc);
}
