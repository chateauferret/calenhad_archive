//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomDocument>
#include <iostream>
#include <QtCore/QFile>
#include "CalenhadServices.h"
#include "pipeline/ModuleFactory.h"
#include "exprtk/VariablesService.h"
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
QNotificationHost* CalenhadServices::_messages;
ProjectionService* CalenhadServices::_projections;
LegendService* CalenhadServices::_legends;
StatisticsService* CalenhadServices::_statistics = new StatisticsService();
ModuleFactory* CalenhadServices::_modules;
VariablesService* CalenhadServices::_calculator;

PreferencesService* CalenhadServices::preferences () {
    return _preferences;
}

QNotificationHost* CalenhadServices::messages () {
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
    _modules = new ModuleFactory();
}

void CalenhadServices::provideMessages (QNotificationHost* service) {
    _messages = service;
}

void CalenhadServices::provideLegends (LegendService* service) {
    _legends = service;
}


bool CalenhadServices::readXml (const QString& fname, QDomDocument& doc) {
    std::cout << "Opening file " << fname.toStdString() << "\n";
    if (fname.isEmpty()) {
        CalenhadServices::messages() -> message ("error", "Couldn't read file " + fname);
    }
    QFile f (fname);
    f.open (QFile::ReadOnly);
    QString error;
    int errLine, errColumn;

    if (! doc.setContent (&f, false, &error, &errLine, &errColumn)) {
        // CalenhadServices::messages() -> message ("error", "Couldn't read file " + fname + "\nError " + error + " at line " + QString::number (errLine) + " col " + QString::number (errColumn) + "\n");
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

void CalenhadServices::provideCalculator (VariablesService* calculator) {
    _calculator = calculator;
}

VariablesService* CalenhadServices::calculator () {
    return _calculator;
}
