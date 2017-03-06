#include "legend.h"

using namespace icosphere;

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

QColor Legend::lookup (const std::experimental::optional<double>& value) {
    if ((bool) value) {
        return lookup (value.value ());
    } else {
        return _defaultColor;
    }
}




