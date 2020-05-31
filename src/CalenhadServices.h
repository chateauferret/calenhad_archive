//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_SERVICELOCATOR_H
#define CALENHAD_SERVICELOCATOR_H

#include <QString>
#include <QDomDocument>
#include <src/graph/ComputeService.h>
#include "icosphere/CubicSphere.h"

namespace calenhad {
    namespace pipeline {
        class ModuleFactory;
    }
    namespace expressions {
        class Calculator;
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

    namespace grid {
        class Icosphere;
    }

    namespace controls {
        namespace globe {
            class StatisticsService;
        }
    }

    class CalenhadServices {

    public:
        static calenhad::preferences::PreferencesService* preferences();
        static calenhad::notification::QNotificationHost* messages();
        static calenhad::legend::LegendService* legends();
        static calenhad::mapping::projection::ProjectionService* projections();
        static calenhad::controls::globe::StatisticsService* statistics();
        static calenhad::pipeline::ModuleFactory* modules();
        static calenhad::expressions::Calculator* calculator();

        static void providePreferences (calenhad::preferences::PreferencesService* service);
        static void provideMessages (calenhad::notification::QNotificationHost* service);
        static void provideLegends (calenhad::legend::LegendService* service);
        static void provideProjections (calenhad::mapping::projection::ProjectionService* service);

        static bool readXml (const QString& fname, QDomDocument& doc);
        static void provideCalculator (calenhad::expressions::Calculator* calculator);
        static void provideModules (pipeline::ModuleFactory* modules);

        static void provideIcosphere (int depth);
        static void provideComputeService ();
        static graph::ComputeService* compute ();
        static void provideCubicSphere (const int& depth);
        static calenhad::grid::Icosphere* icosphere();
        static grid::CubicSphere* cubicSphere ();

    private:
        static calenhad::preferences::PreferencesService* _preferences;
        static calenhad::notification::QNotificationHost* _messages;
        static calenhad::legend::LegendService* _legends;
        static calenhad::mapping::projection::ProjectionService* _projections;
        static calenhad::controls::globe::StatisticsService* _statistics;
        static calenhad::pipeline::ModuleFactory* _modules;
        static calenhad::expressions::Calculator* _calculator;
        static calenhad::grid::Icosphere* _icosphere;
        static calenhad::grid::CubicSphere* _cubicSphere;
        static calenhad::graph::ComputeService* _computeService;



    };
}


#endif //CALENHAD_SERVICELOCATOR_H
