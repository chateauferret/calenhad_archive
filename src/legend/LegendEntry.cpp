//
// Created by martin on 16/02/18.
//

#include <src/CalenhadServices.h>
#include "exprtk/Calculator.h"
#include "LegendEntry.h"

using namespace calenhad::legend;

LegendEntry::LegendEntry() : _key ("0.0"), _color (QColor ("black")) {}

LegendEntry::LegendEntry (const QString& key, const QColor& color) : _key (key), _color (color) {

}

LegendEntry::~LegendEntry() {

}

void LegendEntry::setKey (const QString& key) {
    _key = key;
}

void LegendEntry::setKey (const double& key) {
    _key = QString::number (key);
}

void LegendEntry::setColor (const QColor& color) {
    _color = color;
}

QString LegendEntry::key ()const {
    return _key;
}

double LegendEntry::keyValue () {
    return CalenhadServices::calculator() -> compute (_key);
}

QString LegendEntry::label () {
    double value = keyValue();
    if (CalenhadServices::calculator() -> hasErrors()) {
        QString errors;
        for (QString error : CalenhadServices::calculator () -> errors()) {
            errors += error + "\n";
        }
        return errors;
    } else {
        return _key + (QString::number (value) == _key ? "" : " = " + QString::number (value));
    }
}

QColor LegendEntry::color ()const {
    return _color;
}

bool LegendEntry::operator== (const LegendEntry& other) {
        return key() == other.key() && color() == other.color();
};

bool LegendEntry::isComputed()const {
    bool ok;
    double v = _key.toDouble (&ok);
    return ! ok;
}
