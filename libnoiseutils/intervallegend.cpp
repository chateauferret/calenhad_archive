#include "intervallegend.h"

using namespace noise::utils;

IntervalLegend::IntervalLegend (const IntervalLegend& other) : Legend (other),
    intervals (other.intervals) {
}


IntervalLegend::IntervalLegend (const QString& name) : Legend (name),
    intervals (std::map<double, QColor>()) {
    intervals [INT_MIN] = _defaultColor;
}

IntervalLegend::~IntervalLegend() {

}


QColor IntervalLegend::lookup (const double& value) {
    std::map<double, QColor>::iterator i = std::find_if_not (intervals.begin(), intervals.end(), [&value] (std::pair<double, QColor> entry) -> bool { return entry.first <= value; } );
    return (--i) -> second;
}


// Replace the entry if it exists, add it in otherwise.
void IntervalLegend::addEntry (const double& value, const QColor& colour) {
    intervals [value] = colour;

}

unsigned IntervalLegend::removeEntries (const double& from, const double& unto) {
    unsigned count = 0;
    std::map<double, QColor>::iterator i;
    for (i = intervals.begin(); i != intervals.end(); i++) {
        std::pair<int, QColor> entry = *i;
        if (entry.first > from && entry.first < unto) {
            intervals.erase (i);
            count++;
        }
    }
    return count;
}

bool IntervalLegend::isValid () {
    return intervals.size() >= 2;
}

