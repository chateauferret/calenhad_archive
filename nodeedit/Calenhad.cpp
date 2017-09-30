/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <QtCore/QCoreApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QCloseEvent>
#include <QtCore/QTextStream>
#include <QLayout>
#include <controls/CalenhadLegendDialog.h>
#include "Calenhad.h"
#include "CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "preferences/preferences.h"
#include "CalenhadView.h"
#include "../nodeedit/qnetoolbox.h"
#include "../CalenhadServices.h"
#include "controls/QIconPalette.h"
#include "../exprtk/VariablesDialog.h"
#include "../qmodule/QNode.h"
#include "../legend/LegendService.h"
#include "CalenhadToolBar.h"


using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::legend;
using namespace calenhad::expressions;
using namespace calenhad::notification;

QNEToolBox* Calenhad::toolbox = new QNEToolBox();

Calenhad::Calenhad (QWidget* parent) : QNotificationHost (parent),
    _legendDialog (nullptr) {

    _controller = new CalenhadController (this);

    // Nodes editor

    _view = new CalenhadView (this);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    _view -> centerOn (0, 0);

    setCentralWidget (_view);
    setDockNestingEnabled (true);

    // Legends
    initialiseLegends();

    // Tools

    CalenhadToolBar* viewToolbar = toolbox -> toolbar ("View");
    viewToolbar -> setAcceptDrops (false);
    QDockWidget* viewToolsDock = new QDockWidget (viewToolbar -> windowTitle(), this);
    viewToolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    viewToolbar -> setParent (this);
    viewToolsDock -> setWidget (viewToolbar);
    connect (viewToolsDock, &QDockWidget::dockLocationChanged, viewToolbar, &CalenhadToolBar::arrange);
    addDockWidget (Qt::TopDockWidgetArea, viewToolsDock);


    // modules and other nodes
    QDockWidget* iconsDock = new QDockWidget ("Modules", this);
    iconsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    QIconPalette* iconsPanel = new QIconPalette (this);iconsPanel -> show();

    iconsDock -> setWidget (iconsPanel);
    iconsDock -> layout() -> setAlignment (Qt::AlignLeft);
    connect (iconsDock, SIGNAL (dockLocationChanged (Qt::DockWidgetArea)), iconsPanel, SLOT (moved (Qt::DockWidgetArea)));
    connect (iconsDock, &QDockWidget::topLevelChanged, this, [=] () { iconsPanel -> moved (Qt::NoDockWidgetArea); });
    connect (iconsDock, &QDockWidget::topLevelChanged, this, [=] () { iconsPanel -> moved (Qt::NoDockWidgetArea); });
    addDockWidget (Qt::LeftDockWidgetArea, iconsDock);

    // Actions

    QAction* quitAction = new QAction (tr ("&Quit"), this);
    quitAction -> setShortcuts (QKeySequence::Quit);
    quitAction -> setStatusTip (tr ("Quit the application"));
    connect (quitAction, SIGNAL (triggered()), qApp, SLOT (quit()));

    QAction* loadAction = new QAction (tr ("&Load"), this);
    loadAction -> setShortcuts (QKeySequence::Open);
    loadAction -> setStatusTip (tr ("Open a file"));
    connect (loadAction, &QAction::triggered, this, [=] () { loadFile(); });
    QAction* saveAction = new QAction (tr ("&Save"), this);
    saveAction -> setShortcuts (QKeySequence::Save);
    saveAction -> setStatusTip (tr ("Save a file"));
    connect (saveAction, &QAction::triggered, this, [=] () { saveFile(); });

    QAction* loadLegendsAction = new QAction ("Load legends", this);
    loadLegendsAction -> setStatusTip ("Import legends from a Calenhad file");
    connect (loadLegendsAction, &QAction::triggered, this, [=] () { loadFile (CalenhadFileType::CalenhadLegendFile); });

    QAction* saveLegendsAction = new QAction ("Save legends", this);
    saveLegendsAction -> setStatusTip ("Export legends to a separate file");
    connect (saveLegendsAction, &QAction::triggered, this, [=] () { saveFile (CalenhadFileType::CalenhadLegendFile); });

    QAction* manageLegendsAction = new QAction ("Manage legends", this);
    manageLegendsAction -> setStatusTip ("Manage legends");
    connect (manageLegendsAction, &QAction::triggered, this, [=] () {
        if (! _legendDialog) {
            _legendDialog = new CalenhadLegendDialog (this);
        }
        _legendDialog -> show();
    });

    // Menu

    QMenu* fileMenu = menuBar () -> addMenu (tr ("&File"));
    fileMenu -> addAction (loadAction);
    fileMenu -> addAction (saveAction);

    fileMenu -> addSeparator();
    fileMenu -> addAction (quitAction);

    QMenu* editMenu = menuBar() -> addMenu (tr ("&Edit"));
    editMenu -> setObjectName ("editMenu");

    QMenu* toolMenu = toolbox -> menu ("Modules");
    menuBar() -> addMenu (toolMenu);
    QMenu* legendsMenu = new QMenu ("Legends");
    legendsMenu -> addAction (loadLegendsAction);
    legendsMenu -> addAction (saveLegendsAction);
    legendsMenu -> addAction (manageLegendsAction);
    QMenu* viewMenu = toolbox -> menu ("View");
    viewMenu -> addMenu (legendsMenu);
    menuBar() -> addMenu (viewMenu);

    setWindowTitle (tr ("Node Editor"));

    // Settings

    QSettings* settings = CalenhadServices::preferences() -> settings();
    settings -> beginGroup ("MainWindow");
    resize (settings -> value("size", QSize(400, 400)).toSize());
    move (settings -> value("pos", QPoint(200, 200)).toPoint());
    settings -> endGroup();
    _controller -> addMenus (menuBar());

    // Dialogs

    _variablesDialog = new VariablesDialog();
    QAction* variablesAction = new QAction (tr ("&Variables"));
    saveAction -> setStatusTip (tr ("Variables used in the script"));
    editMenu -> addAction (variablesAction);
    connect (variablesAction, &QAction::triggered, this, [=] () { _variablesDialog -> exec(); });
}

