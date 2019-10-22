//
// Created by martin on 05/06/17.
//
#include "StatisticsService.h"
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include "CalenhadStatsPanel.h"
#include "CalenhadGlobeWidget.h"
#include "module/Module.h"

using namespace calenhad::controls::globe;
using namespace calenhad::mapping;
using namespace calenhad::module;

CalenhadStatsPanel::CalenhadStatsPanel (Module* source, CalenhadGlobeWidget* parent) : QWidget (parent), _source (source), dialog (parent) {
    setLayout (new QVBoxLayout());
    QGroupBox* worldHypsographyBox = new QGroupBox ("Planet hypsograph", this);
    worldHypsographyBox -> setLayout (new QVBoxLayout());
    _hypsography = new HypsographyWidget (source -> preview(), worldHypsographyBox);
    QFormLayout* worldForm = new QFormLayout();
    QWidget* worldFormWidget = new QWidget (this);
    worldFormWidget -> setLayout (worldForm);
    _worldExtremesLabel = new QLabel (this);
    _worldMeanLabel = new QLabel (this);
    worldForm -> addRow ("Range of values in world", _worldExtremesLabel);

    worldHypsographyBox -> layout () -> addWidget (_hypsography);
    worldHypsographyBox -> layout() -> addWidget (worldFormWidget);

    worldForm -> addRow ("Mean value in world", _worldMeanLabel);

    _renderSizeLabel = new QLabel (this);
    worldForm -> addRow ("Render size", _renderSizeLabel);
    _renderTimeLabel = new QLabel (this);
    worldForm -> addRow ("Render time", _renderTimeLabel);
    layout() -> addWidget (worldHypsographyBox);

}

CalenhadStatsPanel::~CalenhadStatsPanel() {

}

void CalenhadStatsPanel::showEvent (QShowEvent* e) {
    refresh();
}

void CalenhadStatsPanel::refresh() {
    _hypsography -> refresh();
    Statistics worldStats = _hypsography -> statistics();
    _renderSizeLabel -> setText ("");
    _renderSizeLabel -> setEnabled (false);
    _renderTimeLabel -> setText ("");
    _renderTimeLabel -> setEnabled (false);
    _worldExtremesLabel -> setText ("");
    _worldExtremesLabel -> setEnabled (false);
    _worldMeanLabel -> setText ("");
    if (worldStats.ok()) {
        _worldExtremesLabel->setText (QString::number (worldStats._min) + " to " + QString::number (worldStats._max));
        _worldExtremesLabel->setEnabled (true);
        _worldMeanLabel->setText (QString::number (worldStats.mean ()));
        if (worldStats._size > 0) {
            _renderSizeLabel->setEnabled (true);
            _renderSizeLabel->setText (QString::number (worldStats._size * 2) + " x " + QString::number (worldStats._size));
        }
        if (worldStats._renderTime > 0) {
            _renderTimeLabel->setEnabled (true);
            _renderTimeLabel->setText (QString::number (worldStats._renderTime) + " ms");
        }
    }
}