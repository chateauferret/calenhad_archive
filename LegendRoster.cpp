//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomNodeList>
#include <iostream>
#include <QtCore/QFile>
#include "LegendRoster.h"
#include "CalenhadServices.h"
#include "mapping/Legend.h"

QString LegendRoster::_filename;

LegendRoster::LegendRoster () : _dirty (false) {
}

LegendRoster::~LegendRoster() {

}

Legend* LegendRoster::find (const QString& name) {
    return _legends.find (name).value ();
}

bool LegendRoster::exists (const QString& name) {
    return  ! (_legends.find (name) == _legends.end());
}

void LegendRoster::provide (Legend* legend) {
    _legends.insert (legend -> name(), legend);
    _dirty = true;
}

void LegendRoster::inflate (const QString& filename) {
    _filename = filename;
    QDomDocument doc;
    if (CalenhadServices::readXml (_filename, doc)) {
        QDomNodeList legendNodes = doc.documentElement ().elementsByTagName ("legend");
        for (int i = 0; i < legendNodes.size (); i++) {
            Legend* legend = new Legend();
            legend -> inflate (legendNodes.item (i));
            provide (legend);
        }
        _dirty = false;
    }

}

QMap<QString, Legend*> LegendRoster::all () {
    return _legends;
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

void LegendRoster::commit () {
    serialise (_filename);
}

void LegendRoster::rollback () {
    _legends.clear();
    inflate (_filename);
}

void LegendRoster::serialise (QString filename) {

    QFile file (filename);
    QTextStream ds (&file);


    QDomDocument doc;
    QDomElement root = doc.createElement ("legends");
    doc.appendChild (root);

    for (Legend* legend : _legends) {
        legend -> serialise (doc);
    }

    std::cout.flush();
    if (! file.open( QIODevice::WriteOnly | QIODevice::Text )) {
        CalenhadServices::messages() -> message ("error", "Failed to open file for writing");
        return;
    }

    ds << doc.toString();

    file.close();
    QMessageService* service = CalenhadServices::messages();
    CalenhadServices::messages() -> message ("info", "Wrote file " + filename);
    _filename = filename;
    _dirty = false;

}

Legend* LegendRoster::defaultLegend() {
  //  if (exists ("default")) {
  //      return find ("default)");
  //  }

    if (legendCount() > 0) {
        return _legends.first();
    }

    // if there are no legends at all, make one up and add it to the roster
    Legend* legend = new Legend();
    legend -> addEntry (-1.0, Qt::black);
    legend -> addEntry (1.0, Qt::white);
    legend -> setName ("default");
    legend -> setNotes ("This legend was added to the roster because no legends were found in the legend file " + _filename + ".");
    provide (legend);
    return legend;
}

bool LegendRoster::isDirty() {
    return _dirty;
}

void LegendRoster::setDirty (const bool& dirty) {
    _dirty = dirty;
}