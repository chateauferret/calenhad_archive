#include "legend.h"
#include <algorithm>

using namespace noise::utils;

Legend::Legend() :
    _defaultColor (QColor (0, 0, 0, 0)) {

}

Legend::~Legend() {

}

Legend::Legend (const Legend& other) :
    _defaultColor (other._defaultColor) {}


void Legend::setDefaultColor (const QColor& colour) {
    _defaultColor = colour;
}




