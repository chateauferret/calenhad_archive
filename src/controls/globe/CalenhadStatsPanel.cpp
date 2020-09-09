//
// Created by martin on 05/06/17.
//
#include "CalenhadStatistics.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include "CalenhadStatsPanel.h"
#include "CalenhadGlobeWidget.h"
#include "module/Module.h"

using namespace calenhad::controls::globe;
using namespace calenhad::mapping;
using namespace calenhad::module;

CalenhadStatsPanel::CalenhadStatsPanel (CalenhadGlobeWidget* parent) : QWidget (parent), dialog (parent), _mapExtremesLabel (nullptr) {
    setLayout (new QVBoxLayout());
    QGroupBox* box = new QGroupBox ("Planet hypsograph", this);
    box -> setLayout (new QVBoxLayout());
    _hypsography = new HypsographyWidget (parent -> globe(), this);
    box -> layout() -> addWidget (_hypsography);
    QFormLayout* form = new QFormLayout();
    QWidget* formWidget = new QWidget (this);
    formWidget -> setLayout (form);
    _worldExtremesLabel = new QLabel (this);
    _worldMeanLabel = new QLabel (this);
    form -> addRow ("Range of values in world", _worldExtremesLabel);

    box -> layout() -> addWidget (formWidget);

    form -> addRow ("Mean value in world", _worldMeanLabel);

    _renderSizeLabel = new QLabel (this);
    form -> addRow ("Render size", _renderSizeLabel);
    _renderTimeLabel = new QLabel (this);
    form -> addRow ("Render time", _renderTimeLabel);
    layout() -> addWidget (box);

    parent -> globe() -> subscribe (this);
    refresh();
}

CalenhadStatsPanel::~CalenhadStatsPanel() {

}

void CalenhadStatsPanel::showEvent (QShowEvent* e) {

}

void CalenhadStatsPanel::refresh() {
    _hypsography -> refresh();
     CalenhadStatistics worldStats = _hypsography -> statistics();
    _renderSizeLabel -> setText ("");
    _renderSizeLabel -> setEnabled (false);
    _renderTimeLabel -> setText ("");
    _renderTimeLabel -> setEnabled (false);
    _worldExtremesLabel -> setText ("");
    _worldExtremesLabel -> setEnabled (false);
    _worldMeanLabel -> setText ("");
    _worldExtremesLabel -> setText (QString::number (worldStats._minValue) + " to " + QString::number (worldStats._maxValue));
    _worldMeanLabel -> setText (QString::number (worldStats._meanValue));
    _renderSizeLabel -> setEnabled (true);
    _renderSizeLabel -> setText (QString::number (worldStats._valueCount) + " values");
    if (worldStats._renderTime > 0) {
        _renderTimeLabel -> setEnabled (true);
        _renderTimeLabel -> setText (QString::number (worldStats._renderTime) + " ms");
    }
}