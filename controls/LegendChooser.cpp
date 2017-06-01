//
// Created by martin on 27/05/17.
//

#include <iostream>
#include "LegendChooser.h"
#include "../CalenhadServices.h"

LegendChooser::LegendChooser (QWidget* parent) : QComboBox (parent) {
    setIconSize (QSize (100, 20));
    connect (this, &QComboBox::currentTextChanged, this, &LegendChooser::legendChosen);
}

LegendChooser::~LegendChooser () {

}

void LegendChooser::showEvent (QShowEvent* e) {
    QComboBox::showEvent (e);
}

void LegendChooser::setLabel (const QString& oldLabel, const QString& newLabel) {
    for (int i = 0; i < count(); i++) {
        if (oldLabel == itemText (i)) {
            setItemText (i, newLabel);
        }
    }
}


void LegendChooser::refresh() {
    blockSignals (true);
    clear();
    for (Legend* legend : CalenhadServices::legends() -> all()) {
        addItem (legend -> widget() -> icon(), legend -> name());
    }
    blockSignals (false);
}

void LegendChooser::legendChosen() {
    emit legendSelected (currentText());
}

Legend* LegendChooser::selectedLegend() {
    std::cout << "Current text '" << currentText().toStdString () << "'\n";
    if (! currentText().isNull ()) {
        return CalenhadServices::legends ()->find (currentText ());
    }
}

void LegendChooser::setCurrentIcon (const QIcon& icon) {
    setItemIcon (currentIndex(), icon);
}

