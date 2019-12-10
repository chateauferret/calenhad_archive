//
// Created by martin on 27/05/17.
//

#include <iostream>
#include "Legend.h"
#include "LegendChooser.h"
#include "LegendService.h"
#include "src/CalenhadServices.h"

using namespace calenhad::legend;

LegendChooser::LegendChooser (LegendService* service, QWidget* parent) : QComboBox (parent),
    _service (service) {
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
    for (Legend* legend : _service -> all()) {
        addItem (legend -> icon(), legend -> name());
    }
    blockSignals (false);
}

void LegendChooser::legendChosen() {
    emit legendSelected (currentText());
}

Legend* LegendChooser::selectedLegend() {
    if (currentText().isNull ()) {
        setCurrentText (_service -> all().at (0) -> name ());
    }

    return _service -> find (currentText ());
}

void LegendChooser::setCurrentIcon (const QIcon& icon) {
    setItemIcon (currentIndex(), icon);
}

