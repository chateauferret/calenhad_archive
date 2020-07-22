#include <iostream>
#include <QApplication>
#include <QtCore/QFile>
#include <src/controls/globe/CalenhadGlobeWidget.h>

#include "nodeedit/Calenhad.h"
#include "pipeline/CalenhadModel.h"
#include "grid/icosphere.h"
#include "exprtk/Calculator.h"
#include "messages/QNotificationHost.h"
#include "preferences/preferences.h"
#include "controls/altitudemap/AltitudeMapPlot.h"
#include "CalenhadServices.h"
#include "legend/LegendRoster.h"
#include "mapping/projection/ProjectionService.h"
#include "pipeline/ModuleFactory.h"


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
Q_DECLARE_METATYPE (std::shared_ptr<calenhad::grid::Icosphere>)
Q_DECLARE_METATYPE (calenhad::controls::altitudemap::CurveType)
Q_DECLARE_METATYPE (Qt::PenStyle)


int main (int argc, char **argv) {

    clock_t start = clock ();

    // Register required metatypes for Qt services
    qRegisterMetaType<QImage>();
    qRegisterMetaType<std::shared_ptr<QImage>>();
    qRegisterMetaType<std::shared_ptr<calenhad::grid::Icosphere>>();
    qRegisterMetaType<CurveType>();
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

    // Make stylesheet available to the application
    QString fileName = preferences -> calenhad_stylesheet;
    QFile file (fileName);
    bool f = file.open (QFile::ReadOnly);
    QString styleSheet = "bollocks";
    if (f) {
        styleSheet = QLatin1String (file.readAll());
    } else {
        QFile defaultFile (":/darkorange.css");
        fileName = defaultFile.fileName();
        f = defaultFile.open (QFile::ReadOnly);
        styleSheet = QLatin1String (defaultFile.readAll());
    };

    app.setStyleSheet (styleSheet);

    // Legends service
    LegendRoster* roster = new LegendRoster();
    CalenhadServices::provideLegends (roster);

    // Calculator service
    Calculator* calculator = new Calculator();
    CalenhadServices::provideCalculator (calculator);

    // Modules service
    ModuleFactory* modules = new ModuleFactory();
    CalenhadServices::provideModules (modules);

    // Projections service
    ProjectionService* projections = new ProjectionService();
    CalenhadServices::provideProjections (projections);

    // Icosphere service
    // CalenhadServices::provideIcosphere (12);
    // CalenhadServices::provideCubicSphere (11);
    CalenhadServices::provideComputeService();

    // Calenhad model - the arrangement of modules and connections between them
    Calenhad window;
    CalenhadServices::provideMainWindow (&window);
    window.showSplash();
    window.show();

    return QApplication::exec();
}
