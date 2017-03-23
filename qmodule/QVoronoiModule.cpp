//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QVoronoiModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"


QVoronoiModule::QVoronoiModule (QWidget* parent)  : QModule (new Voronoi(), parent) {

}

QVoronoiModule::~QVoronoiModule() {

}

void QVoronoiModule::initialise() {
    QModule::initialise();
    frequencySpin = logParameterControl ("Frequency");
    connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    displacementSpin = logParameterControl ("Displacement");
    connect (displacementSpin, SIGNAL (valueChanged (double)), this, SLOT (setDisplacement (double)));
    _contentLayout -> addRow (tr ("Power"), displacementSpin);
    enableDistanceCheck = new QCheckBox();
    enableDistanceCheck -> setChecked (false);
    connect (enableDistanceCheck, SIGNAL (toggled (bool)), this, SLOT (setEnableDistance (bool)));
    module() -> EnableDistance (false);
    enableDistanceCheck -> setToolTip ("Enable distance");
    _contentLayout -> addRow (tr ("Enable distance"), enableDistanceCheck);
    _isInitialised = true;
    emit nodeChanged ("initialised", 0);
}

double QVoronoiModule::frequency() {
    return module() -> GetFrequency();
}

double QVoronoiModule::displacement() {
    return module() -> GetDisplacement();
}

bool QVoronoiModule::enableDistance() {
    return module() -> IsDistanceEnabled();
}


void QVoronoiModule::setFrequency (double value) {
    module() -> SetFrequency (value);
    emit (nodeChanged ("frequency", value));
    frequencySpin -> setValue (value);
}


void QVoronoiModule::setDisplacement (double value) {
    module() -> SetDisplacement (value);
    emit (nodeChanged ("power", value));
    displacementSpin -> setValue (value);
}

void QVoronoiModule::setEnableDistance (bool value) {
    module() -> EnableDistance (value);
    emit (nodeChanged ("roughness", value));
    enableDistanceCheck -> setChecked (value);
}

Voronoi* QVoronoiModule::module () {
    Voronoi* t = dynamic_cast<Voronoi*> (_module);
    return t;
}

QVoronoiModule* QVoronoiModule::newInstance () {
    QVoronoiModule* qm = new QVoronoiModule();
    qm -> initialise();
    return qm;
}

ModuleType QVoronoiModule::type() {
    return ModuleType::VORONOI;
}


QVoronoiModule* QVoronoiModule::addCopy (CalenhadModel* model) {
    QVoronoiModule* qm = QVoronoiModule::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setFrequency (frequency());
        qm -> setDisplacement (displacement());
        qm -> setEnableDistance (enableDistance());
    }
    return qm;
}

QString QVoronoiModule::typeString () {
    return "Voronoi";
}
