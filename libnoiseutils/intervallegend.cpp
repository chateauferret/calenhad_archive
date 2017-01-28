#include "intervallegend.h"

using namespace noise::utils;

IntervalLegend::IntervalLegend (const IntervalLegend& other) : Legend (other),
    intervals (other.intervals) {
}


IntervalLegend::IntervalLegend() : Legend(),
    intervals (std::map<int, QColor>()) {
    intervals [INT_MIN] = noValueColour;
}

IntervalLegend::~IntervalLegend() {

}


QColor IntervalLegend::lookup (const int& value) {
    std::map<int, QColor>::iterator i = std::find_if_not (intervals.begin(), intervals.end(), [&value] (std::pair<int, QColor> entry) -> bool { return entry.first <= value; } );
    return (--i) -> second;
}


// Replace the entry if it exists, add it in otherwise.
void IntervalLegend::addEntry (const int& value, const QColor& colour) {
    intervals [value] = colour;

}

unsigned IntervalLegend::removeEntries (const int& from, const int& unto) {
    unsigned count = 0;
    std::map<int, QColor>::iterator i;
    for (i = intervals.begin(); i != intervals.end(); i++) {
        std::pair<int, QColor> entry = *i;
        if (entry.first > from && entry.first < unto) {
            intervals.erase (i);
            count++;
        }
    }
    return count;
}

