//
// Created by martin on 20/12/16.
//

#include "QLogSpinBox.h"
#include <cmath>

void QLogSpinBox::stepBy (int steps) {
    double newValue;
    if (steps < 0) {
        newValue = value() / (1 + singleStep());
    } else {
        newValue = value() * (1 + singleStep());
    }

    newValue = newValue * 100.0 + 0.5;
    newValue = (std::floor (newValue) / 100);
    setValue (newValue);
}

QLogSpinBox::QLogSpinBox (QWidget* parent) : QDoubleSpinBox (parent) {
    setRange (0.01, 100.0);
    setSingleStep (0.1);
}

QLogSpinBox::~QLogSpinBox() {

}