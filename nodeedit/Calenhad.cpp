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
#include "../CalenhadServices.h"
#include "controls/QIconPalette.h"
#include "../exprtk/VariablesDialog.h"
#include "../qmodule/QNode.h"
#include "../legend/LegendService.h"
#include "qneconnection.h"
#include "QNodeBlock.h"
#include "qneport.h"


using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls;
using namespace calenhad::nodeedit;
using namespace calenhad::pipeline;
using namespace calenhad::qmodule;
using namespace calenhad::legend;
using namespace calenhad::expressions;
using namespace calenhad::notification;



Calenhad::Calenhad (QWidget* parent) : QNotificationHost (parent),
    _legendDialog (nullptr) {

    _controller = new CalenhadController (this);
    _toolbox = new QNEToolBox();

    // tool drawers - tools in the same drawer are grouped in the UI
    _addModuleDrawer = new ToolDrawer ("Modules");
    _toolbox -> addDrawer (_addModuleDrawer);
    _viewDrawer = new ToolDrawer ("View");
    _toolbox -> addDrawer (_viewDrawer);
    _editDrawer = new ToolDrawer ("Edit");
    _toolbox -> addDrawer (_editDrawer);

    // Nodes editor

    _view = new CalenhadView (this);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    _view -> centerOn (0, 0);

    setCentralWidget (_view);
    setDockNestingEnabled (true);

    // Legends
    initialiseLegends();

    // Tools

    CalenhadToolBar* viewToolbar = _toolbox -> toolbar ("View");
    viewToolbar -> setAcceptDrops (false);
    QDockWidget* viewToolsDock = new QDockWidget (viewToolbar -> windowTitle(), this);
    viewToolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    viewToolbar -> setParent (this);
    viewToolsDock -> setWidget (viewToolbar);
    connect (viewToolsDock, &QDockWidget::dockLocationChanged, viewToolbar, &CalenhadToolBar::arrange);
    addDockWidget (Qt::TopDockWidgetArea, viewToolsDock);

    CalenhadToolBar* editToolbar = _toolbox -> toolbar ("Edit");
    editToolbar -> setAcceptDrops (false);
    QDockWidget* editToolsDock = new QDockWidget (editToolbar -> windowTitle(), this);
    editToolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    editToolbar -> setParent (this);
    editToolsDock -> setWidget (editToolbar);
    connect (editToolsDock, &QDockWidget::dockLocationChanged, editToolbar, &CalenhadToolBar::arrange);
    addDockWidget (Qt::TopDockWidgetArea, editToolsDock);

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
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_perlin, "Perlin noise");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_billow, "Billow noise");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_ridgedmulti, "Ridged multifractal noise");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_cylinders, "Cylindrical distance function");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_spheres, "Spherical distance function");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_exponent, "Exponent function");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_translate, "Translation function");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_rotate, "Rotation function");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_scalepoint, "Scale points");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_scalebias, "Scale and bias");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_add, "Add values");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_diff, "Difference between values");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_clamp, "Clamp values");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_constant, "Constant value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_abs, "Absolute value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_blend, "Blend points");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_invert, "Invert values");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_max, "Maximum value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_min, "Minimum value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_voronoi, "Voronoi pattern");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_select, "Select input");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_turbulence, "Turbulence");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_icospheremap, "Icosphere map");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_altitudemap, "Altitude map");

    // A tool for adding a new node group
    QAction* tool = createTool (QIcon (":/sppicons/controls/group_add.png"), "NodeGroup", "Add a new group", "NodeGroup", _addModuleDrawer, true);
    tool -> setCheckable (true);
    _addModuleGroup -> addTool (tool);

    _zoomMenu = new QMenu ("Zoom");
    _defaultContextMenu -> addMenu (_toolbox -> menu ("Modules"));
    _defaultContextMenu -> addMenu (_zoomMenu);

    // scale actions
    zoomInAction = createTool (QIcon (":/appicons/controls/zoom_in.png"), ("Zoom &in"), "Zoom in", CalenhadAction::ZoomInAction, _viewDrawer);
    zoomOutAction = createTool (QIcon (":/appicons/controls/zoom_out.png"), tr ("Zoom &out"), "Zoom out", CalenhadAction::ZoomOutAction, _viewDrawer);
    zoomToFitAction = createTool (QIcon (":/appicons/controls/zoom_to_fit.png"), tr ("Zoom to &fit"), "Zoom to fit", CalenhadAction::ZoomToFitAction, _viewDrawer);
    zoomSelectionAction = createTool (QIcon (":/appicons/controls/zoom.png"), tr ("Zoom to &selection"), "Zoom to selection", CalenhadAction::ZoomToSelectionAction, _viewDrawer);
    _zoomMenu -> addAction (zoomInAction);
    _zoomMenu -> addAction (zoomOutAction);
    _zoomMenu -> addAction (zoomToFitAction);
    _zoomMenu -> addAction (zoomSelectionAction);
    viewToolbar -> addAction (zoomInAction);
    viewToolbar -> addAction (zoomOutAction);
    viewToolbar -> addAction (zoomToFitAction);
    viewToolbar -> addAction (zoomSelectionAction);

    // undo/redo apparatus

    undoAction = createTool (QIcon (":/appicons/controls/undo.png"), tr ("Undo"), "Undo", CalenhadAction::UndoAction, _editDrawer);
    redoAction = createTool (QIcon (":/appicons/controls/redo.png"), tr ("Redo"), "Redo", CalenhadAction::RedoAction, _editDrawer);
    undoAction -> setEnabled (_controller -> canUndo());
    redoAction -> setEnabled (_controller -> canRedo());
    _defaultContextMenu -> addAction (undoAction);
    _defaultContextMenu -> addAction (redoAction);
    editToolbar -> addAction (undoAction);
    editToolbar -> addAction (redoAction);
    connect (_controller, &CalenhadController::canUndoChanged, this, [=] () { undoAction -> setEnabled (_controller -> canUndo()); });
    connect (_controller, &CalenhadController::canRedoChanged, this, [=] () { redoAction -> setEnabled (_controller -> canRedo()); });


    // other module actions
    deleteSelectionAction = createTool (QIcon (":/appicons.controls/delete_selection.png"), tr ("Delete selection"), "Delete selection", CalenhadAction::DeleteSelectionAction, _editDrawer);
    deleteSelectionAction -> setEnabled (false);
    _moduleContextMenu -> addAction (deleteSelectionAction);

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

    QMenu* toolMenu = _toolbox -> menu ("Modules");
    menuBar() -> addMenu (toolMenu);
    QMenu* legendsMenu = new QMenu ("Legends");
    legendsMenu -> addAction (loadLegendsAction);
    legendsMenu -> addAction (saveLegendsAction);
    legendsMenu -> addAction (manageLegendsAction);
    QMenu* viewMenu = _toolbox -> menu ("View");
    viewMenu -> addMenu (legendsMenu);
    menuBar() -> addMenu (viewMenu);

    setWindowTitle (tr ("Node Editor"));

    // Settings

    QSettings* settings = CalenhadServices::preferences() -> settings();
    settings -> beginGroup ("MainWindow");
    resize (settings -> value("size", QSize(400, 400)).toSize());
    move (settings -> value("pos", QPoint(200, 200)).toPoint());
    settings -> endGroup();
    addMenus (menuBar());

    // Dialogs

    _variablesDialog = new VariablesDialog();
    QAction* variablesAction = new QAction (tr ("&Variables"));
    saveAction -> setStatusTip (tr ("Variables used in the script"));
    editMenu -> addAction (variablesAction);
    connect (variablesAction, &QAction::triggered, this, [=] () { _variablesDialog -> exec(); });
}

