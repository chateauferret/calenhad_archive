//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomNodeList>
#include <iostream>
#include "LegendRoster.h"
#include "CalenhadServices.h"

LegendRoster::LegendRoster () {

}

LegendRoster::~LegendRoster () {

}

Legend* LegendRoster::find (const QString& name) {
    return _legends.find (name).value ();
}

bool LegendRoster::exists (const QString& name) {
    return  _legends.keys().contains (name);
}

void LegendRoster::provide (const QString& name, Legend* legend) {
    _legends.insert (name, legend);
}

void LegendRoster::dispose (const QString& name) {
    QMap<QString, Legend*>::iterator i = _legends.find (name);
    if (i != _legends.end()) {
        Legend* legend = i.value();
        _legends.remove (name);
        delete legend;
    }
}

void LegendRoster::provideFromXml (const QString& fname) {
    QDomDocument doc;
    if (CalenhadServices::readXml (fname, doc)) {
        QDomNodeList legendNodes = doc.documentElement ().elementsByTagName ("legend");
        for (int i = 0; i < legendNodes.size (); i++) {
            Legend* legend = Legend::fromNode (legendNodes.item (i));
            std::cout << "Found legend " << legend -> name ().toStdString () << "\n";
            provide (legend->name (), legend);
        }
    }
}

QMap<QString, Legend*> LegendRoster::all () {
    return _legends;
}

void LegendRoster::rename (const QString& from, const QString& to) {
    Legend* legend = find (from);
    _legends.remove (from);
    _legends.insert (to, legend);
}