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

#include <QMenuBar>
#include <QLayout>
#include <controls/CalenhadLegendDialog.h>
#include <QtGui/QGuiApplication>
#include "Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "preferences/preferences.h"
#include "CalenhadView.h"
#include "src/CalenhadServices.h"
#include "controls/QIconPalette.h"
#include "../exprtk/VariablesDialog.h"
#include "module/Node.h"
#include "../legend/LegendService.h"
#include "Connection.h"
#include "NodeBlock.h"
#include "Port.h"
#include "ProjectPropertiesDialog.h"
#include <QClipboard>
#include <QtWidgets/QMessageBox>
#include <controls/SplashDialog.h>
#include "../pipeline/ModuleFactory.h"
#include "../nodeedit/CalenhadController.h"
#include "../controls/globe/CalenhadGlobeDialog.h"

using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;
using namespace calenhad::module;
using namespace calenhad::legend;
using namespace calenhad::expressions;
using namespace calenhad::notification;
using namespace calenhad::controls::globe;



Calenhad::Calenhad (QWidget* parent) : QNotificationHost (parent),
    _legendDialog (nullptr), _model (nullptr), _splash (nullptr), _globe (nullptr) {
    setWindowTitle (tr ("Calenhad"));
    // Message service
    CalenhadServices::provideMessages (this);
    _controller = new CalenhadController (this);

    // Nodes editor
    _docs = new QMdiArea (this);
    addView (_model, "View 1");
    //_scroll = new QScrollArea (this);
    //_scroll -> setWidgetResizable (true);
    //_scroll -> setWidget (_view);
    setCentralWidget (_docs);
    setDockNestingEnabled (true);


// Settings

    QSettings* settings = CalenhadServices::preferences() -> settings();
    settings -> beginGroup ("MainWindow");
    resize (settings -> value ("size", QSize(400, 400)).toSize());
    move (settings -> value ("pos", QPoint(200, 200)).toPoint());
    settings -> endGroup();
    setMenuBar (_controller -> getMenuBar());
}

Calenhad::~Calenhad() {
    delete _legendDialog;
    delete _globe;
}

void Calenhad::resizeEvent (QResizeEvent* event) {
    reorder();
}

void Calenhad::moveEvent (QMoveEvent* event) {
    reorder();
}

void Calenhad::setModel (CalenhadModel* model) {
    _model = model;
    _controller -> setModel (_model);
    _model -> setController (_controller);
    connect (_model, &CalenhadModel::titleChanged, this, &Calenhad::titleChanged);
    _controller -> updateZoomActions();
    CalenhadServices::provideGlobe (new CalenhadGlobeDialog (this));

    if (activeView()) {
        activeView() -> setModel (_model);
    }
}

CalenhadModel* Calenhad::model() {
    return _model;
}

void Calenhad::saveFileAs (const CalenhadFileType& fileType) {
    QString fname = QFileDialog::getSaveFileName();
    _model -> serialize (fname, fileType);
    _lastFile = fname;
    _controller -> rememberFile (fname);
}

void Calenhad::saveFile() {
    if (_lastFile.isNull()) {
        saveFileAs (CalenhadFileType::CalenhadModelFile);
    } else {
        _model -> serialize (_lastFile, CalenhadFileType::CalenhadModelFile);
    }
}

void Calenhad::loadFile (const CalenhadFileType& fileType) {
    QString fname = QFileDialog::getOpenFileName ();
    loadFile (fname, fileType);
}

void Calenhad::loadFile (const QString& fname, const CalenhadFileType& fileType) {
    _model -> inflate (fname);
    _lastFile = fname;
    _controller -> rememberFile (fname);
}

void Calenhad::showEvent (QShowEvent* event) {

}

void Calenhad::closeEvent (QCloseEvent* event) {
    closeProject();
    CalenhadServices::preferences() -> saveSettings();
    QSettings* settings = CalenhadServices::preferences() -> settings();
    settings -> beginGroup ("MainWindow");
    settings -> setValue ("size", size());
    settings -> setValue ("pos", pos());
    settings -> endGroup();
    event -> accept();
    delete _splash;
}

void Calenhad::addToolbar (QToolBar* toolbar, Module* m) {
    QDockWidget* paramsDock = new QDockWidget (m -> name(), this);
    paramsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    paramsDock -> setParent (this);
    //paramsDock -> setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    paramsDock -> setFixedSize (300, 200);
    addDockWidget (Qt::LeftDockWidgetArea, paramsDock);
    paramsDock -> setWidget (toolbar);
    paramsDock -> setFloating (true);
    connect (m, SIGNAL (nameChanged (const QString&)), paramsDock, SLOT (setWindowTitle (const QString&)));

}

CalenhadController* Calenhad::controller () {
    return _controller;
}

void Calenhad::openProject (const QString& filename) {
    if (_splash && _splash->isVisible()) { _splash -> close(); }
    closeProject();
    CalenhadModel* model = new CalenhadModel();
    setModel (model);
    loadFile (filename, CalenhadFileType::CalenhadModelFile);
    clearUndo();
}


