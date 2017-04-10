#include <iostream>
#include <QApplication>
#include "nodeedit/Calenhad.h"
#include "httplistener.h"
#include "httpserver/CalenhadRequestHandler.h"

#include <memory>
#include "preferences.h"
#include "controls/AltitudeMapPlot.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace Marble;

Q_DECLARE_METATYPE (TileId)
Q_DECLARE_METATYPE (std::shared_ptr<QImage>)
Q_DECLARE_METATYPE (std::shared_ptr<icosphere::Icosphere>)
Q_DECLARE_METATYPE (CurveType)

int main (int argc, char **argv) {


    // Register required metatypes for Qt services
    qRegisterMetaType<QImage>();
    qRegisterMetaType<std::shared_ptr<QImage>>();
    qRegisterMetaType<std::shared_ptr<icosphere::Icosphere>>();
    qRegisterMetaType<TileId>();
    qRegisterMetaType<CurveType>();

    QCoreApplication::setOrganizationName("calenhad");
    QCoreApplication::setOrganizationDomain("chateauferret.com");
    QCoreApplication::setApplicationName("calenhad");
    Calenhad::preferences = new Preferences();
    Calenhad::preferences -> loadSettings();

    QString fileName = Calenhad::preferences -> calenhad_stylesheet;
    QFile file (fileName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String (file.readAll());
    QApplication app (argc, argv);
    app.setStyleSheet (styleSheet);;

    CalenhadModel* model = new CalenhadModel();
    Calenhad* window = new Calenhad();
    window -> setModel (model);
    window -> show();

    // set up web service endpoint
    QSettings* listenerSettings = new QSettings("/home/martin/.config/calenhad/webapp1.ini", QSettings::IniFormat, &app);
    qDebug("config file loaded");
    listenerSettings -> beginGroup("listener");

    // Start the HTTP server
    CalenhadRequestHandler* handler = new CalenhadRequestHandler (&app);
    handler -> setModel (model);
    HttpListener* listener = new HttpListener (listenerSettings, handler, &app);

    return app.exec();
}