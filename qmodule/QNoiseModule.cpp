//
// Created by martin on 26/11/16.
//

#include <libnoise/module/modulebase.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QDoubleSpinBox>
#include <QDomElement>

#include "QNoiseModule.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../nodeedit/CalenhadView.h"
#include "../actions/ChangeModuleCommand.h"
#include "../nodeedit/CalenhadController.h"


QNoiseModule::QNoiseModule (noise::module::Module* m, QWidget* parent) : QModule (m, parent) {

}

QNoiseModule::~QNoiseModule () {

}

void QNoiseModule::initialise() {
    QModule::initialise();

    frequencySpin = logParameterControl ("Frequency");
    frequencySpin -> setValue (getFrequency());
    _contentLayout -> addRow (tr ("Frequency"), frequencySpin);
    connect (frequencySpin, SIGNAL (valueChanged (const double&)), this, SLOT (frequencyChangeRequested (const double&)));

    lacunaritySpin = logParameterControl ("Lacunarity");
    lacunaritySpin -> setValue (getLacunarity());
    _contentLayout -> addRow (tr ("Lacunarity"), lacunaritySpin);
    connect (lacunaritySpin, SIGNAL (valueChanged (const double&)), this, SLOT (lacunarityChangeRequested (const double&)));

    if (hasPersistence()) {
        persistenceSpin = logParameterControl ("Persistence");
        persistenceSpin -> setValue (getPersistence());
        _contentLayout -> addRow (tr ("Persistence"), persistenceSpin);
        connect (persistenceSpin, SIGNAL (valueChanged (const double&)), this, SLOT (persistenceChangeRequested (const double&)));
    }

    octaveSpin = countParameterControl ("Number of octaves");
    octaveSpin -> setValue (getOctaveCount());
    connect (octaveSpin, SIGNAL (valueChanged (const int&)), this, SLOT (octaveCountChangeRequested (const int&)));
    _contentLayout -> addRow (tr ("Octaves"), octaveSpin);
    _isInitialised = true;

    setFrequency (getFrequency ());
    setLacunarity (getLacunarity ());
    setOctaveCount (getOctaveCount ());
    setPersistence (getPersistence ());

    emit initialised();
}

void QNoiseModule::frequencyChangeRequested (const double& value) {
    if (_model && (getFrequency () != value)) {
        ChangeModuleCommand* c = new ChangeModuleCommand (this, "frequency", getFrequency (), value);
        if (_model) {
            _model->controller() -> doCommand (c);
        }
    }
    frequencySpin -> setValue (value);
    _model -> update();
}

void QNoiseModule::lacunarityChangeRequested (const double& value) {
    if (_model && (getLacunarity () != value)) {
        ChangeModuleCommand* c = new ChangeModuleCommand (this, "lacunarity", getLacunarity (), value);
        if (_model) {
            _model->controller() -> doCommand (c);
        }
    }
}

void QNoiseModule::persistenceChangeRequested (const double& value) {
    if (_model && (getPersistence () != value)) {
        ChangeModuleCommand* c = new ChangeModuleCommand (this, "persistence", getPersistence (), value);
        if (_model) {
            _model->controller() -> doCommand (c);
        }
    }
}

void QNoiseModule::octaveCountChangeRequested (const int& value) {
    if (_model && (getOctaveCount() != value)) {
        ChangeModuleCommand* c = new ChangeModuleCommand (this, "octaveCount", getOctaveCount (), value);
        if (_model) {
            _model->controller() -> doCommand (c);
        }
    }
}

double QNoiseModule::getFrequency () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetFrequency ();
    }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) {
        return billow -> GetFrequency();
    }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) {
        return rm -> GetFrequency();
    }
    return 0.0;
}

void QNoiseModule::setFrequency (const double& value) {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) { perlin -> SetFrequency (value); }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) { billow -> SetFrequency (value); }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) { rm -> SetFrequency (value); }
    if (perlin || billow || rm) {
        emit (nodeChanged ("getFrequency", value));
        //frequencySpin -> setValue (value);
        std::cout << "Set value " << value << "\n";
        return;
    }
}

double QNoiseModule::getLacunarity() {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetLacunarity();
    }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) {
        return billow -> GetLacunarity();
    }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) {
        return rm -> GetLacunarity();
    }
    return 0.0;
}

