//
// Created by martin on 19/02/18.
//

#include "QColorButton.h"
#include <QColorDialog>

using namespace calenhad::controls::legend;

QColorButton::QColorButton (QWidget* parent ) {
    connect (this, &QPushButton::clicked, this, &QColorButton::changeColor);
}

void QColorButton::updateColor() {
    setStyleSheet( "background-color: " + _color.name() );
}

void QColorButton::changeColor() {
    QColor newColor = QColorDialog::getColor();
    if (newColor != _color) {
        setColor (newColor);
    }
}

void QColorButton::setColor( const QColor& color ) {
    _color = color;
    updateColor();
}

const QColor& QColorButton::color() {
    return _color;
}