#include <iostream>
#include <QApplication>
#include "nodeedit/Calenhad.h"
#include "httplistener.h"
#include "httpserver/CalenhadRequestHandler.h"

#include <memory>
#include "preferences.h"
#include "controls/AltitudeMapPlot.h"
#include "CalenhadServices.h"
#include "LegendRoster.h"
#include "pipeline/GlobeRenderJob.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace Marble;

Q_DECLARE_METATYPE (TileId)
Q_DECLARE_METATYPE (std::shared_ptr<QImage>)
Q_DECLARE_METATYPE (std::shared_ptr<icosphere::Icosphere>)
Q_DECLARE_METATYPE (std::shared_ptr<GlobeBuffer>)
Q_DECLARE_METATYPE (CurveType)

int main (int argc, char **argv) {


    // Register required metatypes for Qt services
    qRegisterMetaType<QImage>();
    qRegisterMetaType<std::shared_ptr<QImage>>();
    qRegisterMetaType<std::shared_ptr<icosphere::Icosphere>>();
    qRegisterMetaType<std::shared_ptr<GlobeBuffer>>();
    qRegisterMetaType<TileId>();
    qRegisterMetaType<CurveType>();

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
    QString fname = CalenhadServices::preferences() -> calenhad_legends_filename;
    LegendRoster* roster = new LegendRoster();
    roster->inflate (fname);
    CalenhadServices::provideLegends (roster);

    // Gradients service (to do)

    // Projections service
    ProjectionService* projections = new ProjectionService();
    CalenhadServices::provideProjections (projections);

    // Stylesheet
    QString fileName = CalenhadServices::preferences() -> calenhad_stylesheet;
    QFile file (fileName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String (file.readAll());
    app.setStyleSheet (styleSheet);;

    // Calenhad model - the arrangement of modules and connections between them
    CalenhadModel* model = new CalenhadModel();
    Calenhad* window = new Calenhad();

    // Message service
    MessageFactory* factory = new MessageFactory();
    factory -> setHost (window);
    CalenhadServices::provideMessages (factory);
    window -> setModel (model);
    window -> show();

    // set up web service endpoint
    //QSettings* listenerSettings = new QSettings("/home/martin/.config/calenhad/webapp1.ini", QSettings::IniFormat, &app);
    //qDebug("config file loaded");
    //listenerSettings -> beginGroup("listener");

    // Start the HTTP server
    //CalenhadRequestHandler* handler = new CalenhadRequestHandler (&app);
    //handler -> setModel (model);
    //HttpListener* listener = new HttpListener (listenerSettings, handler, &app);

    // done
    return app.exec();
}