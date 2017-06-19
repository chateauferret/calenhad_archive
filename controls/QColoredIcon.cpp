//
// Created by martin on 17/06/17.
//

#include <QtWidgets/QGraphicsColorizeEffect>
#include "QColoredIcon.h"

QColoredIcon::QColoredIcon (QWidget* parent) : QLabel (parent), _colorizer (new QGraphicsColorizeEffect()) {
    setGraphicsEffect (_colorizer);
    setAttribute (Qt::WA_Hover, true);
}


QColoredIcon::~QColoredIcon () {
    if (_colorizer) { delete _colorizer; }
}

void QColoredIcon::setColor (const QColor& color) {
    _color = color;
    _colorizer -> setColor (_color);
}

void QColoredIcon::setMouseOverColor (const QColor& color) {
    _mouseOverColor = color;
}

void QColoredIcon::enterEvent(QEvent * event) {
   _colorizer -> setColor (_mouseOverColor);
    QWidget::enterEvent(event);
}

void QColoredIcon::leaveEvent(QEvent * event) {
    _colorizer -> setColor (_color);
    QWidget::leaveEvent(event);
}
