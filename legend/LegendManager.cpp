//
// Created by martin on 02/06/17.
//

#include "../messages/QNotificationHost.h"
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QMessageBox>
#include "Legend.h"
#include "LegendWidget.h"
#include "LegendManager.h"
#include "LegendService.h"
#include "CalenhadServices.h"
#include "LegendChooser.h"
#include "controls/globe/CalenhadGlobeConfigDialog.h"
#include "../messages/QNotificationHost.h"

using namespace calenhad::legend;
using namespace calenhad::notification;

LegendManager::LegendManager (QWidget* parent) : QWidget (parent), _legend (nullptr) {
    _service = CalenhadServices::legends();
    _chooser = new LegendChooser (_service, this);
    connect (_chooser, &LegendChooser::legendSelected, this, &LegendManager::showLegend);
    _legendDetailArea = new QWidget (this);
    _legendDetailArea -> setLayout (new QStackedLayout());
    setLayout (new QVBoxLayout);
    layout() -> addWidget (_chooser);
    layout() -> addWidget (_legendDetailArea);

    // install a widget into the stack for each of the available legends
    for (Legend* legend : _service -> all()) {
        addLegendWidget (legend);
    }

    // New and delete legend buttons
    _legendButtonPanel = new QWidget (this);
    _legendButtonPanel -> setLayout (new QHBoxLayout());
    layout() -> addWidget (_legendButtonPanel);
    layout() -> setAlignment (_legendButtonPanel, Qt::AlignRight);
    _newLegendButton = new QPushButton (this);
    _newLegendButton -> setText ("New legend");
    _legendButtonPanel -> layout() -> addWidget (_newLegendButton);
    connect (_newLegendButton, &QPushButton::pressed, this, &LegendManager::newLegend);

    _deleteLegendButton = new QPushButton (this);
    _deleteLegendButton -> setText ("Delete legend");
    _legendButtonPanel -> layout() -> addWidget (_deleteLegendButton);
    connect (_deleteLegendButton, &QPushButton::pressed, this, &LegendManager::deleteLegend);

}

Legend* LegendManager::currentLegend() {
    if (! _legend) {
        _legend = _chooser -> selectedLegend();
    }
    return _legend;
}

void LegendManager::setCurrentLegend (Legend* legend) {
    _legend = legend;
    if (_legend) {
        _chooser->setCurrentText (legend->name ());
    }
}

void LegendManager::showLegend () {
    _legend = _chooser -> selectedLegend();
    ((QStackedLayout*) _legendDetailArea -> layout())->setCurrentWidget (_legend -> widget());
}

void LegendManager::newLegend() {
    Legend* l = CalenhadServices::legends() -> find (_chooser -> currentText ());
    Legend* legend = new Legend (*l);
    _service -> provide (legend);
    // add new legend to the chooser and the widget stack
    addLegendWidget (legend);
    _chooser -> addItem (legend -> icon(), legend -> name());
    _chooser -> setCurrentText (legend -> name());
    _deleteLegendButton -> setEnabled (true);
}

void LegendManager::deleteLegend() {
    if (CalenhadServices::legends() -> legendCount() > 1) {
        Legend* l = _service -> find (_chooser -> currentText ());
        QMessageBox confirm;
        confirm.setText ("Delete legend " + l -> name() + "?");
        confirm.setStandardButtons (QMessageBox::Ok | QMessageBox::Cancel);
        confirm.setDefaultButton (QMessageBox::Ok);
        confirm.setEscapeButton (QMessageBox::Cancel);
        int result = confirm.exec ();
        if (result == QMessageBox::Ok) {
            // delete the selected legend
            CalenhadServices::legends() -> remove (_chooser -> currentText());
            // remove new legend from the chooser and the widget stack
            _legendDetailArea -> layout() -> removeWidget (((QStackedLayout*) _legendDetailArea -> layout()) -> currentWidget());
            _chooser -> removeItem (_chooser -> currentIndex());
        }
    } else {
        CalenhadServices::messages() -> message ("Last legend", "Cannot delete the last legend. There must always be at least one legend available", NotificationStyle::WarningNotification);
    }
}

void LegendManager::showEvent (QShowEvent* e) {
    _chooser -> refresh();
    // turn up the right widget for the legend currently in use
    if (_legend) {
        ((QStackedLayout*) _legendDetailArea->layout ())->setCurrentWidget (_legend->widget ());
        _chooser->setCurrentText (_legend->name ());
    }

}

LegendManager::~LegendManager () {

}

void LegendManager::addLegendWidget (Legend* legend) {
    LegendWidget* w = legend -> widget();
    QStackedLayout* layout = (QStackedLayout*) _legendDetailArea -> layout();
    layout -> addWidget (w);
    connect (w, SIGNAL (nameChanged (QString)), legend, SLOT (setName (const QString&)));
    connect (w, SIGNAL (iconChanged (QIcon)), _chooser, SLOT (setCurrentIcon (const QIcon&)));
    connect (legend, &Legend::renamed, this, [=] () {
        _chooser -> setItemText (_chooser -> currentIndex(), legend -> name());
        _chooser -> setCurrentText (legend -> name());
    });
}
