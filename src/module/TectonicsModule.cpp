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
                                  _tectonics (nullptr), _simulateAction (new QAction()), _simulated (false) {

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
    if (_simulated) {
        _computed = true;
        CalenhadServices::compute() -> compute (this, _buffer);
        emit nodeChanged();
    };
}

void TectonicsModule::simulate() {
    if (! _tectonics) {
        _tectonics = new Tectonics();
    }
    _tectonics -> initialise ((int) parameterValue ("seed"), (int) parameterValue ("plates"));
    _tectonics -> createInitialCrust();
    _tectonics -> run();
    qDebug ("Run complete");
    _buffer -> fromIcosphere (_tectonics);
    _simulated = true;
    refresh();
}

bool TectonicsModule::isComplete() {
    return Cache::isComplete() && _tectonics && _simulated;
}
