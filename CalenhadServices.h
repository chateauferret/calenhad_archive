//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_SERVICELOCATOR_H
#define CALENHAD_SERVICELOCATOR_H


#include "preferences.h"
#include "messagefactory.h"
#include "LegendService.h"
#include "ProjectionService.h"

class QDomDocument;

class CalenhadServices {

public:
    static PreferencesService* preferences();
    static MessageService* messages();
    static LegendService* legends();
    static ProjectionService* projections();

    static void providePreferences (PreferencesService* service);
    static void provideMessages (MessageService* service);
    static void provideLegends (LegendService* service);
    static void provideProjections (ProjectionService* service);

    static bool readXml (const QString& fname, QDomDocument& doc);

private:
    static PreferencesService* _preferences;
    static MessageService* _messages;
    static LegendService* _legends;
    static ProjectionService* _projections;


};


#endif //CALENHAD_SERVICELOCATOR_H
