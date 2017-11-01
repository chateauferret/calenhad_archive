//
// Created by martin on 05/06/17.
//
#include "StatisticsService.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include "CalenhadStatsTab.h"
#include "CalenhadGlobeDialog.h"
#include "../../qmodule/QModule.h"

using namespace calenhad::controls::globe;
using namespace calenhad::qmodule;

CalenhadStatsTab::CalenhadStatsTab (QModule* source, CalenhadGlobeDialog* parent) : QWidget (parent), _source (source), dialog (parent) {
    setLayout (new QVBoxLayout());
    QGroupBox* worldHypsographyBox = new QGroupBox ("Planet hypsograph", this);
    worldHypsographyBox -> setLayout (new QVBoxLayout());
    _worldHypsography = new HypsographyWidget (source -> preview(), worldHypsographyBox);
    QFormLayout* worldForm = new QFormLayout();
    QWidget* worldFormWidget = new QWidget (this);
    worldFormWidget -> setLayout (worldForm);
    _worldExtremesLabel = new QLabel (this);
    _worldMeanLabel = new QLabel (this);
    worldForm -> addRow ("Minimum and maximum values in world", _worldExtremesLabel);
    worldForm -> addRow ("Mean value in world", _worldMeanLabel);
    worldHypsographyBox -> layout () -> addWidget (_worldHypsography);
    worldHypsographyBox -> layout() -> addWidget (worldFormWidget);

    QGroupBox* mapHypsographyBox = new QGroupBox ("This map hypsograph", this);
    mapHypsographyBox -> setLayout (new QVBoxLayout());
    _mapHypsography = new HypsographyWidget (parent -> globe(), mapHypsographyBox, _worldHypsography);
    QFormLayout* mapForm = new QFormLayout();
    QWidget* mapFormWidget = new QWidget (this);
    mapFormWidget -> setLayout (mapForm);
    _mapExtremesLabel = new QLabel (this);
    _mapMeanLabel = new QLabel (this);
    mapForm -> addRow ("Minimum and maximum values in map", _mapExtremesLabel);
    mapForm -> addRow ("Mean value in map", _mapMeanLabel);
    mapHypsographyBox -> layout () -> addWidget (_mapHypsography);
    mapHypsographyBox -> layout() -> addWidget (mapFormWidget);

    layout() -> addWidget (worldHypsographyBox);
    layout() -> addWidget (mapHypsographyBox);
}

CalenhadStatsTab::~CalenhadStatsTab() {

}

void CalenhadStatsTab::showEvent (QShowEvent* e) {
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
    Statistics mapStats = _mapHypsography -> statistics();
    if (mapStats.ok()) {
        _mapExtremesLabel -> setText (QString::number (mapStats._min) + " to " + QString::number (mapStats._max));
        _mapExtremesLabel -> setEnabled (true);
        _mapMeanLabel -> setText (QString::number (mapStats.mean()));
    } else {
        _mapExtremesLabel -> setText ("");
        _mapExtremesLabel -> setEnabled (false);
        _mapMeanLabel -> setText ("");
    }
}