Calenhad::~Calenhad() {

}

void Calenhad::resizeEvent (QResizeEvent* event) {
    reorder();
}

void Calenhad::moveEvent (QMoveEvent* event) {
    reorder();
}

void Calenhad::setModel (CalenhadModel* model) {
    _model = model;
    _model -> setSceneRect (-1000, -1000, 1000, 1000);
    _controller-> setModel (_model);
    _model -> setController (_controller);
    _view -> setController (_controller);
    _controller -> addView (_view);
    _view -> setScene (_model);
}

CalenhadModel* Calenhad::model() {
    return _model;
}

void Calenhad::saveFile (const CalenhadFileType& fileType) {
    QString fname = QFileDialog::getSaveFileName();
    _model -> serialize (fname, fileType);
    _lastFile = fname;
}

void Calenhad::loadFile (const CalenhadFileType& fileType) {
    QString fname = QFileDialog::getOpenFileName ();
    _model -> inflate (fname);
    _lastFile = fname;
}

void Calenhad::closeEvent (QCloseEvent* event) {
    CalenhadServices::preferences() -> saveSettings();
    QSettings* settings = CalenhadServices::preferences() -> settings();
    settings -> beginGroup ("MainWindow");
    settings -> setValue ("size", size());
    settings -> setValue ("pos", pos());
    settings -> endGroup();
    event -> accept();
}

void Calenhad::initialiseLegends() {

}

bool Calenhad::readXml (const QString& fname, QDomDocument& doc) {
    return false;
}

void Calenhad::addToolbar (QToolBar* toolbar, QNode* node) {
    QDockWidget* paramsDock = new QDockWidget (node -> name(), this);
    paramsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    paramsDock -> setParent (this);
    //paramsDock -> setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    paramsDock -> setFixedSize (300, 200);
    addDockWidget (Qt::LeftDockWidgetArea, paramsDock);
    paramsDock -> setWidget (toolbar);
    paramsDock -> setFloating (true);
    connect (node, SIGNAL (nameChanged (const QString&)), paramsDock, SLOT (setWindowTitle (const QString&)));

}

CalenhadController* Calenhad::controller () {
    return _controller;
}
