//
// Created by martin on 19/10/17.
//

#include "ColorButton.h"
#include <QPainter>

using namespace calenhad::controls;

ColorButton::ColorButton (QWidget* parent) : QPushButton (parent), _color (Qt::black), _dialog (new QColorDialog()) {

}

ColorButton::~ColorButton () {
    delete _dialog;
}

void ColorButton::showEvent (QShowEvent* event) {
    connect (this, &QPushButton::pressed, this, [=] () {
        _dialog -> exec();
        _color = _dialog -> currentColor();
    });
    connect (_dialog, &QDialog::accepted, this, [=] () {
        QColor c = _dialog -> selectedColor();
        setColor (c);
        emit colorSelected (c);
    });
}

QColor ColorButton::color () {
    return _color;
}

void ColorButton::setColor (const QColor& color) {
    _color = color;
    setToolTip (color.name());
    update();
}

void ColorButton::paintEvent (QPaintEvent* event) {
    QPushButton::paintEvent (event);
    QPainter p (this);
    QRect rect = QRect (5, 5, width() - 10, height() - 10);
    p.setPen (QPen (Qt::black));
    p.setBrush (QBrush (_color));
    p.drawRect (rect);
}
