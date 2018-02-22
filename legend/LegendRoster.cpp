//
// Created by martin on 15/04/17.
//

#include <QDomNodeList>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QtWidgets/QFileDialog>
#include "LegendRoster.h"
#include "CalenhadServices.h"
#include "Legend.h"
#include "../messages/QNotificationHost.h"

using namespace calenhad;
using namespace calenhad::notification;
using namespace calenhad::legend;

LegendRoster::LegendRoster () : _dirty (false), _lastUsed (nullptr) {
}

LegendRoster::~LegendRoster() {

}

Legend* LegendRoster::find (const QString& name) {
    if (exists (name)) {
        _lastUsed =  _legends.find (name).value ();
    } else {
        _lastUsed = defaultLegend();
    }
    if (_lastUsed -> name().isEmpty ()) {
        std::cout << "Found legend with no name\n";
    }
    return _lastUsed;
}

bool LegendRoster::exists (const QString& name) {
    return  ! (_legends.find (name) == _legends.end());
}

void LegendRoster::provide (Legend* legend) {
    if (legend -> name().isEmpty ()) {
        std::cout << "No legend name\n";
    }
    _legends.insert (legend -> name(), legend);
    _dirty = true;
}

QList <Legend*> LegendRoster::all () {
    return _legends.values ();
}

void LegendRoster::rename (const QString& from, const QString& to) {
    Legend* legend = find (from);
    _legends.remove (from);
    _legends.insert (to, legend);
    _dirty = true;
}

int LegendRoster::legendCount () {
    return _legends.count();
}

bool LegendRoster::remove (const QString& name) {
    if (legendCount() > 1) {
        Legend* legend = find (name);
        if (_legends.remove (name) == 0) {
            _dirty = true;
            return true;
        }
    }
    return false;
}

Legend* LegendRoster::defaultLegend() {
    if (exists ("default")) {
        return find ("default");
    }

    if (legendCount() > 0) {
        return _legends.first();
    }

    // if there are no legends at all, make one up and add it to the roster
    Legend* legend = new Legend();
    _legends.insert ("default", legend);
    legend -> addEntry (LegendEntry ("-1.0", Qt::black));
    legend -> addEntry (LegendEntry ("1.0", Qt::white));
    legend -> setName ("default");
    provide (legend);
    return legend;
}

bool LegendRoster::isDirty() {
    return _dirty;
}

void LegendRoster::setDirty (const bool& dirty) {
    _dirty = dirty;
}

void LegendRoster::commit () {
    if (isDirty()) {
        emit commitRequested();
    }
}

void LegendRoster::rollback () {
    if (isDirty()) {
        emit rollbackRequested();
    }
}

void LegendRoster::clear () {
    _legends.clear();
}

Legend* LegendRoster::lastUsed () {
    return _lastUsed;
}