void Calenhad::open() {
    if (_splash && _splash->isVisible()) { _splash -> close(); }
    openProject (QFileDialog::getOpenFileName (this, "Open project", "/home/martin", "Calenhad project files (*.chp *.xml)"));
}


void Calenhad::newProject() {
    if (_splash && _splash -> isVisible()) { _splash -> close(); }
    closeProject();
    QString fname = CalenhadServices::preferences() -> calenhad_legends_filename;
    CalenhadModel* model = new CalenhadModel();
    model -> inflate (fname, calenhad::nodeedit::CalenhadLegendFile);
    setModel (model);
    clearUndo();
}

void Calenhad::closeProject() {
    if (_splash && _splash->isVisible()) { _splash -> close(); }

        if (_model) {
            if (_model -> isChanged()) {
                if (QMessageBox::question (this, "Save file", "Save this model before closing?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                    saveFile ();
                }
            }
            CalenhadServices::disposeGlobe();
            delete _model;
            _model = nullptr;
        }

    CalenhadServices::messages() -> clearAll();
    clearUndo();
}

void Calenhad::quit() {
    if (_splash && _splash->isVisible()) { _splash -> close(); }
    close();
}


void Calenhad::titleChanged (const QString& title) {
    setWindowTitle (title);
}

void Calenhad::projectProperties () {
    ProjectPropertiesDialog* dialog = new ProjectPropertiesDialog (_model);
    dialog -> show();
}

void Calenhad::clearUndo () {
    _controller -> clearUndo ();
}


void Calenhad::manageLegends() {
    if (!_legendDialog) {
        _legendDialog = new CalenhadLegendDialog (this);
    }
    _legendDialog -> show();
}


void Calenhad::showSplash() {
    if (! _splash) { provideSplashDialog(); }
    _splash -> show();
}

void Calenhad::provideSplashDialog() {
    _splash = new SplashDialog(this);
    connect(_splash, &SplashDialog::openProject, this, &Calenhad::openProject);
    connect(_splash, &SplashDialog::closeCalenhad, this, &Calenhad::quit);
    connect(_splash, &SplashDialog::newProject, this, &Calenhad::newProject);
}

void Calenhad::showGlobe (Module* module) {
    if (! _globe) { provideGlobe (module); }
    _globe -> selectModule (module);
    _globe -> show();
}

void Calenhad::provideGlobe (Module* module) {
    _globe = new CalenhadGlobeDialog (this);
    CalenhadServices::provideGlobe (_globe);
}

QList<CalenhadView*> Calenhad::views() const {
    QList<QMdiSubWindow*> docs = _docs -> subWindowList();
    QList<CalenhadView*> views;
    for (QMdiSubWindow* doc : docs) {
        views.append ((CalenhadView*) doc -> widget());
    }
    return views;
}

CalenhadView* Calenhad::activeView() const {
    if (! _docs -> activeSubWindow()) {
        if (_docs->subWindowList ().isEmpty ()) {
            addView (nullptr, QString ());
        }
        _docs -> setActiveSubWindow (_docs -> subWindowList().first());
    }
    QMdiSubWindow* doc = _docs -> activeSubWindow();

    if (doc) {
        return (CalenhadView*) doc -> widget();
    } else {
        return nullptr;
    }
}

void Calenhad::addView (CalenhadModel* model, const QString& title) const {
    CalenhadView* _view = new CalenhadView (nullptr);
    _view -> setModel (model);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    _view -> centerOn (0, 0);
    double extent = CalenhadServices::preferences() -> calenhad_model_extent / 2;
    QRectF minRect (-extent / 2, -extent / 2, extent, extent);
    _view -> setSceneRect (minRect);
    QMdiSubWindow* w = _docs -> addSubWindow (_view);
    w -> setWindowTitle (title);
    w -> setAttribute (Qt::WA_DeleteOnClose);
    _docs -> setActiveSubWindow (w);
    _controller -> addView (_view);
    w -> show();
}

void Calenhad::closeActiveView() const {
    CalenhadView* view = (CalenhadView*) _docs -> activeSubWindow() -> widget();
    _docs -> activateNextSubWindow();
    QMdiSubWindow* w = _docs -> activeSubWindow();
    _docs -> removeSubWindow (w);
    delete view;
}

void Calenhad::tileViews() const {
    _docs -> tileSubWindows();
}

void Calenhad::cascadeViews () const {
    _docs -> cascadeSubWindows();
}

void Calenhad::makeWindowSwitchMenu (QMenu* menu) {
    menu -> clear();
    for (QMdiSubWindow* w : _docs -> subWindowList()) {
        QAction* a = new QAction (w -> windowTitle());
        a -> setCheckable (true);
        a -> setChecked (w -> isActiveWindow());
        connect (a, &QAction::toggled, this, [=] { _docs -> setActiveSubWindow (w); });
        menu -> addAction (a);
    }
}
