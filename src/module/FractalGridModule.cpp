//
// Created by ferret on 07/02/2021.
//

#include "FractalGridModule.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"
#include "../grid/FractalGrid.h"
#include <QToolButton>

using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::grid;

FractalGridModule::FractalGridModule (const QString& type) : Cache (type),
                                                             _generateLevelVerticesLabel (new QLabel()), _sampleLevelVerticesLabel (new QLabel()),
                                                             _grid (nullptr), _generateAction (new QAction()), _generated (false), generateLevelBox(new QSpinBox()), sampleLevelBox(new QSpinBox()) {

    _generateAction -> setText ("Generate grid");
    connect (_generateAction, &QAction::triggered, this, [=] () { generate(); });
    QToolButton* generateButton = new QToolButton();
    generateButton -> setDefaultAction (_generateAction);

    generateLevelBox -> setMinimum (3);
    generateLevelBox -> setMaximum (9);
    generateLevelBox -> setValue (7);
    connect (generateLevelBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] () {
        _generateLevel = generateLevelBox -> value();
        int v = Icosphere::verticesForLevel (_generateLevel);
        _generateLevelVerticesLabel -> setText (QString::number (v) + " points");
    });

    sampleLevelBox -> setMinimum (2);
    sampleLevelBox -> setMaximum (7);
    sampleLevelBox -> setValue (3);
    connect (sampleLevelBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] () {
        _sampleLevel = sampleLevelBox -> value();
        int v = Icosphere::verticesForLevel (_sampleLevel);
        _sampleLevelVerticesLabel -> setText (QString::number (v) + " points");
        generateLevelBox -> setMinimum (_sampleLevel + 1);
        if (_generateLevel <= _sampleLevel) {
            generateLevelBox -> setValue (_sampleLevel + 1);
        }
    });

    QLabel* sampleLevelCaptionLabel = new QLabel(), * generateLevelCaptionLabel = new QLabel();
    generateLevelCaptionLabel-> setText ("Generate to level");
    sampleLevelCaptionLabel -> setText ("Sample input at level");

    QWidget* generateLevelWidget = new QWidget();
    generateLevelWidget -> setLayout (new QHBoxLayout());
    generateLevelWidget -> layout() -> addWidget (generateLevelCaptionLabel);
    generateLevelWidget -> layout() -> addWidget (generateLevelBox);
    generateLevelWidget -> layout() -> addWidget (_generateLevelVerticesLabel);

    QWidget* sampleLevelWidget = new QWidget();
    sampleLevelWidget -> setLayout (new QHBoxLayout());
    sampleLevelWidget -> layout() -> addWidget (sampleLevelCaptionLabel);
    sampleLevelWidget -> layout() -> addWidget (sampleLevelBox);
    sampleLevelWidget -> layout() -> addWidget (_sampleLevelVerticesLabel);


    if (!(_content)) {
        addContentPanel();
    }
    _contentLayout -> addWidget (generateLevelWidget);
    _contentLayout -> addWidget (sampleLevelWidget);
    _contentLayout -> addWidget (generateButton);

}

FractalGridModule::~FractalGridModule () {
    delete _grid;
}

void FractalGridModule::invalidate() {
    _generated = false;
    Cache::invalidate();
}

void FractalGridModule::refresh() {
    if (_generated) {
        _computed = true;
        CalenhadServices::compute() -> compute (this, _buffer);
        emit nodeChanged();
    };
}

void FractalGridModule::generate() {
    delete _grid;
    Module* in = inputs() [0] -> connections() [0]->otherEnd (inputs() [0]) -> owner();
    in -> fetch (_buffer);
    _grid = new FractalGrid (_generateLevel, _sampleLevel, (int) parameterValue ("seed"), _buffer);
    _buffer -> fromIcosphere (_grid);
    _generated = true;
    refresh();
}

bool FractalGridModule::isComplete() {
    return Cache::isComplete() && _grid && _generated;
}
