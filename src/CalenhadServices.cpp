//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomDocument>
#include <iostream>
#include <QtCore/QFile>
#include <grid/CubicSphere.h>
#include "CalenhadServices.h"
#include "pipeline/ModuleFactory.h"
#include "exprtk/Calculator.h"
#include "preferences/PreferencesService.h"
#include "messages/QNotificationHost.h"
#include "legend/LegendService.h"
#include "mapping/projection/ProjectionService.h"
#include "controls/globe/CalenhadStatistics.h"
#include "grid/icosphere.h"
#include "graph/ComputeService.h"
#include "module/Module.h"

using namespace calenhad;
using namespace calenhad::preferences;
using namespace calenhad::notification;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::mapping::projection;
using namespace calenhad::grid;
using namespace calenhad::graph;
using namespace calenhad::module;
using namespace calenhad::nodeedit;



PreferencesService* CalenhadServices::_preferences;
QNotificationHost* CalenhadServices::_messages = nullptr;
ProjectionService* CalenhadServices::_projections;
LegendService* CalenhadServices::_legends;
ModuleFactory* CalenhadServices::_modules;
Calculator* CalenhadServices::_calculator;
CubicSphere* CalenhadServices::_cubicSphere = nullptr;
ComputeService* CalenhadServices::_computeService = nullptr;
CalenhadGlobeDialog* CalenhadServices::_globeDialog = nullptr;
calenhad::controls::globe::CalenhadGlobeDialog* CalenhadServices::_mainWindow = nullptr;


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


void CalenhadServices::provideComputeService() {
    delete _computeService;
    _computeService = new ComputeService();
}

ComputeService* CalenhadServices::compute() {
    return _computeService;
}


QStringList CalenhadServices::recentFiles() {
    QFile f (CalenhadServices::preferences() -> calenhad_recentfiles_filename);
    QStringList files;
    if (f.exists()) {
        if (f.open (QIODevice::ReadOnly)) {
            while (! f.atEnd ()) {
                QByteArray line = f.readLine().trimmed();
                if (QFile::exists (line)) {
                    files.append (line);
                }
            }
            f.close();
        }
    }
    return files;
}

void CalenhadServices::provideGlobe (CalenhadGlobeDialog* globe) {
    _globeDialog = globe;
    provideGlobe();
}

void CalenhadServices::provideGlobe() {
    if (_globeDialog) {
        _globeDialog -> widget() -> updateModules();
    }
}

void CalenhadServices::disposeGlobe() {
    if (_globeDialog) {
        _globeDialog -> close();
        delete _globeDialog;
        _globeDialog = nullptr;
    }
}

CalenhadGlobeDialog* CalenhadServices::globe() {
    return _globeDialog;
}

CalenhadGlobeDialog* CalenhadServices::globe (CalenhadModel* model) {
    if (model) {
        _globeDialog -> selectModule (nullptr);
        _globeDialog -> setModel (model);
    }
    return _globeDialog;
}

CalenhadGlobeDialog* CalenhadServices::globe (Module* module) {
    if (module) {
        CalenhadModel* model = module -> model();
        if (model) {
            _globeDialog -> setModel (model);
            _globeDialog -> selectModule(module);
        }
    }
    return _globeDialog;
}



CalenhadGlobeDialog* CalenhadServices::mainWindow() {
    return _mainWindow;
}

void CalenhadServices::provideMainWindow (CalenhadGlobeDialog* mainWindow) {
    _mainWindow = mainWindow;
}


