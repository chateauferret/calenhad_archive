//
// Created by martin on 05/06/17.
//
#include "StatisticsService.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include "CalenhadStatsPanel.h"
#include "CalenhadGlobeDialog.h"
#include "qmodule/Module.h"

using namespace calenhad::controls::globe;
using namespace calenhad::mapping;
using namespace calenhad::qmodule;

CalenhadStatsPanel::CalenhadStatsPanel (Module* source, CalenhadGlobeDialog* parent) : QWidget (parent), _source (source), dialog (parent) {
    setLayout (new QVBoxLayout());
    QGroupBox* worldHypsographyBox = new QGroupBox ("Planet hypsograph", this);
    worldHypsographyBox -> setLayout (new QVBoxLayout());
    _worldHypsography = new HypsographyWidget (source -> preview(), worldHypsographyBox);
    QFormLayout* worldForm = new QFormLayout();
    QWidget* worldFormWidget = new QWidget (this);
    worldFormWidget -> setLayout (worldForm);
    _worldExtremesLabel = new QLabel (this);
    _worldMeanLabel = new QLabel (this);
    worldForm -> addRow ("Range of values in world", _worldExtremesLabel);

    worldHypsographyBox -> layout () -> addWidget (_worldHypsography);
    worldHypsographyBox -> layout() -> addWidget (worldFormWidget);

    worldForm -> addRow ("Mean value in world", _worldMeanLabel);

    layout() -> addWidget (worldHypsographyBox);

}

CalenhadStatsPanel::~CalenhadStatsPanel() {

}

void CalenhadStatsPanel::showEvent (QShowEvent* e) {
    refresh();
}

void CalenhadStatsPanel::refresh() {
    Statistics worldStats = _worldHypsography -> statistics();
    if (worldStats.ok()) {
        _worldExtremesLabel -> setText (QString::number (worldStats._min) + " to " + QString::number (worldStats._max));
        _worldExtremesLabel -> setEnabled (true);
        _worldMeanLabel -> setText (QString::number (worldStats.mean()));
    } else {
        _worldExtremesLabel -> setText ("");
        _worldExtremesLabel -> setEnabled (false);
        _worldMeanLabel -> setText ("");
    }
}