//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_SERVICELOCATOR_H
#define CALENHAD_SERVICELOCATOR_H


#include <pipeline/ModuleFactory.h>
#include "preferences.h"
#include "messages/QNotificationStack.h"
#include "LegendService.h"
#include "ProjectionService.h"
#include "StatisticsService.h"

class QDomDocument;

class CalenhadServices {

public:
    static PreferencesService* preferences();
    static QNotificationService* messages();
    static LegendService* legends();
    static ProjectionService* projections();
    static StatisticsService* statistics();
    static ModuleFactory* modules();

    static void providePreferences (PreferencesService* service);
    static void provideMessages (QNotificationService* service);
    static void provideLegends (LegendService* service);
    static void provideProjections (ProjectionService* service);
    static bool readXml (const QString& fname, QDomDocument& doc);

private:
    static PreferencesService* _preferences;
    static QNotificationService* _messages;
    static LegendService* _legends;
    static ProjectionService* _projections;
    static StatisticsService* _statistics;
    static ModuleFactory* _modules;


};


#endif //CALENHAD_SERVICELOCATOR_H
