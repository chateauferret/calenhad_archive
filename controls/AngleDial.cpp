//
// Created by martin on 26/04/17.
//

#include <QtGui/QDoubleValidator>
#include <QtWidgets/QDoubleSpinBox>
#include "AngleDial.h"
#include "qwt/qwt_round_scale_draw.h"
#include "qwt/qwt_dial_needle.h"
#include "qwt/qwt_dial.h"

AngleDial::AngleDial (QWidget* parent) : QWidget (parent), _value (0) {

}

AngleDial::~AngleDial() {

}

void AngleDial::initialise() {

    // editing box
    _angleEdit = new QDoubleSpinBox (this);
    _angleEdit -> setMinimum (-180.0);
    _angleEdit -> setMaximum (180.0);
    _angleEdit -> setSingleStep (0.1);
    _angleEdit -> setValue (_value);

    // dial button
    _dialButton = new QPushButton (this);
    _dialButton -> setText ("...");
    connect (_dialButton, &QPushButton::pressed, this, &AngleDial::requestDial);

    // dial
    QwtDial* spin = new QwtDial (this);
    spin -> setLowerBound (-180.0);
    spin -> setUpperBound (180.0);
    spin -> setMinimumSize (80, 80);

    QPalette palette0;
    for ( int c = 0; c < QPalette::NColorRoles; c++ ) {
        const QPalette::ColorRole colorRole = static_cast<QPalette::ColorRole>( c );
        palette0.setColor( colorRole, QColor() );
    }

    palette0.setColor( QPalette::Base, palette0.color ( QPalette::Base ) );
    palette0.setColor( QPalette::WindowText, palette0.color( QPalette::Base ) );
    spin->setLineWidth( 4 );
    spin->setFrameShadow (QwtDial::Sunken);
    palette0.setColor (QPalette::Base, Qt::darkBlue );
    palette0.setColor (QPalette::WindowText, QColor( Qt::darkBlue ).dark( 120 ) );
    palette0.setColor( QPalette::Text, Qt::white );
    spin -> setPalette (palette0);
    QwtRoundScaleDraw *scaleDraw = new QwtRoundScaleDraw();
    spin->setScaleMaxMajor (12);
    spin->setScaleMaxMinor (5);
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Ticks, true );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Labels, true );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, true );
    scaleDraw->setTickLength( QwtScaleDiv::MinorTick, 1 );
    scaleDraw->setTickLength( QwtScaleDiv::MediumTick, 3 );
    scaleDraw->setTickLength( QwtScaleDiv::MajorTick, 5 );
    scaleDraw -> setSpacing (2);
    scaleDraw -> setRadius (360.0);
    spin -> setMouseTracking (false);
    spin -> setScaleDraw (scaleDraw);
    _dial = spin;
}

void AngleDial::setValue (const double& value) {
    _value = value;
    _dial -> setValue (value);
    _angleEdit -> setValue (value);
}

void AngleDial::requestDial () {

}
