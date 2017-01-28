#include <iostream>
#include <QApplication>
#include "nodeedit/qnemainwindow.h"
#include "httplistener.h"
#include "httpserver/CalenhadRequestHandler.h"


// Includes for OSGEarth test
#include "preferences.h"


#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif


int main (int argc, char **argv) {

    QCoreApplication::setOrganizationName("calenhad");
    QCoreApplication::setOrganizationDomain("chateauferret.com");
    QCoreApplication::setApplicationName("Calenhad");
    QNEMainWindow::preferences = new Preferences();
    QNEMainWindow::preferences -> loadSettings();

    QString fileName = QNEMainWindow::preferences -> calenhad_stylesheet;
    QFile file (fileName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String (file.readAll());
    QApplication app (argc, argv);
    app.setStyleSheet (styleSheet);;

    CalenhadModel* model = new CalenhadModel();
    QNEMainWindow* window = new QNEMainWindow();
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