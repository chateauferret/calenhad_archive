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
#include <QScrollBar>
#include <QScrollArea>
#include <QFileDialog>
#include <QCloseEvent>
#include <QtCore/QTextStream>
#include <QLayout>
#include <controls/CalenhadLegendDialog.h>
#include <QtGui/QGuiApplication>
#include "Calenhad.h"
#include "CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "preferences/preferences.h"
#include "CalenhadView.h"
#include "../CalenhadServices.h"
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


using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;
using namespace calenhad::module;
using namespace calenhad::legend;
using namespace calenhad::expressions;
using namespace calenhad::notification;



Calenhad::Calenhad (QWidget* parent) : QNotificationHost (parent),
    _legendDialog (nullptr), _model (nullptr) {

    // Message service
    CalenhadServices::provideMessages (this);

    _controller = new CalenhadController (this);
    _toolbox = new ToolBox();

    // tool drawers - tools in the same drawer are grouped in the UI
    _addModuleDrawer = new ToolDrawer ("Modules");
    _toolbox -> addDrawer (_addModuleDrawer);
    _viewDrawer = new ToolDrawer ("View");
    _toolbox -> addDrawer (_viewDrawer);
    _editDrawer = new ToolDrawer ("Edit");
    _toolbox -> addDrawer (_editDrawer);
    _editDrawer = new ToolDrawer ("File");
    _toolbox -> addDrawer (_editDrawer);

    // Nodes editor

    _view = new CalenhadView (this);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    _view -> centerOn (0, 0);
    double extent = CalenhadServices::preferences() -> calenhad_model_extent / 2;
    QRectF minRect (-extent / 2, -extent / 2, extent, extent);
    _view -> setSceneRect (minRect);


    //_scroll = new QScrollArea (this);
    //_scroll -> setWidgetResizable (true);
    //_scroll -> setWidget (_view);
    setCentralWidget (_view);
    setDockNestingEnabled (true);

    // Legends
    initialiseLegends();

    // Tools

    CalenhadToolBar* viewToolbar = makeToolbar ("View");
    CalenhadToolBar* editToolbar = makeToolbar ("Edit");
    fileToolbar = makeToolbar ("File");

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

    // tool groups - activating a toggle action deactivates all others in the same group (like a radio button)
    _addModuleGroup = new ToolGroup();
    Calenhad::_toolbox -> addGroup (_addModuleGroup);

    _nodeRoster = new QWidget();
    _nodeRoster -> setLayout (new QGridLayout());

    // create context menus
    _connectionContextMenu = new QMenu ("Connection");
    _moduleContextMenu = new QMenu ("Module");
    _outputPortContextMenu = new QMenu ("Output");
    _inputPortContextMenu = new QMenu ("Input");
    _defaultContextMenu = new QMenu ("Menu");
    _addModuleMenu = new QMenu ("Add module");

    // tools to create modules

    //addModuleTool (CalenhadServices::preferences() -> calenhad_module_icospheremap, "Icosphere map");
    addModuleTool ("altitudemap", CalenhadServices::preferences() -> calenhad_module_altitudemap, "altitudemap");
    QAction* nodeGroupTool = addModuleTool ("nodegroup", CalenhadServices::preferences() -> calenhad_nodegroup, "nodegroup");
    QStringList types = CalenhadServices::modules() -> types();
    for (QString key :  types) {
        addModuleTool (key, CalenhadServices::modules() -> label (key), CalenhadServices::modules() -> description (key));
    }

    _viewMenu = new QMenu ("Zoom");
    _defaultContextMenu -> addMenu (_toolbox -> menu ("Modules"));
    _defaultContextMenu -> addMenu (_viewMenu);

    // scale actions
    zoomInAction = createTool (QIcon (":/appicons/controls/zoom_in.png"), tr ("Zoom &in"), "Zoom in", CalenhadAction::ZoomInAction, _viewDrawer);
    zoomOutAction = createTool (QIcon (":/appicons/controls/zoom_out.png"), tr ("Zoom &out"), "Zoom out", CalenhadAction::ZoomOutAction, _viewDrawer);
    zoomToFitAction = createTool (QIcon (":/appicons/controls/zoom_to_fit.png"), tr ("Zoom to &fit"), "Zoom to fit", CalenhadAction::ZoomToFitAction, _viewDrawer);
    zoomSelectionAction = createTool (QIcon (":/appicons/controls/zoom.png"), tr ("Zoom to &selection"), "Zoom to selection", CalenhadAction::ZoomToSelectionAction, _viewDrawer);
    _viewMenu -> addAction (zoomInAction);
    _viewMenu -> addAction (zoomOutAction);
    _viewMenu -> addAction (zoomToFitAction);
    _viewMenu -> addAction (zoomSelectionAction);
    _viewMenu -> addSeparator();
    viewToolbar -> addAction (zoomInAction);
    viewToolbar -> addAction (zoomOutAction);
    viewToolbar -> addAction (zoomToFitAction);
    viewToolbar -> addAction (zoomSelectionAction);
    connect (_view, &CalenhadView::zoomInRequested, zoomInAction, &QAction::trigger);
    connect (_view, &CalenhadView::zoomOutRequested, zoomOutAction, &QAction::trigger);

    // grid visible toggle on / off
    toggleGridAction = createTool (QIcon (":/appicons/controls/grid.png"), tr ("Toggle grid"), "Toggle grid", CalenhadAction::ToggleGridAction, _viewDrawer);
    toggleGridAction -> setCheckable (true);
    toggleGridAction -> setChecked (_view -> gridVisible ());
    viewToolbar -> addAction (toggleGridAction);
    _viewMenu -> addAction (toggleGridAction);

    // grid snapping on / off
    toggleSnapToGridAction = createTool (QIcon (":/appicons/controls/grid-snap.png"), tr ("Snap to grid"), "Snap to grid", CalenhadAction::ToggleSnapToGridAction, _viewDrawer);
    toggleSnapToGridAction -> setCheckable (true);
    toggleSnapToGridAction -> setChecked (_view -> gridVisible ());
    viewToolbar -> addAction (toggleSnapToGridAction);
    _viewMenu -> addAction (toggleSnapToGridAction);

    // module tree
    toggleModuleTreeAction = createTool (QIcon (":/appicons/controls/tree.png"), tr ("Module tree"), "Module tree", CalenhadAction::ToggleModuleTreeAction, _viewDrawer);
    toggleModuleTreeAction -> setCheckable (true);
    toggleModuleTreeAction -> setChecked (false);
    viewToolbar -> addAction (toggleModuleTreeAction);
    _viewMenu -> addAction (toggleModuleTreeAction);


    // undo/redo apparatus

    undoAction = createTool (QIcon (":/appicons/controls/undo.png"), tr ("Undo"), "Undo", CalenhadAction::UndoAction, _editDrawer);
    redoAction = createTool (QIcon (":/appicons/controls/redo.png"), tr ("Redo"), "Redo", CalenhadAction::RedoAction, _editDrawer);
    undoAction -> setEnabled (false);
    redoAction -> setEnabled (false);
    _defaultContextMenu -> addAction (undoAction);
    _defaultContextMenu -> addAction (redoAction);
    editToolbar -> addAction (undoAction);
    editToolbar -> addAction (redoAction);
    nodeGroupTool -> setIcon (QIcon (":/appicons/controls/group_add.png"));
    editToolbar -> addAction (nodeGroupTool);
    connect (_controller, &CalenhadController::canUndoChanged, this, [=] () { undoAction -> setEnabled (_controller -> canUndo()); });
    connect (_controller, &CalenhadController::canRedoChanged, this, [=] () { redoAction -> setEnabled (_controller -> canRedo()); });
    // other module actions

    newAction = createAction (QIcon (":/appicons/controls/new.png"), tr ("&New"), "Start a new project", QKeySequence::New);
    connect (newAction, &QAction::triggered, this, &Calenhad::newProject);
    fileToolbar -> addAction (newAction);

    QAction* closeAction = createAction (QIcon (":/appicons/controls/close.png"), tr ("&Close"), "Start a new project", QKeySequence::New);
    connect (closeAction, &QAction::triggered, this, &Calenhad::closeProject);
    fileToolbar -> addAction (closeAction);

    quitAction = createAction (QIcon (":/appicons/controls/quit.png"), tr ("&Quit"), "Quit the application", QKeySequence::Quit);
    connect (quitAction, &QAction::triggered, this, &Calenhad::quit);
    fileToolbar -> addAction (quitAction);

    openAction = createAction (QIcon (":/appicons/controls/open_file.png"), tr ("&Open"), "Open a Calenhad model file", QKeySequence::Open);
    connect (openAction, &QAction::triggered, this, &Calenhad::open);
    fileToolbar -> addAction (openAction);

    importAction = createAction (QIcon (":/appicons/controls/import_file.png"), tr ("&Open"), "Import a Calenhad model file into this project");
    connect (importAction, &QAction::triggered, this, [=] () { loadFile (CalenhadFileType::CalenhadModelFile); });
    fileToolbar -> addAction (openAction);

    QAction* saveAction = createAction (QIcon (":/appicons/controls/save.png"), tr ("&Save"), "Save model", QKeySequence::Save);
    connect (saveAction, &QAction::triggered, this, [=] () { saveFile(); });
    fileToolbar -> addAction (saveAction);

    QAction* saveAsAction = createAction (QIcon (":/appicons/controls/save_as.png"), tr ("&Save as..."), "Save model in a new file", QKeySequence::SaveAs);
    connect (saveAsAction, &QAction::triggered, this, [=] () { saveFileAs (CalenhadFileType::CalenhadModelFile); });
    fileToolbar -> addAction (saveAction);

    QAction* loadLegendsAction = new QAction ("Load legends", this);
    loadLegendsAction -> setStatusTip ("Import legends from a Calenhad file");
    connect (loadLegendsAction, &QAction::triggered, this, [=] () { loadFile (CalenhadFileType::CalenhadLegendFile); });

    QAction* saveLegendsAction = new QAction ("Save legends", this);
    saveLegendsAction -> setStatusTip ("Export legends to a separate file");
    connect (saveLegendsAction, &QAction::triggered, this, [=] () { saveFileAs (CalenhadFileType::CalenhadLegendFile); });

    QAction* projectPropertiesAction = createAction (QIcon (":/appicons/controls/properties.png"), tr ("Properties..."), "Project properties", QKeySequence::Preferences);
    connect (projectPropertiesAction, &QAction::triggered, this, &Calenhad::projectProperties);
    fileToolbar -> addAction (projectPropertiesAction);

    mouseModeGroup = new QActionGroup (this);

    selectModeAction = createAction (QIcon (":/appicons/controls/select.png"), tr ("Select mode"), "Select mode");
    selectModeAction -> setCheckable (true);
    selectModeAction -> setChecked (false);
    connect (selectModeAction, &QAction::toggled, this, &Calenhad::toggleMouseMode);
    viewToolbar -> addAction (selectModeAction);
    mouseModeGroup -> addAction (selectModeAction);

    panModeAction = createAction (QIcon (":/appicons/controls/pan.png"), tr ("Select mode"), "Select mode");
    panModeAction -> setCheckable (true);
    panModeAction -> setChecked (true);
    connect (panModeAction, &QAction::toggled, this, &Calenhad::toggleMouseMode);
    viewToolbar -> addAction (panModeAction);
    mouseModeGroup -> addAction (panModeAction);

    QMenu* mouseModeMenu = new QMenu ("Mouse mode");
    mouseModeMenu -> addAction (selectModeAction);
    mouseModeMenu -> addAction (panModeAction);
    _viewMenu -> addMenu (mouseModeMenu);


    QAction* xmlAction = createAction (QIcon (":/appicons/controls/xml.png"), tr ("&XML"), "View model as an XML file", QKeySequence::NativeText);
    connect (xmlAction, &QAction::triggered, this, [=] () {
        QDomDocument doc = _model -> serialize (CalenhadFileType::CalenhadModelFile);
        QString xml = doc.toString();
        QTextEdit* xmlText = new QTextEdit();
        xmlText -> setText (xml);
        xmlText -> setAttribute (Qt::WA_DeleteOnClose);
        xmlText -> setReadOnly (true);
        xmlText -> resize (480, 360);
        xmlText -> show();

    });
    fileToolbar -> addAction (xmlAction);

    QAction* manageLegendsAction = createAction (QIcon (":/appicons/controls/legend.png"), tr ("&Legends"), "Manage the list of map legends");

    connect (manageLegendsAction, &QAction::triggered, this, [=] () {
        if (! _legendDialog) {
            _legendDialog = new CalenhadLegendDialog (this);
        }
        _legendDialog -> show();
    });

    editToolbar -> addAction (manageLegendsAction);
    // Menu

    fileMenu = menuBar () -> addMenu (tr ("&File"));
    fileMenu -> addAction (newAction);
    fileMenu -> addAction (openAction);

    openRecentMenu = new QMenu();
    openRecentMenu -> setTitle ("Open recent");
    openRecentMenu -> setToolTip ("Open a file used recently");
    fileMenu -> addMenu (openRecentMenu);
    makeRecentFilesMenu();

    fileMenu -> addAction (saveAction);
    fileMenu -> addAction (saveAsAction);
    fileMenu -> addAction (closeAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction (loadLegendsAction);
    fileMenu -> addAction (saveLegendsAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction (projectPropertiesAction);
    fileMenu -> addAction (xmlAction);
    fileMenu -> addAction (quitAction);

    cutAction = createAction (QIcon (":/appicons/controls/cut.png"), tr ("Cut"), "Cut selection to the clipboard", QKeySequence::Cut);
    cutAction -> setEnabled (false);
    cutAction -> setData (CalenhadAction::CutAction);
    connect (cutAction, SIGNAL (triggered()), _controller, SLOT (actionTriggered ()));
    fileToolbar -> addAction (cutAction);

    copyAction = createAction (QIcon (":/appicons/controls/copy.png"), tr ("Copy"), "Copy selection to the clipboard", QKeySequence::Copy);
    copyAction -> setEnabled (false);
    copyAction -> setData (CalenhadAction::CopyAction);
    connect (copyAction, SIGNAL (triggered()), _controller, SLOT (actionTriggered ()));
    fileToolbar -> addAction (copyAction);

    pasteAction = createAction (QIcon (":/appicons/controls/paste.png"), tr ("Paste"), "Paste selection from the clipboard", QKeySequence::Paste);
    pasteAction -> setEnabled (false);
    pasteAction -> setData (CalenhadAction::PasteAction);
    connect (pasteAction, SIGNAL (triggered()), _controller, SLOT (actionTriggered ()));
    fileToolbar -> addAction (pasteAction);
    connect (QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Calenhad::updatePasteAction);
    updatePasteAction();

    deleteSelectionAction = createTool (QIcon (":/appicons/controls/delete_selection.png"), tr ("Delete selection"), "Delete selection", CalenhadAction::DeleteSelectionAction, _editDrawer);
    deleteSelectionAction -> setEnabled (false);
    _moduleContextMenu -> addAction (deleteSelectionAction);
    editToolbar -> addAction (deleteSelectionAction);

    assignSelectionToGroupAction = createTool (QIcon (":/appicons/controls/group_add.png"), tr ("Assign selected modules to a group"), "Assign selected modules to a group", CalenhadAction::NodeGroupFromSelectionAction, _editDrawer);
    assignSelectionToGroupAction -> setEnabled (false);
    _defaultContextMenu -> addAction (assignSelectionToGroupAction);
    _moduleContextMenu -> addAction (assignSelectionToGroupAction);
    editToolbar -> addAction (assignSelectionToGroupAction);

    manageGroupsAction = createTool (QIcon (":/appicons/controls/nodegroups.png"), tr ("Manage groups..."), "Create, edit and delete nodegroups", CalenhadAction::ManageNodeGroupsAction, _editDrawer);
    _defaultContextMenu -> addAction (manageGroupsAction);
    editToolbar -> addAction (manageGroupsAction);


    QMenu* editMenu = menuBar() -> addMenu (tr ("&Edit"));
    editMenu -> setObjectName ("editMenu");
    editMenu -> addAction (cutAction);
    editMenu -> addAction (copyAction);
    editMenu -> addAction (pasteAction);
    editMenu -> addAction (assignSelectionToGroupAction);
    editMenu -> addAction (manageGroupsAction);
    editMenu -> addSeparator();
    editMenu -> addAction (manageLegendsAction);

    QMenu* toolMenu = _toolbox -> menu ("Modules");
    menuBar() -> addMenu (toolMenu);
    QMenu* legendsMenu = new QMenu ("Legends");
    legendsMenu -> addAction (loadLegendsAction);
    legendsMenu -> addAction (saveLegendsAction);
    legendsMenu -> addAction (manageLegendsAction);
    QMenu* viewMenu = _toolbox -> menu ("View");
    viewMenu -> addMenu (legendsMenu);
    menuBar() -> addMenu (viewMenu);

    setWindowTitle (tr ("Calenhad"));

    // Settings

    QSettings* settings = CalenhadServices::preferences() -> settings();
    settings -> beginGroup ("MainWindow");
    resize (settings -> value ("size", QSize (400, 400)).toSize());
    move (settings -> value ("pos", QPoint (200, 200)).toPoint());
    settings -> endGroup();
    addMenus (menuBar());

    // Dialogs

    _variablesDialog = new VariablesDialog();
    QAction* variablesAction = createAction (QIcon (":/appicons/controls/variables.png"), tr ("&Variables"), "Review and edit module parameter variables", QKeySequence());
    editToolbar -> addAction (variablesAction);
    editMenu -> addAction (variablesAction);
    connect (variablesAction, &QAction::triggered, this, [=] () { _variablesDialog -> exec(); });

    // there is no model yet so switch all the controls off except for allowing a model to be created or opened
    _splash = new SplashDialog (this);
    _splash -> setModal (true);
    connect (_splash, &SplashDialog::openProject, this, &Calenhad::openProject);
    connect (_splash, &SplashDialog::closeCalenhad, this, &Calenhad::quit);
    connect (_splash, &SplashDialog::newProject, this, &Calenhad::newProject);
    setActive (false);
}

Calenhad::~Calenhad() {
    delete _addModuleGroup;
    delete _viewDrawer;
    delete _editDrawer;
    delete _addModuleDrawer;
    if (_legendDialog) { delete _legendDialog; }
}

void Calenhad::resizeEvent (QResizeEvent* event) {
    reorder();
}

void Calenhad::moveEvent (QMoveEvent* event) {
    reorder();
}

void Calenhad::setModel (CalenhadModel* model) {
    _model = model;
    _controller-> setModel (_model);
    _model -> setController (_controller);
    _view -> setController (_controller);
    _view -> setModel (_model);
    _controller -> addView (_view);
    connect (_view, &CalenhadView::viewZoomed, this, &Calenhad::updateZoomActions);
    connect (_model, &CalenhadModel::titleChanged, this, &Calenhad::titleChanged);
}

CalenhadModel* Calenhad::model() {
    return _model;
}

void Calenhad::saveFileAs (const CalenhadFileType& fileType) {
    QString fname = QFileDialog::getSaveFileName();
    _model -> serialize (fname, fileType);
    _lastFile = fname;
    rememberFile (fname);
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
    rememberFile (fname);
    setActive (true);
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

void Calenhad::initialiseLegends() {

}

bool Calenhad::readXml (const QString& fname, QDomDocument& doc) {
    return false;
}

void Calenhad::addToolbar (QToolBar* toolbar, Node* node) {
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

void Calenhad::addMenus (QMenuBar* menuBar) {
    QMenu* editMenu = menuBar -> findChild<QMenu*>("editMenu");
    if (editMenu) {
        editMenu -> addAction (deleteSelectionAction);
        editMenu -> addSeparator();
        editMenu -> addAction (undoAction);
        editMenu -> addAction (redoAction);
    }
}

QAction* Calenhad::createAction (const QIcon& icon, const QString& name, const QString statusTip, const QKeySequence& shortcut) {
    QAction* tool = new QAction (icon, name, this);
    tool -> setStatusTip (statusTip);
    return tool;
}

QAction* Calenhad::createTool (const QIcon& icon, const QString& name, const QString& statusTip, const QVariant& id, ToolDrawer* drawer, const bool& toggle, const QKeySequence& shortcut) {
    QAction* tool = createAction (icon, name, statusTip, shortcut);

    tool -> setCheckable (toggle);
    tool -> setData (id);

    drawer -> addTool (tool);
    if (toggle) {
        connect (tool, SIGNAL (toggled (bool)), _controller, SLOT (toolSelected (bool)));
    } else {
        connect (tool, SIGNAL (triggered()), _controller, SLOT (actionTriggered ()));
    }
    return tool;
}

QAction* Calenhad::addModuleTool (const QString& name, const QString& label, const QString& tooltip) {
    QAction* tool = createTool (QIcon (":/resources/appicons/tools/" + name + ".png"), label, tooltip, name, _addModuleDrawer, true);
    tool -> setCheckable (true);
    _addModuleGroup -> addTool (tool);
    return tool;
}

void Calenhad::clearTools() {
    _addModuleGroup -> toolToggled (false);
}

void Calenhad::setSelectionActionsEnabled (const bool& enabled) {
    deleteSelectionAction -> setEnabled (enabled);
    cutAction -> setEnabled (enabled);
    copyAction -> setEnabled (enabled);
    assignSelectionToGroupAction -> setEnabled (enabled);
}

void Calenhad::updateZoomActions() {
    if (! _controller -> views() -> isEmpty ()) {
        double z = _controller -> views() -> at (0) -> currentZoom ();
        zoomInAction -> setEnabled (z < CalenhadServices::preferences() -> calenhad_desktop_zoom_limit_zoomin); // 4
        zoomOutAction -> setEnabled (z > CalenhadServices::preferences() -> calenhad_desktop_zoom_limit_zoomout); // 0.025);
    }
}

void Calenhad::updatePasteAction() {
    pasteAction -> setEnabled (! (QGuiApplication::clipboard() -> text().isEmpty()));
}

CalenhadToolBar* Calenhad::makeToolbar (const QString& name) {
    CalenhadToolBar* toolbar = _toolbox -> toolbar (name);
    toolbar -> setAcceptDrops (false);
    QDockWidget* toolsDock = new QDockWidget (toolbar -> windowTitle(), this);
    toolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    toolbar -> setParent (this);
    toolsDock -> setWidget (toolbar);
    connect (toolsDock, &QDockWidget::dockLocationChanged, toolbar, &CalenhadToolBar::arrange);
    connect (toolsDock, &QDockWidget::topLevelChanged, toolbar, &CalenhadToolBar::detached);
    addDockWidget (Qt::TopDockWidgetArea, toolsDock);
    return toolbar;
}

void Calenhad::openProject (const QString& filename) {
    closeProject();
    CalenhadModel* model = new CalenhadModel();
    setModel (model);
    loadFile (filename, CalenhadFileType::CalenhadModelFile);
    setActive (true);
    clearUndo();
}


void Calenhad::open() {
    openProject (QFileDialog::getOpenFileName (this, "Open project", "/home/martin", "Calenhad project files (*.chp *.xml)"));
}


void Calenhad::newProject() {
    closeProject();
    QString fname = CalenhadServices::preferences() -> calenhad_legends_filename;
    CalenhadModel* model = new CalenhadModel();
    model -> inflate (fname, calenhad::nodeedit::CalenhadLegendFile);
    setModel (model);
    setActive (true);
    clearUndo();
}

void Calenhad::closeProject() {
    if (_view ) {
        if (_model) {
            if (_model -> isChanged()) {
                if (QMessageBox::question (_view, "Save file", "Save this model before closing?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                    saveFile ();
                }
            }
            _model -> suppressRender (true);   // otherwise destructing the model's contents will keep making it try to rerender
            delete _model;
            _model = nullptr;
        }
    }
    CalenhadServices::messages() -> clearAll();
    setActive (false);
    clearUndo();
}

void Calenhad::quit() {
    if (_splash) {
        _splash -> accept();
    }
    close();
}

void Calenhad::setActive (bool enabled) {
    for (QObject* object : children()) {
        QWidget* widget = dynamic_cast<QWidget*> (object);
        if (widget) {
            widget -> setEnabled (enabled);
        }
        QAction* action = dynamic_cast<QAction*> (object);
        if (action) {
            action -> setEnabled (enabled);
        }
    }

    // actions and widgets that must always be available to allow models to be created and opened
    newAction -> setEnabled (true);
    openAction -> setEnabled (true);
    quitAction -> setEnabled (true);
    _splash -> setEnabled (true);
    setActive (fileMenu, true);
    setActive (fileToolbar, true);

    if (! enabled) {
        _splash -> show();
        _view -> setEnabled (false);
    } else {
        _splash -> accept();
        _view -> setEnabled (true);
    }

}

void Calenhad::setActive (QWidget* widget, bool enabled) {
    if (widget -> parent()) {
        QWidget* p = dynamic_cast<QWidget*> (widget -> parent());
        if (p) {
            p -> setEnabled (enabled);
            setActive (p, enabled);
        }
    }
}

void Calenhad::rememberFile (const QString& file) {
    QStringList files = recentFiles();
    if (! files.contains (file)) {
        files.append (file);
    }
    QFile f (CalenhadServices::preferences() -> calenhad_recentfiles_filename);
    if (f.open (QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream out (&f);
        for (QString item : files) {
            out << item << "\n";
        }
        out.flush();
        f.close();
    }
    makeRecentFilesMenu();
}

QStringList Calenhad::recentFiles() {
    QFile f (CalenhadServices::preferences() -> calenhad_recentfiles_filename);
    QStringList files;
    if (f.exists()) {
        if (f.open (QIODevice::ReadOnly)) {
            while (! f.atEnd ()) {
                QByteArray line = f.readLine().trimmed();
                if (QFile::exists (line)) {
                    files.append (line);
                }
            }
            f.close();
        }
    }
    return files;
}

void Calenhad::makeRecentFilesMenu() {
    openRecentMenu -> clear();
    QStringList files = recentFiles();
    for (QString entry : files) {
        QFile f (entry);
        QDomDocument doc;
        doc.setContent (&f);
        QDomElement metadataElement = doc.documentElement().firstChildElement ("metadata");
        QDomElement titleElement = metadataElement.firstChildElement ("title");
        QString title = titleElement.text ();
        title += " (" + entry + ")";
        QAction* action = new QAction();
        action -> setText (title);
        action -> setData (entry);
        openRecentMenu -> addAction (action);
        connect (action, &QAction::triggered, this, [=] () {
            openProject (action -> data().toString());
        });
    }

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

void Calenhad::fixScrollBars() {
    QSize areaSize = _view -> viewport() -> size();
    QSize  widgetSize = _view -> size();
    _view -> verticalScrollBar()->setPageStep(areaSize.height());
    _view -> horizontalScrollBar()->setPageStep(areaSize.width());
    _view -> verticalScrollBar()->setRange(0, widgetSize.height() - areaSize.height());
    _view -> horizontalScrollBar()->setRange(0, widgetSize.width() - areaSize.width());
}

void Calenhad::toggleMouseMode () {
    QAction* a = (QAction*) sender();
    if (a == selectModeAction) {
        _model -> setMouseMode (a -> isChecked() ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
    }
    if (a == panModeAction) {
        _model -> setMouseMode (a -> isChecked() ? QGraphicsView::ScrollHandDrag :  QGraphicsView::RubberBandDrag);
    }


}
