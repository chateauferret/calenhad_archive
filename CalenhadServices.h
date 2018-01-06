//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_SERVICELOCATOR_H
#define CALENHAD_SERVICELOCATOR_H

#include <QString>
#include <QDomDocument>
namespace calenhad {
    namespace pipeline {
        class ModuleFactory;
    }
    namespace expressions {
        class VariablesService;
    }
    namespace preferences {
        class PreferencesService;
    }
    namespace notification {
        class QNotificationHost;
    }
    namespace legend {
        class LegendService;
    }
    namespace mapping {
        namespace projection {
            class ProjectionService;
        }
    }
    namespace controls {
        namespace globe {
            class StatisticsService;
        }
    }

    class CalenhadServices {

    public:
        static calenhad::preferences::PreferencesService* preferences ();

        static calenhad::notification::QNotificationHost* messages ();

        static calenhad::legend::LegendService* legends ();

        static calenhad::mapping::projection::ProjectionService* projections ();

        static calenhad::controls::globe::StatisticsService* statistics ();

        static calenhad::pipeline::ModuleFactory* modules ();

        static calenhad::expressions::VariablesService* calculator ();

        static void providePreferences (calenhad::preferences::PreferencesService* service);

        static void provideMessages (calenhad::notification::QNotificationHost* service);

        static void provideLegends (calenhad::legend::LegendService* service);

        static void provideProjections (calenhad::mapping::projection::ProjectionService* service);

        static bool readXml (const QString& fname, QDomDocument& doc);

        static void provideCalculator (calenhad::expressions::VariablesService* calculator);

    private:
        static calenhad::preferences::PreferencesService* _preferences;
        static calenhad::notification::QNotificationHost* _messages;
        static calenhad::legend::LegendService* _legends;
        static calenhad::mapping::projection::ProjectionService* _projections;
        static calenhad::controls::globe::StatisticsService* _statistics;
        static calenhad::pipeline::ModuleFactory* _modules;
        static calenhad::expressions::VariablesService* _calculator;

    };
}


#endif //CALENHAD_SERVICELOCATOR_H
