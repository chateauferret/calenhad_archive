//
// Created by martin on 05/02/18.
//

#include <CalenhadServices.h>
#include "exprtk/Calculator.h"
#include "AltitudeMapping.h"
using namespace calenhad::controls::altitudemap;


AltitudeMapping::AltitudeMapping () : AltitudeMapping (QPointF()) {

}

AltitudeMapping::AltitudeMapping (const QPointF& point) : _x (QString::number (point.x())), _y (QString::number (point.y())) {

}

AltitudeMapping::AltitudeMapping (const QString& x, const QString& y) : _x (x), _y (y) {

}


AltitudeMapping::AltitudeMapping (const double& x, const double& y) : AltitudeMapping (QPointF (x, y)) {

}

AltitudeMapping::~AltitudeMapping () {

}

void AltitudeMapping::setPoint (const double& x, const double& y) {
    setExpressionX (QString::number (x));
    setExpressionY (QString::number (y));
}

void AltitudeMapping::setPoint (const QString& x, const QString& y) {
    setExpressionX (x);
    setExpressionY (y);
}

void AltitudeMapping::setPoint (const QPointF& point) {
    setPoint (point.x(), point.y());
}

QPointF AltitudeMapping::point() const {
    return QPointF (x(), y());
}

double AltitudeMapping::x() const {
    return CalenhadServices::calculator() -> compute (_x);
}

double AltitudeMapping::y() const {
    return CalenhadServices::calculator() -> compute (_y);
}

QString AltitudeMapping::expressionX() const {
    return _x;
}


QString AltitudeMapping::expressionY() const {
    return _y;
}

void AltitudeMapping::setExpressionX (const QString& x) {
    _x = x;
}

void AltitudeMapping::setExpressionY (const QString& y) {
    _y = y;
}

bool AltitudeMapping::isComputed()const {
    bool ok;
    _x.toDouble (&ok);
    if (ok) { _y.toDouble (&ok); }

    return ! ok;
}

bool AltitudeMapping::isValid () const {
    CalenhadServices::calculator() -> compute (_x);
    CalenhadServices::calculator() -> compute (_y);
    return ! CalenhadServices::calculator() -> hasErrors();
}
