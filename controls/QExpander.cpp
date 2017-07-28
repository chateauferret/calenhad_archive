//
// Created by martin on 18/11/16.
//

#include "QExpander.h"
#include <iostream>

using namespace calenhad::controls;

QExpander::QExpander (const QString& title, QWidget* parent) : QToolBar (parent) {
    /*_button = new QButton (this);
    _button -> setText (title);
    _button -> setCheckable (true);
    _button -> setChecked (true);
    addNotification (_button);*/
    setOrientation (Qt::Vertical);
}

void QExpander::showEvent (QShowEvent* event) {
    connect (_button, &QPushButton::clicked, this, &QExpander::toggle);
}

QExpander::~QExpander () {
}

void QExpander::toggle() {
    setExpanded (_button -> isChecked());
}

void QExpander::setExpanded (const bool& expanded) {
    QList<QWidget*> children = findChildren<QWidget*>();

    for (QWidget* child : children) {
        if (child != _button) {
            if (expanded) {
               child -> setVisible (false);
            } else {
               child -> setVisible (true);
            }
        }
    }
    emit expansionChanged (expanded);
}

void QExpander::setTitle (const QString& title) {
    _button -> setText (title);
}

bool QExpander::isExpanded () const {
    return _button -> isChecked();
}
