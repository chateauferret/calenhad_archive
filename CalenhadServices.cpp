//
// Created by martin on 15/04/17.
//
#include "s2/s2point_index.h"
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
using namespace calenhad;
using namespace calenhad::preferences;
using namespace calenhad::notification;
using namespace calenhad::controls::globe;
using namespace calenhad::legend;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::mapping::projection;
using namespace calenhad::icosphere;



PreferencesService* CalenhadServices::_preferences;
QNotificationHost* CalenhadServices::_messages = nullptr;
ProjectionService* CalenhadServices::_projections;
LegendService* CalenhadServices::_legends;
StatisticsService* CalenhadServices::_statistics = new StatisticsService();
ModuleFactory* CalenhadServices::_modules;
Calculator* CalenhadServices::_calculator;
Icosphere* CalenhadServices::_icosphere = nullptr;
S2PointIndex<float>* CalenhadServices::_pointIndex = nullptr;
CubicSphere* CalenhadServices::_cubicSphere = nullptr;

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
        std::cout << "Built icosphere with " << _icosphere -> vertexCount () << " vertices in " << end - start << " seconds" << "\n";
}

calenhad::icosphere::Icosphere* CalenhadServices::icosphere () {
    return _icosphere;
}

void CalenhadServices::providePointIndex (int depth) {
    delete _pointIndex;
    double start = clock () / static_cast<double> (CLOCKS_PER_SEC);
    _pointIndex = new S2PointIndex<float>();

    // create initial point on the sphere
    const double gr = (sqrt(5.0) + 1.0) / 2.0;    // golden ratio = 1.6180339887498948482
    const double ga = (2.0 - gr) * (2.0 * M_PI);        // golden angle = 2.39996322972865332
    double lat, lon;
    size_t num_points = 2000000;
    for (size_t i = 1; i <= num_points; ++i) {
        lat = asin (-1.0 + 2.0 * double(i) / (num_points + 1));
        lon = ga * i;

        S2Point point;
        point.Set (cos (lon) * cos (lat), sin (lon) * cos (lat), sin (lat));
        _pointIndex -> Add (point, 0.0f);

    }


    double end = clock () / static_cast<double> (CLOCKS_PER_SEC);
    std::cout << "Built point index with " << num_points << " points in " << end - start << " seconds" << "\n";
}

S2PointIndex<float>* CalenhadServices::pointIndex () {
    return _pointIndex;
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