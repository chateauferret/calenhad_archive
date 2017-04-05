//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include "QVoronoiModule.h"
#include "../pipeline/ModuleFactory.h"
#include "QNode.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"


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

QString QVoronoiModule::moduleType () {
    return Calenhad::preferences -> calenhad_module_voronoi;
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

void QVoronoiModule::inflate (const QDomElement& element, MessageFactory* messages) {
    QModule::inflate (element, messages);
    bool ok;

    double frequency = _model -> readParameter (element, "frequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }

    double displacement = _model -> readParameter (element, "displacement").toDouble (&ok);
    if (ok) { setDisplacement (displacement); }

    bool enableDistance = element.attribute ("enableDistance").toStdString() == "y";
    setEnableDistance (enableDistance);
}

void QVoronoiModule::serialise (QDomDocument& doc, MessageFactory* messages) {
    QModule::serialise (doc, messages);
    _model -> writeParameter (_element, "frequency", QString::number (frequency()));
    _model -> writeParameter (_element, "displacement", QString::number (displacement()));
    _model -> writeParameter (_element, "enableDistance", enableDistance() ? "y" : "n");
}
