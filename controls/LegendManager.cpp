//
// Created by martin on 02/06/17.
//

#include <QtWidgets/QStackedLayout>
#include <iostream>
#include <QtWidgets/QMessageBox>
#include "LegendManager.h"
#include "../CalenhadServices.h"
#include "LegendChooser.h"
#include "CalenhadGlobeConfigDialog.h"


LegendManager::LegendManager (QWidget* parent) : QWidget (parent), _legendsChanged (false) {
    _service = CalenhadServices::legends();
    _chooser = new LegendChooser (_service, this);
    connect (_chooser, &LegendChooser::legendSelected, this, &LegendManager::showLegend);
    _legendDetailArea = new QWidget (this);
    _legendDetailArea -> setLayout (new QStackedLayout());
    setLayout (new QVBoxLayout);
    layout() -> addWidget (_chooser);
    layout() -> addWidget (_legendDetailArea);

    // install a widget into the stack for each of the available legends
    for (QString key : _service -> all().keys()) {
        Legend* legend = _service -> find (key);
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
    _chooser -> setCurrentText (legend -> name());
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
    _legendsChanged = true;
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
        _legendsChanged = true;
    } else {
        CalenhadServices::messages() -> message ("Cannot delete the last legend", "There must always be at least one legend available");
    }
}

void LegendManager::showEvent (QShowEvent* e) {
    _legendsChanged = false;
    _chooser -> refresh();

    // turn up the right widget for the legend currently in use
    ((QStackedLayout*)_legendDetailArea -> layout()) -> setCurrentWidget (_legend -> widget());

    _chooser -> setCurrentText (_legend -> name ());

}

LegendManager::~LegendManager () {

}

void LegendManager::commit () {
    if (_legendsChanged) {
        _service->commit ();
    }
}

void LegendManager::rollback () {
    if (_legendsChanged) {
        _service->rollback ();
    }
}

void LegendManager::addLegendWidget (Legend* legend) {
    LegendWidget* w = legend -> widget();
    ((QStackedLayout*) _legendDetailArea -> layout())->addWidget (w);
    connect (w, SIGNAL (nameChanged (QString)), legend, SLOT (setName (const QString&)));
    connect (w, SIGNAL (iconChanged (QIcon)), _chooser, SLOT (setCurrentIcon (const QIcon&)));
    connect (legend, &Legend::legendChanged, this, [=] () {
       _legendsChanged = true;
    });
    connect (legend, &Legend::renamed, this, [=] () {
        _chooser -> setItemText (_chooser -> currentIndex(), legend -> name());
        _chooser -> setCurrentText (legend -> name());
        _legendsChanged = true;
    });
}
