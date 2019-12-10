//
// Created by martin on 27/09/17.
//

#include "LegendEditorSlider.h"
#include <QPainter>
#include <src/CalenhadServices.h>
#include "exprtk/Calculator.h"

using namespace calenhad::controls::legend;

LegendEditorSlider::LegendEditorSlider (int orientation, QColor col, QWidget* parent) : QWidget (parent), _orientation (orientation), _color (col), _computed (false) {
    if (_orientation == Qt::Horizontal) {
        setFixedSize (9, 16);
    } else {
        setFixedSize (16, 9);
    }
}

// -----------------------------------------------------------
void LegendEditorSlider::setColor (QColor col) {
    _color = col;
}

void LegendEditorSlider::setComputed (const bool& computed) {
    _computed = computed;
    assignCursor();
}

void LegendEditorSlider::assignCursor() {
    setCursor (_computed ? Qt::PointingHandCursor : Qt::OpenHandCursor);
}

// ---------------------------------------------------------
QColor LegendEditorSlider::color () {
    return _color;
}

// -----------------------------------------------------------
void LegendEditorSlider::paintEvent (QPaintEvent* e) {

    QPainter painter (this);
    painter.setPen (_computed ? Qt::gray : Qt::black);
    painter.setBrush (_color);
    if (_orientation == Qt::Horizontal) {
        QRect rec (0, 7, 8, 8);
        painter.drawRect (rec);
        QPolygon pp;
        pp << QPoint (0, 7) << QPoint (4, 0) << QPoint (8, 7);
        painter.drawPolygon (pp, Qt::OddEvenFill);
    } else {
        QRect rec (7, 0, 8, 8);
        painter.drawRect (rec);
        QPolygon pp;
        pp << QPoint (7, 0) << QPoint (0, 4) << QPoint (7, 8);
        painter.drawPolygon (pp, Qt::OddEvenFill);
    }

}

qreal LegendEditorSlider::value() const {
    return CalenhadServices::calculator() -> compute (_key);
}

void LegendEditorSlider::setKey (const QString& key) {
    _key = key;
}

QString LegendEditorSlider::key() {
    return _key;
}

bool LegendEditorSlider::isComputed () {
    return _computed;
}
