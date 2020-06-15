//
// Created by martin on 15/04/17.
//

#include <QtXml/QDomDocument>
#include <iostream>
#include <QtCore/QFile>
#include <icosphere/CubicSphere.h>
#include "CalenhadServices.h"
#include "pipeline/ModuleFactory.h"
#include "exprtk/Calculator.h"
#include "preferences/PreferencesService.h"
#include "messages/QNotificationHost.h"
#include "legend/LegendService.h"
#include "mapping/projection/ProjectionService.h"
#include "controls/globe/StatisticsService.h"
#include "icosphere/icosphere.h"
#include "graph/ComputeService.h"
#include "../controls/SplashDialog.h"

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



PreferencesService* CalenhadServices::_preferences;
QNotificationHost* CalenhadServices::_messages = nullptr;
ProjectionService* CalenhadServices::_projections;
LegendService* CalenhadServices::_legends;
StatisticsService* CalenhadServices::_statistics = new StatisticsService();
ModuleFactory* CalenhadServices::_modules;
Calculator* CalenhadServices::_calculator;
Icosphere* CalenhadServices::_icosphere = nullptr;
CubicSphere* CalenhadServices::_cubicSphere = nullptr;
ComputeService* CalenhadServices::_computeService = nullptr;


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

void CalenhadServices::provideIcosphere (int depth) {
    delete _icosphere;

        double start = clock () / static_cast<double> (CLOCKS_PER_SEC);
        _icosphere = new Icosphere (depth);
        double end = clock () / static_cast<double> (CLOCKS_PER_SEC);
        std::cout << "Built grid with " << _icosphere -> vertexCount () << " vertices in " << end - start << " seconds" << "\n";
}

calenhad::grid::Icosphere* CalenhadServices::icosphere () {
    return _icosphere;
}

void CalenhadServices::provideCubicSphere (const int& depth) {
    delete _cubicSphere;
    double start = clock () / static_cast<double> (CLOCKS_PER_SEC);
    _cubicSphere = new CubicSphere (depth);
    double end = clock () / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built cubic sphere with " << _cubicSphere -> count()  << " vertices in " << end - start << " seconds" << "\n";

}

CubicSphere* CalenhadServices::cubicSphere() {
    return _cubicSphere;
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