void QNoiseModule::setLacunarity (const double& value) {
    preserve();
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) { perlin -> SetLacunarity (value); }
    Billow* billow = dynamic_cast<Billow*> (_module);
    if (billow) { billow -> SetLacunarity (value); }
    RidgedMulti* rm = dynamic_cast<RidgedMulti*> (_module);
    if (rm) { rm -> SetLacunarity (value); }
    if (perlin || billow || rm) {
        emit (nodeChanged ("getLacunarity", value));
        //lacunaritySpin -> setValue (value);
        return;
    }
}

double QNoiseModule::getPersistence () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetPersistence();
    }
    return 0.0;
}

void QNoiseModule::setPersistence (const double& value) {
    preserve();
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        perlin -> SetPersistence (value);
        //persistenceSpin -> setValue (value);
        emit (nodeChanged ("getPersistence", value));
        return;
    }
}

int QNoiseModule::getOctaveCount () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        return perlin -> GetOctaveCount();
    }
    return 0;
}

void QNoiseModule::setOctaveCount (const int& value) {
    preserve();
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    if (perlin) {
        perlin -> SetOctaveCount (value);
        //octaveSpin -> setValue (value);
        emit (nodeChanged ("getOctaveCount", value));
        return;
    }
}

bool QNoiseModule::hasPersistence () {
    Perlin* perlin = dynamic_cast<Perlin*> (_module);
    return perlin != nullptr;
}

QNoiseModule* QNoiseModule::newRidgedMultiInstance() {
    RidgedMulti* rm = new RidgedMulti();
    rm -> SetSeed (seed);
    rm -> SetNoiseQuality (noiseQuality);
    Module* m = rm;
    return newInstance (m);
}

QNoiseModule* QNoiseModule::newBillowInstance() {
    Billow* b = new Billow();
    b -> SetSeed (seed);
    b -> SetNoiseQuality (noiseQuality);
    Module* m = b;
    return newInstance (m);
}

QNoiseModule* QNoiseModule::newPerlinInstance() {
    Perlin* p = new Perlin ();
    p -> SetSeed (seed);
    p -> SetNoiseQuality (noiseQuality);
    Module* m = p;
    return newInstance (m);
}

QNoiseModule* QNoiseModule::newInstance (Module* m) {
    QNoiseModule* qm = new QNoiseModule (m);
    qm -> initialise();
    return qm;
}

QString QNoiseModule::moduleType () {
    if (dynamic_cast<Perlin*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_perlin; }
    if (dynamic_cast<Billow*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_billow; }
    if (dynamic_cast<RidgedMulti*> (_module)) { return CalenhadServices::preferences() -> calenhad_module_ridgedmulti; }
    return QString::null;
}

QNoiseModule* QNoiseModule::addCopy (CalenhadModel* model)  {
    QNoiseModule* qm = nullptr;
    if (moduleType() == CalenhadServices::preferences() -> calenhad_module_perlin) { qm = newPerlinInstance(); }
    if (moduleType() == CalenhadServices::preferences() -> calenhad_module_billow) { qm = newBillowInstance(); }
    if (moduleType() == CalenhadServices::preferences() -> calenhad_module_ridgedmulti) { qm = newRidgedMultiInstance(); }
    if (qm) {
        qm -> setModel (model);
        qm -> setFrequency (getFrequency ());
        qm -> setLacunarity (getLacunarity ());
        qm -> setOctaveCount (getOctaveCount ());
        qm -> setPersistence (getPersistence ());
    }
    return qm;
}


void QNoiseModule::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;

    double frequency = _model -> readParameter (element, "getFrequency").toDouble (&ok);
    if (ok) { setFrequency (frequency); }

    double lacunarity = _model -> readParameter (element, "getLacunarity").toDouble (&ok);
    if (ok) { setLacunarity (lacunarity); }

    int octaves = _model -> readParameter (element, "octaves").toInt (&ok);
    if (ok) { setOctaveCount (octaves); }

    if (hasPersistence()) {
        double persistence = _model -> readParameter (element, "getPersistence").toDouble (&ok);
        if (ok) { setPersistence (persistence); }
    }
}

void QNoiseModule::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    _model -> writeParameter (_element, "getFrequency", QString::number (getFrequency ()));
    _model -> writeParameter (_element, "getLacunarity", QString::number (getLacunarity ()));
    _model -> writeParameter (_element, "octaves", QString::number (getOctaveCount ()));
    if (hasPersistence ()) {
        _model -> writeParameter (_element, "getPersistence", QString::number (getPersistence ()));
    }
}