Calenhad::~Calenhad() {
    delete _addModuleGroup;
    delete _viewDrawer;
    delete _editDrawer;
    delete _addModuleDrawer;
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
    connect (_view, &CalenhadView::viewZoomed, this, &Calenhad::updateZoomActions);
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

void Calenhad::addMenus (QMenuBar* menuBar) {
    QMenu* editMenu = menuBar -> findChild<QMenu*>("editMenu");
    if (editMenu) {
        editMenu -> addAction (deleteSelectionAction);
        editMenu -> addSeparator();
        editMenu -> addAction (undoAction);
        editMenu -> addAction (redoAction);
    }
}

QAction* Calenhad::createTool (const QIcon& icon, const QString& name, const QString& statusTip, const QVariant& id, ToolDrawer* drawer, const bool& toggle) {
    QAction* tool = new QAction (icon, name, this);
    tool -> setStatusTip (statusTip);
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

void Calenhad::addModuleTool (const QString& name, const QString& tooltip) {
    QAction* tool = createTool (QIcon (":/resources/appicons/tools/" + name + ".png"), name, tooltip, name, _addModuleDrawer, true);
    tool -> setCheckable (true);
    _addModuleGroup -> addTool (tool);
}

void Calenhad::clearTools() {
    _addModuleGroup -> toolToggled (false);
}

void Calenhad::setSelectionActionsEnabled (const bool& enabled) {
    deleteSelectionAction -> setEnabled (enabled);
}

void Calenhad::updateZoomActions() {
    if (! _controller -> views() -> isEmpty ()) {
        double z = _controller -> views() -> at (0) -> currentZoom ();
        zoomInAction -> setEnabled (z < 4.0);
        zoomOutAction -> setEnabled (z > 0.025);
    }
}