//
// Created by martin on 19/12/16.
//

#include "QClampModule.h"
#include "QRangeModule.h"


QRangeModule::QRangeModule (Module* module, QWidget* parent) : QModule (module, parent){

}

void QRangeModule::initialise() {
    QModule::initialise();
    lowerBoundSpin = QNode::noiseValueParamControl ("Lower bound");
    _contentLayout -> addRow (tr ("Lower bound"), lowerBoundSpin);
    connect (lowerBoundSpin, SIGNAL (valueChanged (double)), this, SLOT (setLowerBound (double)));

    upperBoundSpin = QNode::noiseValueParamControl ("Upper bound");
    _contentLayout -> addRow (tr ("Upper bound"), upperBoundSpin);
    connect (upperBoundSpin, SIGNAL (valueChanged (double)), this, SLOT (setUpperBound (double)));


}

void QRangeModule::setLowerBound (double lowerBound) {
    if (lowerBound < upperBound()) {
        lowerBoundSpin -> setValue (lowerBound);
        setBounds (lowerBound, upperBound());
        upperBoundSpin -> setMinimum (lowerBound + lowerBoundSpin -> singleStep());
        emit QNode::nodeChanged ("lowerBound", lowerBound);
    }

}

void QRangeModule::setUpperBound (double upperBound) {
    if (upperBound > lowerBound()) {
        upperBoundSpin -> setValue (upperBound);
        setBounds (lowerBound(), upperBound);
        lowerBoundSpin -> setMaximum (upperBound - upperBoundSpin -> singleStep());
        emit QNode::nodeChanged ("upperBound", upperBound);
    }
}

