//
// Created by martin on 19/11/17.
//

#include <cmath>
#include <iostream>
#include <QtWidgets/QGroupBox>
#include <QRegularExpression>
#include <geoutils.h>
#include "QAngleControl.h"

using namespace calenhad::controls;
using namespace calenhad::module;
using namespace geoutils;

QAngleControl::QAngleControl (const QString& caption, const AngleType& type, QWidget* parent) : QWidget (parent), _type (type), _statusLabel (nullptr), _validator (nullptr), _isValid (false) {

    _statusLabel = new QLabel (this);

    _statusOrright = QPixmap (":/appicons/status/orright.png");
    _statusGoosed = QPixmap (":/appicons/status/goosed.png");
    _statusLabel -> setPixmap (_statusOrright);
    setLayout (new QHBoxLayout());
    layout() -> setSpacing(0);
    layout() -> setMargin(0);
    layout() -> setContentsMargins(0,0,0,0);
    _stack = new QStackedLayout();
    _stack->setSpacing(0);
    _stack->setMargin(0);
    _stack->setContentsMargins(0,0,0,0);

    _layout = new QHBoxLayout();
    _layout->setSpacing(0);
    _layout->setMargin(0);
    _layout->setContentsMargins(0,0,0,0);
    QGroupBox* box = new QGroupBox();
    box -> setTitle (caption);
    box -> setLayout (_layout);

    _tradLayout = new QHBoxLayout();
    _tradLayout->setSpacing(0);
    _tradLayout->setMargin(0);
    _tradLayout->setContentsMargins(0,0,0,0);
    _tradDegreesText = new QLineEdit();

    _tradLayout -> addWidget (_tradDegreesText);
    _tradDegreesText -> setInputMask ("009°00\'00\"A");
    QWidget* tradWidget = new QWidget();
    tradWidget -> setLayout (_tradLayout);
    _stack -> addWidget (tradWidget);
    _decimalLayout = new QHBoxLayout();
    _decimalLayout->setSpacing(0);
    _decimalLayout->setMargin(0);
    _decimalLayout->setContentsMargins(0,0,0,0);
    _decimalDegreesText = new QLineEdit();
    _decimalDegreesText -> setInputMask ("009.000000°A");
    _decimalLayout -> addWidget (_decimalDegreesText);
    QWidget* decimalWidget = new QWidget();
    decimalWidget -> setLayout (_decimalLayout);
    _stack -> addWidget (decimalWidget);

    QWidget* stackWidget = new QWidget();
    stackWidget -> setLayout (_stack);
    _layout -> addWidget (_statusLabel);
    _layout -> addWidget (stackWidget);
    _formatButton = new QPushButton();

    _layout -> addWidget (_formatButton);

    connect (_formatButton, &QPushButton::pressed, this, &QAngleControl::toggleFormat);
    connect (_decimalDegreesText, &QLineEdit::textChanged, this, &QAngleControl::decimalUpdated);
    connect (_tradDegreesText, &QLineEdit::textChanged, this, &QAngleControl::tradUpdated);
    layout() -> addWidget (box);

    trimBox (_decimalDegreesText);
    trimBox (_tradDegreesText);
    trimBox (_formatButton);
    setValidator (new AcceptAngle (type));
}

QAngleControl::~QAngleControl () {
    if (_validator) { delete _validator; }
}

void QAngleControl::refresh() {
    QString letter;
    if (_type == geoutils::AngleType::Latitude) {
        letter = _value >= 0 ? "N" : "S";
    } else {
        letter = _value >= 0 ? "E" : "W";
    }
    QString tradText = geoutils::Math::toTraditional (std::abs (_value), 4) + letter;
    QString decimalText = QString::number (std::abs (_value)) + "°" + letter;
    _tradDegreesText -> setText (tradText);
    _decimalDegreesText -> setText (decimalText);
}

void QAngleControl::trimBox (QWidget* w) {
    QFontMetrics m (w -> font());
    int rowHeight = m.lineSpacing();
    w -> setFixedHeight  (rowHeight + 6);
}

double QAngleControl::value () {
    return _value;
}

void QAngleControl::decimalUpdated() {
    bool ok;
    QString text = _decimalDegreesText -> text().trimmed();
    text = text.toUpper();
    int sign = (text.endsWith ("W") || text.endsWith ("S")) ? -1 : 1;

    text = text.split ("°").first();
    double degrees = sign * text.toDouble (&ok);
    if (ok && degrees != _value) {
        setValue (degrees);
    }
}

void QAngleControl::tradUpdated() {
    bool ok;
    QString text = _tradDegreesText -> text().trimmed();
    text = text.toUpper();
    int sign = (text.endsWith ("W") || text.endsWith ("S")) ? -1 : 1;
    QStringList tokens = text.split ("°");
    int degrees = tokens.first().toInt (&ok);
    tokens = tokens.at (1).split ("\'");
    int minutes = tokens.first().toInt (&ok);
    QString s = tokens.at (1);
    s = s.split ("\"").first();

    double seconds = s.toDouble (&ok);
    double value = sign * (degrees + minutes / 60.0 + seconds / 3600.0);
    if (ok && value != _value) {
        setValue (value);
    } else {
        if (! ok) {
            setToolTip (_validator->toString (value));
            _statusLabel->setPixmap (_statusGoosed);
        }
    }
}

void QAngleControl::setValue (const double& value) {
    if (value != _value) {
        _value = value;
    }

    validate();
}

void QAngleControl::toggleFormat() {
    _stack -> setCurrentIndex (_stack -> currentIndex() == 0 ? 1 : 0);
    refresh();
}

AngleType QAngleControl::angleType () {
    return _type;
}

void QAngleControl::setValidator (calenhad::module::AcceptAngle* validator) {
    _validator = validator;
}

void QAngleControl::validate() {
    if ((! _validator) || _validator -> isInValidSet (_value)) {
        setToolTip (QString::null);
        _statusLabel -> setPixmap (_statusOrright);
        _isValid = true;
    } else {
        setToolTip (_validator->toString (_value));
        _statusLabel->setPixmap (_statusGoosed);
        _isValid = false;
    }
    emit valueChanged (_value);
}

bool QAngleControl::isValid() {
    return _isValid;
}
