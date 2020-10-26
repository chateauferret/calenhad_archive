//
// Created by ferret on 16/10/2020.
//

#include "TectonicsModule.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"
#include "../grid/Tectonics.h"
#include <QToolButton>

using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::grid;

TectonicsModule::TectonicsModule (const QString& type) : Cache (type),
                                  _tectonics (nullptr), _simulated (false), _simulateAction (nullptr) {
    _simulateAction = new QAction();
    _simulateAction -> setText ("Run simulation");
    connect (_simulateAction, &QAction::triggered, this, &TectonicsModule::simulate);
    QToolButton* simulateButton = new QToolButton();
    simulateButton -> setDefaultAction (_simulateAction);
    if (!(_content)) {
        addContentPanel();
    }
    _contentLayout -> addWidget (simulateButton);
}

TectonicsModule::~TectonicsModule () {
    delete _tectonics;
}

void TectonicsModule::invalidate() {
    _simulated = false;
    Cache::invalidate();
}

void TectonicsModule::refresh() {

    if (isComplete()) {
        _buffer -> fromIcosphere (_tectonics);
        _computed = true;
        CalenhadServices::compute() -> compute (this, _buffer);

    }
}

void TectonicsModule::simulate() {
    if (! inputs().isEmpty()) {
        CubicSphere* _oceanic = new CubicSphere ();
        CubicSphere* _continental = new CubicSphere ();
        // fetch rasters for the initial continental and oceanic crust thickness
        Port* p = inputs ()[1];
        Module* source = p -> connections().first() -> otherEnd(p) -> owner();
        source -> fetch (_oceanic);

        p = inputs ()[0];
        source = p -> connections().first()->otherEnd (p) -> owner();
        source -> fetch (_continental);

        delete _tectonics;
        _tectonics = new Tectonics();
        _tectonics -> initialise ((int) parameterValue ("seed"), (int) parameterValue ("plates"));
        _tectonics -> createInitialCrust (_oceanic, _continental);
        _tectonics -> run();
        qDebug ("Run complete");
        delete _continental;
        delete _oceanic;
        _simulated = true;
        refresh();
    }
}

bool TectonicsModule::isComplete() {
    return Cache::isComplete() && _tectonics && _simulated;
}