#include <iostream>
#include <QApplication>
#include <QtCore/QFile>
#include "nodeedit/Calenhad.h"
#include "pipeline/CalenhadModel.h"
#include "icosphere/icosphere.h"
#include "exprtk/Calculator.h"
#include "messages/QNotificationHost.h"
#include "nodeedit/qnetoolbox.h"
#include "preferences/preferences.h"
#include "controls/altitudemap/AltitudeMapPlot.h"
#include "CalenhadServices.h"
#include "legend/LegendRoster.h"
#include "mapping/projection/ProjectionService.h"

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
Q_DECLARE_METATYPE (std::shared_ptr<icosphere::Icosphere>)
Q_DECLARE_METATYPE (calenhad::controls::altitudemap::CurveType)
Q_DECLARE_METATYPE (calenhad::nodeedit::CalenhadAction)
Q_DECLARE_METATYPE (Qt::PenStyle)


int main (int argc, char **argv) {


    // Register required metatypes for Qt services
    qRegisterMetaType<QImage>();
    qRegisterMetaType<std::shared_ptr<QImage>>();
    qRegisterMetaType<std::shared_ptr<icosphere::Icosphere>>();
    qRegisterMetaType<CurveType>();
    qRegisterMetaType<CalenhadAction>();
    qRegisterMetaType<Qt::PenStyle>();

    // set up application
    QCoreApplication::setOrganizationName("calenhad");
    QCoreApplication::setOrganizationDomain("chateauferret.com");
    QCoreApplication::setApplicationName("calenhad");
    QApplication app (argc, argv);


    // set up services
    // Preferences service
    Preferences* preferences = new Preferences();
    preferences -> loadSettings();
    CalenhadServices::providePreferences (preferences);

    // Legends service

    LegendRoster* roster = new LegendRoster();
    CalenhadServices::provideLegends (roster);

    // Projections service
    ProjectionService* projections = new ProjectionService();
    CalenhadServices::provideProjections (projections);

    // Calculator service
    Calculator* calculator = new Calculator();
    CalenhadServices::provideCalculator (calculator);


    // Stylesheet
    QString fileName = CalenhadServices::preferences() -> calenhad_stylesheet;
    QFile file (fileName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String (file.readAll());
    app.setStyleSheet (styleSheet);
    std::cout << "Stylesheet: " << file.fileName().toStdString() << "\n";

    // Calenhad model - the arrangement of modules and connections between them

    Calenhad* window = new Calenhad();
    window -> show();


    return app.exec();
}
