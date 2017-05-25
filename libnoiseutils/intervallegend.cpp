#include <QtCore/QVector>
#include "intervallegend.h"

using namespace noise::utils;

IntervalLegend::IntervalLegend (const IntervalLegend& other) : Legend (other),
    _entries (other._entries) {
}


IntervalLegend::IntervalLegend (const QString& name) : Legend (name),
    _entries (std::map<double, QColor>()) {
    _entries [INT_MIN] = _defaultColor;
}

IntervalLegend::~IntervalLegend() {

}


QColor IntervalLegend::lookup (const double& value) {
    std::map<double, QColor>::iterator i = std::find_if_not (_entries.begin(), _entries.end(), [&value] (std::pair<double, QColor> entry) -> bool { return entry.first <= value; } );
    return (--i) -> second;
}


// Replace the entry if it exists, add it in otherwise.
void IntervalLegend::addEntry (const double& value, const QColor& colour) {
    _entries [value] = colour;

}

unsigned IntervalLegend::removeEntries (const double& from, const double& unto) {
    unsigned count = 0;
    std::map<double, QColor>::iterator i;
    for (i = _entries.begin(); i != _entries.end(); i++) {
        std::pair<int, QColor> entry = *i;
        if (entry.first > from && entry.first < unto) {
            _entries.erase (i);
            count++;
        }
    }
    return count;
}

bool IntervalLegend::isValid () {
    return _entries.size() >= 2;
}

QVector<QPair<qreal, QColor>> IntervalLegend::entries () {
    QVector<QPair<qreal, QColor>> entries;
    std::map<double, QColor>::iterator i;
    for (i = _entries.begin (); i != _entries.end (); i++) {
        std::pair<const double, QColor> entry = *i;
        entries.append (QPair<qreal, QColor> (entry.first, entry.second));
    }
    return entries;
}

int IntervalLegend::size () {
    return entries().size ();
}

LegendEntry IntervalLegend::at (int i) {
    return entries().at (i);
}

void IntervalLegend::clear() {
    _entries.clear ();
}

