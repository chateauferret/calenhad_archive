//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QVoronoiModule.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::qmodule;
using namespace noise::module;

QVoronoiModule::QVoronoiModule (QWidget* parent)  : QModule (new Voronoi(), parent) {

}

QVoronoiModule::~QVoronoiModule() {

}

void QVoronoiModule::initialise() {
    QModule::initialise();
    frequencySpin = parameterControl ("Frequency", "frequency");
    //connect (frequencySpin, SIGNAL (valueChanged (double)), this, SLOT (setFrequency (double)));
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    displacementSpin = parameterControl ("Displacement", "displacement");
    //connect (displacementSpin, SIGNAL (valueChanged (double)), this, SLOT (setDisplacement (double)));
    _contentLayout -> addRow (tr ("Power"), displacementSpin);
    enableDistanceCheck = new QCheckBox();
    enableDistanceCheck -> setChecked (false);
    //connect (enableDistanceCheck, SIGNAL (toggled (bool)), this, SLOT (setEnableDistance (bool)));
    module() -> EnableDistance (false);
    enableDistanceCheck -> setToolTip ("Enable distance");
    _contentLayout -> addRow (tr ("Enable distance"), enableDistanceCheck);
    _isInitialised = true;
    emit initialised();
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
    emit (nodeChanged());
    frequencySpin -> setValue (value);
}


void QVoronoiModule::setDisplacement (double value) {
    module() -> SetDisplacement (value);
    emit (nodeChanged());
    displacementSpin -> setValue (value);
}

void QVoronoiModule::setEnableDistance (bool value) {
    module() -> EnableDistance (value);
    emit (nodeChanged());
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

QString QVoronoiModule::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_voronoi;
}


QVoronoiModule* QVoronoiModule::clone () {
    QVoronoiModule* qm = QVoronoiModule::newInstance();
    if (qm) {
        qm -> setFrequency (frequency());
        qm -> setDisplacement (displacement());
        qm -> setEnableDistance (enableDistance());
    }
    return qm;
}

void QVoronoiModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double frequency = _model -> readParameter (element, "getFrequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }

    double displacement = _model -> readParameter (element, "displacement").toDouble (&ok);
    if (ok) { setDisplacement (displacement); }

    bool enableDistance = element.attribute ("enableDistance").toStdString() == "y";
    setEnableDistance (enableDistance);
}

void QVoronoiModule::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "getFrequency", QString::number (frequency()));
    _model -> writeParameter (_element, "displacement", QString::number (displacement()));
    _model -> writeParameter (_element, "enableDistance", enableDistance() ? "y" : "n");
}
