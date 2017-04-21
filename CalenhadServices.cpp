//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomDocument>
#include <iostream>
#include <QtCore/QFile>
#include "CalenhadServices.h"
#include "LegendService.h"


PreferencesService* CalenhadServices::_preferences;
MessageService* CalenhadServices::_messages;
LegendService* CalenhadServices::_legends;

PreferencesService* CalenhadServices::preferences () {
    return _preferences;
}

MessageService* CalenhadServices::messages () {
    return _messages;
}

LegendService* CalenhadServices::legends () {
    return _legends;
}

void CalenhadServices::providePreferences (PreferencesService* service) {
    _preferences = service;
}

void CalenhadServices::provideMessages (MessageService* service) {
    _messages = service;
}

void CalenhadServices::provideLegends (LegendService* service) {
    _legends = service;
}


bool CalenhadServices::readXml (const QString& fname, QDomDocument& doc) {
    std::cout << "Opening file " << fname.toStdString() << "\n";
    if (fname.isEmpty()) {
        CalenhadServices::messages() -> message ("Couldn't read file " + fname, "File not found");
    }
    QFile f (fname);
    f.open (QFile::ReadOnly);
    QString error;
    int errLine, errColumn;

    if (! doc.setContent (&f, false, &error, &errLine, &errColumn)) {
        CalenhadServices::messages() -> message ("Couldn't read file " + fname, "Error " + error + " at line " + QString::number (errLine) + " col " + QString::number (errColumn) + "\n");
        return false;
    } else {
        return true;
    }

}