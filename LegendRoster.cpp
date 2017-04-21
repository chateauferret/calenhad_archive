//
// Created by martin on 15/04/17.
//

#include "LegendRoster.h"
#include "CalenhadServices.h"

using namespace icosphere;

LegendRoster::LegendRoster () {

}

LegendRoster::~LegendRoster () {

}

icosphere::Legend* LegendRoster::find (const QString& name) {
    return _legends.find (name).value ();
}

void LegendRoster::provide (const QString& name, icosphere::Legend* legend) {
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
            _legends.insert (legend->name (), legend);
        }
    }
}