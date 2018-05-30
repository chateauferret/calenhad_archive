//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomDocument>
#include <iostream>
#include <QtCore/QFile>
#include "CalenhadServices.h"
#include "pipeline/ModuleFactory.h"
#include "exprtk/Calculator.h"
#include "preferences/PreferencesService.h"
#include "messages/QNotificationHost.h"
#include "legend/LegendService.h"
#include "mapping/projection/ProjectionService.h"
#include "controls/globe/StatisticsService.h"
using namespace calenhad;
using namespace calenhad::preferences;
using namespace calenhad::notification;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::mapping::projection;

PreferencesService* CalenhadServices::_preferences;
QNotificationHost* CalenhadServices::_messages = nullptr;
ProjectionService* CalenhadServices::_projections;
LegendService* CalenhadServices::_legends;
StatisticsService* CalenhadServices::_statistics = new StatisticsService();
ModuleFactory* CalenhadServices::_modules;
Calculator* CalenhadServices::_calculator;

PreferencesService* CalenhadServices::preferences () {
    return _preferences;
}

QNotificationHost* CalenhadServices::messages() {
    return _messages;
}

LegendService* CalenhadServices::legends () {
    return _legends;
}

ProjectionService* CalenhadServices::projections() {
    return _projections;
}

StatisticsService* CalenhadServices::statistics() {
    return _statistics;
}

void CalenhadServices::providePreferences (PreferencesService* service) {
    _preferences = service;
}

void CalenhadServices::provideModules (ModuleFactory* modules) {
    _modules = modules;
}

void CalenhadServices::provideMessages (QNotificationHost* service) {
    _messages = service;
}

void CalenhadServices::provideLegends (LegendService* service) {
    _legends = service;
}


bool CalenhadServices::readXml (const QString& fname, QDomDocument& doc) {
    QNotificationHost* messages = CalenhadServices::messages();
    if (fname.isEmpty()) {
        if (_messages) {
            _messages->message ("File error", "Couldn't read file " + fname, NotificationStyle::ErrorNotification);
        } else {
            std::cout << "File error: Couldn't read file " << fname.toStdString() << "\n";
        }
    }
    QFile f (fname);
    f.open (QFile::ReadOnly);
    QString error;
    int errLine, errColumn;

    if (! doc.setContent (&f, false, &error, &errLine, &errColumn)) {
        if (_messages) {
            _messages -> message ("Parse error", "Couldn't parse " + fname + "\nError " + error + " at line " + QString::number (errLine) + " col " + QString::number (errColumn), NotificationStyle::ErrorNotification);
        }
        std::cout << "Couldn't parse " << fname.toStdString () << "\nError " << error.toStdString () << " at line " << errLine << " col " << errColumn << "\n";
        return false;
    } else {
        return true;
    }

}

void CalenhadServices::provideProjections (ProjectionService* service) {
    _projections = service;
}

ModuleFactory* CalenhadServices::modules () {
    return _modules;
}

void CalenhadServices::provideCalculator (Calculator* calculator) {
    _calculator = calculator;
}

Calculator* CalenhadServices::calculator () {
    return _calculator;
}
