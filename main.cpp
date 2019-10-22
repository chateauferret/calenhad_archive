#include <iostream>
#include <QApplication>
#include <QtCore/QFile>
#include "nodeedit/Calenhad.h"
#include "pipeline/CalenhadModel.h"
#include "icosphere/icosphere.h"
#include "exprtk/Calculator.h"
#include "messages/QNotificationHost.h"
#include "nodeedit/Toolbox.h"
#include "preferences/preferences.h"
#include "controls/altitudemap/AltitudeMapPlot.h"
#include "CalenhadServices.h"
#include "legend/LegendRoster.h"
#include "mapping/projection/ProjectionService.h"
#include "pipeline/ModuleFactory.h"


#include "icosphere/icosphere.h"


#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace calenhad;
using namespace calenhad::nodeedit;
using namespace calenhad::preferences;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::controls::altitudemap;
using namespace calenhad::legend;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::notification;
using namespace calenhad::mapping::projection;

Q_DECLARE_METATYPE (std::shared_ptr<QImage>)
Q_DECLARE_METATYPE (std::shared_ptr<calenhad::icosphere::Icosphere>)
Q_DECLARE_METATYPE (calenhad::controls::altitudemap::CurveType)
Q_DECLARE_METATYPE (calenhad::nodeedit::CalenhadAction)
Q_DECLARE_METATYPE (Qt::PenStyle)


int main (int argc, char **argv) {

    clock_t start = clock ();

    // Register required metatypes for Qt services
    qRegisterMetaType<QImage>();
    qRegisterMetaType<std::shared_ptr<QImage>>();
    qRegisterMetaType<std::shared_ptr<calenhad::icosphere::Icosphere>>();
    qRegisterMetaType<CurveType>();
    qRegisterMetaType<CalenhadAction>();
    qRegisterMetaType<Qt::PenStyle>();

    // set up application
    QCoreApplication::setOrganizationName ("calenhad");
    QCoreApplication::setOrganizationDomain ("chateauferret.com");
    QCoreApplication::setApplicationName ("calenhad");
    QApplication app (argc, argv);

    // i18n
    QTranslator translator;
    translator.load ("calenhad_en");
    app.installTranslator (&translator);

    // set up services
    // Preferences service
    Preferences* preferences = new Preferences();
    preferences -> loadSettings();
    CalenhadServices::providePreferences (preferences);

    // Modules service
    ModuleFactory* modules = new ModuleFactory();
    CalenhadServices::provideModules (modules);

    // Legends service
    LegendRoster* roster = new LegendRoster();
    CalenhadServices::provideLegends (roster);

    // Projections service
    ProjectionService* projections = new ProjectionService();
    CalenhadServices::provideProjections (projections);

    // Calculator service
    Calculator* calculator = new Calculator();
    CalenhadServices::provideCalculator (calculator);

    // Icosphere service
    //CalenhadServices::provideIcosphere (12);
    CalenhadServices::provideCubicSphere (11);

    // Calenhad model - the arrangement of modules and connections between them
    Calenhad* window = new Calenhad();
    window -> setStyleSheet (Preferences::styleSheet());
    window -> show();



    return QApplication::exec();
}
