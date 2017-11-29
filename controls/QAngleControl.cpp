//
// Created by martin on 19/11/17.
//

#include <cmath>
#include <iostream>
#include <QtWidgets/QGroupBox>
#include <QRegularExpression>
#include "QAngleControl.h"

using namespace calenhad::controls;
using namespace calenhad::qmodule;


QAngleControl::QAngleControl (const QString& caption, const AngleType& type, QWidget* parent) : QWidget (parent), _type (type), _validator (nullptr) {

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

    if (_type == AngleType::Latitude) {
        setValidator (new AcceptRange (-90.0, 90.0, -90.0, 90.0));
    }
    if (_type == AngleType::Longitude) {
        setValidator (new AcceptRange (-180.0, 180.0, -180.0, 180.0));
    }

}

QAngleControl::~QAngleControl () {
    if (_validator) { delete _validator; }
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
    int sign = (_decimalDegreesText -> text().endsWith ("W") || _decimalDegreesText -> text().endsWith ("S")) ? -1 : 1;
    QString s = _decimalDegreesText -> text();
    s = s.split ("°").first();
    double degrees = sign * s.toDouble (&ok);
    if (ok && degrees != _value) {
        setValue (degrees);
        std::cout << _decimalDegreesText -> text().toStdString() << " = " << _value << "\n";
    }
}

void QAngleControl::tradUpdated() {
    bool ok;
    int sign = (_tradDegreesText -> text().endsWith ("W") || _tradDegreesText -> text().endsWith ("S")) ? -1 : 1;
    QStringList tokens = _tradDegreesText -> text ().split ("°");
    int degrees = tokens.first().toInt (&ok);
    tokens = tokens.at (1).split ("\'");
    int minutes = tokens.first().toInt (&ok);
    QString s = tokens.at (1);
    s = s.split ("\"").first();

    double seconds = s.toDouble (&ok);
    double value = sign * (degrees + minutes / 60.0 + seconds / 3600.0);
    if (ok && value != _value) {
        setValue (value);
    }
}

void QAngleControl::setValue (const double& value) {
    if (value != _value) {
        if ((! _validator) || _validator -> isInValidSet (value)) {
            _value = value;
            std::cout << "Set value " << value << "\n";
            QString letter = "";
            if (_type == AngleType::Latitude) {
                letter = value > 0 ? "N" : "S";
            }
            if (_type == AngleType::Longitude) {
                letter = value > 0 ? "E" : "W";
            }
            int degrees = (int) std::floor (std::abs (_value));
            double minutes = (std::abs (_value) - degrees) * 60.0;
            int mins = (int) (value > 0 ? std::floor (minutes) : std::ceil (minutes));
            double seconds = (minutes - mins) * 60;
            QString tradText = QString::number (degrees) + "°" + QString::number (mins) + "\'" + QString::number (seconds) + "\"" + letter;
            QString decimalText = QString::number (_value) + letter + "°";
            _tradDegreesText->setText (tradText);
            _decimalDegreesText->setText (decimalText);
            setToolTip (QString::null);
            _statusLabel->setPixmap (_statusOrright);
            emit valueChanged (value);
        } else {
            setToolTip (_validator -> toString (value));
            _statusLabel->setPixmap (_statusGoosed);
        }
    }
}

void QAngleControl::toggleFormat() {
    _stack -> setCurrentIndex (_stack -> currentIndex() == 0 ? 1 : 0);
}

AngleType QAngleControl::angleType () {
    return _type;
}

void QAngleControl::setValidator (calenhad::qmodule::AcceptRange* validator) {
    _validator = validator;
}