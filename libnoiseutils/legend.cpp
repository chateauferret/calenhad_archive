#include "legend.h"
#include <algorithm>

using namespace noise::utils;

Legend::Legend() :
    noValueColour (QColor (0, 0, 0, 0)) {

}

Legend::~Legend() {

}

Legend::Legend (const Legend& other) :
    noValueColour (other.noValueColour) {}


void Legend::setNoValueColour (const QColor& colour) {
    noValueColour = colour;
}




