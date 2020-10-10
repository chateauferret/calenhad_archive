//
// Created by ferret on 04/06/2020.
//

#include <QtWidgets/QDockWidget>
#include "CalenhadUi.h"
#include "nodeedit/CalenhadController.h"
#include "nodeedit/CalenhadToolBar.h"
#include "nodeedit/Port.h"
#include "module/Node.h"
#include "nodeedit/NodeBlock.h"
#include "nodeedit/Connection.h"
#include "pipeline/CalenhadModel.h"
#include "../actions/ContextAction.h"
#include "../controls/SplashDialog.h"
#include "../exprtk/VariablesDialog.h"
#include <QActionGroup>
#include "../nodeedit/CalenhadView.h"
#include "../pipeline/ModuleFactory.h"
#include "../controls/SplashDialog.h"

using namespace calenhad;
using namespace calenhad::nodeedit;
using namespace calenhad::module;
using namespace calenhad::pipeline;
using namespace calenhad::controls;
using namespace calenhad::expressions;
using namespace calenhad::controls::globe;

CalenhadUi::CalenhadUi (CalenhadController* controller) :
    _controller (controller),
    _app ((Calenhad*) (controller -> parent())),
    _applicationMenu (nullptr), _windowMenu (nullptr), _openRecentMenu (nullptr), _toolbarsMenu (nullptr),
    _defaultContextMenu (nullptr),
    _viewToolbar (nullptr), _modulesToolbar (nullptr), _editToolbar (nullptr), _fileToolbar (nullptr),
    _menuBar (nullptr), _mouseModeMenu (nullptr), _editMenu (nullptr), _viewMenu (nullptr), _fileMenu (nullptr), _moduleMenu (nullptr), _contextMenu (nullptr),
    _moduleGroup (nullptr),
    _mouseModeGroup (nullptr) {
}

void CalenhadUi::initialise() {
    _moduleGroup = new QActionGroup (_controller);
    _mouseModeGroup = new QActionGroup (_controller);
    makeActions();
    makeWidgets();

}

CalenhadUi::~CalenhadUi() {

}

QMenu* CalenhadUi::getApplicatonMenu() {
    return _applicationMenu;
}

// still a shambles FFS
void CalenhadUi::makeWidgets() {

    _defaultContextMenu = new QMenu (_app);

    // main menu
    _menuBar = _app -> menuBar();
    _fileMenu = _menuBar -> addMenu(tr("&File"));
    _fileMenu -> addAction (_newAction);
    _fileMenu -> addAction (_openAction);
    _fileMenu -> addAction (_saveAction);
    _fileMenu -> addAction (_saveAsAction);
    _fileMenu -> addAction (_importAction);
    _fileMenu -> addAction (_closeAction);
    _fileMenu -> addSeparator();
    _fileMenu -> addAction (_loadLegendsAction);
    _fileMenu -> addAction (_saveLegendsAction);
    _fileMenu -> addSeparator();
    _fileMenu -> addAction (_projectPropertiesAction);
    _fileMenu -> addAction (_xmlAction);
    _fileMenu -> addAction (_quitAction);
    _openRecentMenu = _fileMenu -> addMenu ("Open recent");
    _openRecentMenu -> setToolTip ("Open a file used recently");
    makeRecentFilesMenu();

    _editMenu = _menuBar -> addMenu (tr("&Edit"));
    _editMenu -> setObjectName("editMenu");
    _editMenu -> addAction (_cutAction);
    _editMenu -> addAction (_copyAction);
    _editMenu -> addAction (_pasteAction);
    _editMenu -> addAction (_deleteAction);
    _editMenu -> addSeparator();
    _editMenu -> addAction (_variablesAction);
    _mouseModeMenu = _editMenu -> addMenu ("Mouse &mode");
    _mouseModeMenu -> addAction (_selectModeAction);
    _mouseModeMenu -> addAction (_panModeAction);

    _viewMenu = _menuBar -> addMenu ("&View");
    _viewMenu -> addAction (_moduleTreeAction);
    _viewMenu -> addAction (_gridAction);
    _viewMenu -> addAction(_snapAction);
    _viewMenu -> addSeparator();
    _viewMenu -> addAction (_zoomInAction);
    _viewMenu -> addAction (_zoomOutAction);
    _viewMenu -> addAction (_zoomToFitAction);
    _viewMenu -> addAction (_zoomSelectionAction);

    _moduleMenu = _menuBar -> addMenu ("Modules");



    QMenu* legendsMenu = _viewMenu -> addMenu ("&Legends");
    legendsMenu -> addAction (_loadLegendsAction);
    legendsMenu -> addAction (_saveLegendsAction);
    legendsMenu -> addAction (_manageLegendsAction);

    // toolbars
    _toolbarsMenu = new QMenu ("Toolbars");

    _modulesToolbar = makeToolbar ("Modules");

        QStringList types = CalenhadServices::modules() -> types ();
        QMap<QString, QMenu*> actions;
        for (const QString& key : types) {
            QDomElement element = CalenhadServices::modules() -> xml (key);
            QString role = element.attribute ("role");
            QAction* action = CalenhadServices::modules() -> makeModuleTool (key);

            if (! actions.contains (role)) {
                QMenu* subMenu = new QMenu (role);
                actions.insert (role, subMenu);
                subMenu -> addAction (action);
                _moduleMenu -> addMenu (subMenu);
            } else {
                QMenu* subMenu = actions.find (role).value ();
                subMenu -> addAction (action);
            }

            connect (action, &QAction::toggled, _controller, &CalenhadController::toolSelected);
            _moduleGroup -> addAction (action);
        }


    QPushButton* moduleMenuButton = new QPushButton();
    moduleMenuButton -> setIcon (QIcon (":/appicons/controls/modules.png"));
    moduleMenuButton -> setToolTip ("Add a new module to the model");
    moduleMenuButton -> setMenu (_moduleMenu);
    _modulesToolbar -> addWidget (moduleMenuButton);

    _viewToolbar = makeToolbar ("View");
    _editToolbar = makeToolbar ("Edit");
    _fileToolbar = makeToolbar ("File");

    _fileToolbar -> addAction (_newAction);
    _fileToolbar -> addAction (_openAction);
    _fileToolbar -> addAction (_importAction);
    _fileToolbar -> addAction (_closeAction);
    _fileToolbar -> addAction (_quitAction);
    _fileToolbar -> addSeparator();
    _modulesToolbar -> addAction (_xmlAction);
    _fileToolbar -> addAction (_projectPropertiesAction);

    _editToolbar -> addAction (_cutAction);
    _editToolbar -> addAction (_copyAction);
    _editToolbar -> addAction (_pasteAction);
    _editToolbar -> addAction (_deleteAction);
    _editToolbar -> addSeparator();
    _editToolbar -> addAction (_variablesAction);
    _editToolbar -> addAction (_manageLegendsAction);
    _editToolbar -> addSeparator();
    _editToolbar -> addAction (_undoAction);
    _editToolbar -> addAction (_redoAction);
    _editToolbar -> addSeparator();
    _editToolbar -> addAction (_selectModeAction);
    _editToolbar -> addAction (_panModeAction);

    _viewToolbar -> addAction (_gridAction);
    _modulesToolbar -> addAction(_moduleTreeAction);
    _viewToolbar -> addAction(_snapAction);
    _viewToolbar -> addSeparator();
    _viewToolbar -> addAction (_zoomInAction);
    _viewToolbar -> addAction (_zoomOutAction);
    _viewToolbar -> addAction (_zoomToFitAction);
    _viewToolbar -> addAction (_zoomSelectionAction);

    _viewMenu -> addSeparator();
    _viewMenu -> addMenu (_toolbarsMenu);

    _windowMenu = _menuBar -> addMenu ("Window");
    _windowMenu -> addAction (_newWindowAction);
    _windowMenu -> addAction (_closeWindowAction);
    _windowMenu -> addAction (_tileWindowsAction);
    _windowMenu -> addAction (_cascadeWindowsAction);
    _windowSwitchMenu = _windowMenu -> addMenu ("Switch");
    connect (_windowMenu, &QMenu::aboutToShow, this, [=] () {
        Calenhad* c = (Calenhad*) _controller -> parent();
        c -> makeWindowSwitchMenu (_windowSwitchMenu);
    });
}


QToolBar * CalenhadUi::makeToolbar (const QString& name) {
    CalenhadToolBar* toolbar = new CalenhadToolBar (name);
    toolbar -> setOrientation (Qt::Vertical);
    toolbar -> setAcceptDrops (false);
    QDockWidget* toolsDock = new QDockWidget (toolbar -> windowTitle(), _app);
    _toolbarsMenu -> addAction (toolsDock -> toggleViewAction());

    toolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    toolsDock -> setWidget (toolbar);
    connect (toolsDock, &QDockWidget::dockLocationChanged, toolbar, &CalenhadToolBar::arrange);
    connect (toolsDock, &QDockWidget::topLevelChanged, toolbar, &CalenhadToolBar::detached);
    _app -> addDockWidget (Qt::TopDockWidgetArea, toolsDock);
    return toolbar;
}

// Make context menus for right clicks on various objects (or on the canvas, if *item is null)
QMenu* CalenhadUi::makeContextMenu (QGraphicsItem* item) {

    if (_contextMenu) { delete _contextMenu; _contextMenu = nullptr; }
    _contextMenu = nullptr;

    // if context is a Port, get the context menu from the Port
    if (dynamic_cast<Port*> (item)) {
        Port* port = static_cast<Port*> (item);
        _contextMenu = new QMenu ("Port");
        _contextMenu -> addMenu (port -> connectMenu());
        return _contextMenu;
    }

    // if context is a Connection, only action is to disconnect it
    if (dynamic_cast<Connection*> (item)) {
        // connection actions
        _contextMenu = new QMenu("Connection");
        Connection *c = dynamic_cast<Connection *> (item);
        QAction *disconnectAction = new QAction(QIcon(":/appicons/controls/disconnect.png"), "Disconnect");
        connect(disconnectAction, &ContextAction<Connection>::triggered, this, [=]() { _controller->disconnect(c); });
        _contextMenu->addAction(disconnectAction);
    }

    if (dynamic_cast<NodeBlock*> (item)) {
        NodeBlock* block = dynamic_cast<NodeBlock*> (item);
        Node* n = block -> node();
        _contextMenu = new QMenu (n -> name() + " (" + n -> nodeType() + ")");
        QAction* duplicateAction = new QAction (QIcon (":/appicons/controls/duplicate.png"), "Duplicate");
        connect (duplicateAction, &QAction::triggered, this, [=] () { _controller->duplicateNode(n); });
        QAction* deleteAction = new QAction (QIcon (":/appicons/controls/delete.png"), "Delete");
        connect (deleteAction, &QAction::triggered, this, [=] () { _controller->deleteNode(n); });
        _contextMenu -> addAction (duplicateAction);
        _contextMenu -> addAction (deleteAction);

        if (dynamic_cast<Module*> (n)) {
            Module* module = dynamic_cast<Module*> (n);
            _contextMenu -> addSeparator();
            QAction* editAction = new QAction (QIcon (":/appicons/controls/edit.png"), tr ("Edit"));
            editAction -> setToolTip ("Edit module's details and parameters");
            connect (editAction, &QAction::triggered, this, [=]() { module -> showModuleDetail (true); });
            _contextMenu -> addAction (editAction);
            QAction* globeAction = new QAction (QIcon (":/appicons/controls/globe.png"), "Show globe");
            connect (globeAction, &QAction::triggered, module, &Module::showGlobe);
            globeAction -> setEnabled (module -> isComplete());
            _contextMenu -> addAction (globeAction);
            _contextMenu -> addSeparator();

            QMenu* connectOutputMenu = module -> output() -> connectMenu();
            _contextMenu -> addMenu (connectOutputMenu);
            if (connectOutputMenu -> isEmpty()) {
                connectOutputMenu -> setEnabled (false);
            }
        }
        return _contextMenu;
    }

    // context menu for the canvas itself
    if (! _contextMenu) {
        _contextMenu = new QMenu ("Model");
    }

    // actions that operate on selections
    _contextMenu -> addSeparator();
    _contextMenu -> addAction (_copyAction);
    _contextMenu -> addAction (_cutAction);
    _contextMenu -> addAction (_deleteAction);
    _contextMenu -> addAction (_pasteAction);
    setEditActionStatus (_controller -> model());

    _contextMenu -> addMenu (_mouseModeMenu);
    _contextMenu->addAction (_undoAction);
    _contextMenu -> addAction (_redoAction);
    _contextMenu -> addMenu (_moduleMenu);

    return _contextMenu;
}

void CalenhadUi::setEditActionStatus (CalenhadModel* model) {
    _copyAction -> setEnabled (! model -> selectedItems().empty());
    _cutAction -> setEnabled (! model -> selectedItems().empty());
    _deleteAction -> setEnabled (! model -> selectedItems().empty());
    _zoomSelectionAction -> setEnabled (! model -> selectedItems().empty());
    QString xml = QGuiApplication::clipboard()  -> text();
    QDomDocument doc;
    _pasteAction -> setEnabled (doc.setContent (xml));
}

QAction* CalenhadUi::createAction (const QIcon& icon, const QString& name, const QString& statusTip, const QKeySequence& shortcut) {
    QAction* action = new QAction (icon, name, this);
    action -> setStatusTip (statusTip);
    return action;
}

QMenuBar *CalenhadUi::getMenuBar() {
    return _menuBar;
}



void CalenhadUi::makeRecentFilesMenu() {
    QStringList files = CalenhadServices::recentFiles();
    _openRecentMenu -> clear();
    for (const QString& entry : files) {
        QFile f (entry);
        QDomDocument doc;
        doc.setContent (&f);
        QDomElement metadataElement = doc.documentElement().firstChildElement ("metadata");
        QDomElement titleElement = metadataElement.firstChildElement ("title");
        QString title = titleElement.text ();
        title += " (" + entry + ")";
        QAction* action = new QAction();
        action -> setText (title);
        _openRecentMenu -> addAction (action);
        connect (action, &QAction::triggered, this, [=] () { _app -> openProject (entry); });
    }
}


void CalenhadUi::makeActions() {

// add module actions

    QStringList types = CalenhadServices::modules() -> types();

    for (const QString& key :  types) {
        QAction* action = createAction (QIcon (":/icons/transform.png"), key, CalenhadServices::modules() -> description (key));
        _moduleGroup -> addAction (action);
        action -> setCheckable (true);
        action -> setData (key);
        connect (action, &QAction::toggled, _controller, &CalenhadController::toolSelected);
    }

// scale actions
    _zoomInAction = createAction (QIcon(":/appicons/controls/zoom_in.png"), tr("Zoom &in"), "Zoom in", QKeySequence::ZoomIn);
    connect (_zoomInAction, &QAction::triggered, _controller, &CalenhadController::zoomIn);
    _zoomOutAction = createAction (QIcon(":/appicons/controls/zoom_out.png"), tr("Zoom &out"), "Zoom out", QKeySequence::ZoomOut);
    connect (_zoomOutAction, &QAction::triggered, _controller, &CalenhadController::zoomOut);
    _zoomToFitAction = createAction (QIcon(":/appicons/controls/zoom_to_fit.png"), tr("Zoom to &fit"), "Zoom to fit");
    connect (_zoomToFitAction, &QAction::triggered, _controller, &CalenhadController::zoomToFit);
    _zoomSelectionAction = createAction (QIcon(":/appicons/controls/zoom.png"), tr("Zoom to &selection"), "Zoom to selection");
    connect (_zoomSelectionAction, &QAction::triggered, _controller, &CalenhadController::zoomToSelection);
    // grid visible toggle on / off
    _gridAction = createAction(QIcon(":/appicons/controls/grid.png"), tr("Toggle grid"), "Toggle grid");
    _gridAction -> setCheckable (true);

    connect (_gridAction, &QAction::triggered, _controller, &CalenhadController::toggleGrid);
    // grid snapping on / off
    _snapAction = createAction (QIcon(":/appicons/controls/grid-snap.png"), tr("Snap to grid"), "Snap to grid");
    _snapAction -> setCheckable (true);

    connect (_snapAction, &QAction::triggered, _controller, &CalenhadController::snapToGrid);
    // module tree
    _moduleTreeAction = createAction (QIcon(":/appicons/controls/tree.png"), tr("Module tree"), "Module tree");
    _moduleTreeAction -> setCheckable (true);
    _moduleTreeAction -> setChecked (false);
    connect (_moduleTreeAction, &QAction::triggered, _controller, &CalenhadController::moduleTree);
// undo/redo apparatus

    _undoAction = createAction  (QIcon (":/appicons/controls/undo.png"), tr("Undo"), "Undo", QKeySequence::Undo);
    _redoAction = createAction  (QIcon (":/appicons/controls/redo.png"), tr("Redo"), "Redo", QKeySequence::Redo);
    _undoAction -> setEnabled (false);
    _redoAction -> setEnabled (false);
    connect (_controller, &CalenhadController::canUndoChanged, this, [=]() { _undoAction -> setEnabled (_controller -> canUndo()); });
    connect (_controller, &CalenhadController::canRedoChanged, this, [=]() { _redoAction -> setEnabled (_controller -> canRedo()); });
    connect (_undoAction, &QAction::triggered, _controller, &CalenhadController::undo);
    connect (_redoAction, &QAction::triggered, _controller, &CalenhadController::redo);
    _newAction = createAction  (QIcon(":/appicons/controls/new.png"), tr("&New"), "Start a new project", QKeySequence::New);
    connect (_newAction, &QAction::triggered, _app, &Calenhad::newProject);
    _closeAction = createAction (QIcon(":/appicons/controls/close.png"), tr("&Close"), "Start a new project", QKeySequence::New);
    connect (_closeAction, &QAction::triggered, _app, &Calenhad::closeProject);
    _quitAction = createAction (QIcon(":/appicons/controls/quit.png"), tr("&Quit"), "Quit the application", QKeySequence::Quit);
    connect (_quitAction, &QAction::triggered, _app, &Calenhad::quit);
    _openAction = createAction (QIcon(":/appicons/controls/open_file.png"), tr("&Open"), "Open a Calenhad model file", QKeySequence::Open);
    connect (_openAction, &QAction::triggered, _app, &Calenhad::open);
    _importAction = createAction (QIcon(":/appicons/controls/import_file.png"), tr("&Open"), "Import a Calenhad model file into this project", QKeySequence::AddTab);
    connect (_importAction, &QAction::triggered, this, [=]() { _app -> loadFile (CalenhadFileType::CalenhadModelFile); });

    _saveAction = createAction (QIcon(":/appicons/controls/save.png"), tr("&Save"), "Save model", QKeySequence::Save);
    connect (_saveAction, &QAction::triggered, _app, &Calenhad::saveFile);
    _saveAsAction = createAction (QIcon(":/appicons/controls/save_as.png"), tr("&Save as..."), "Save model in a new file", QKeySequence::SaveAs);
    connect (_saveAsAction, &QAction::triggered, this, [=]() { _app -> saveFileAs (CalenhadFileType::CalenhadModelFile); });
    _loadLegendsAction = createAction (QIcon(":/appicons/controls/loadLegends.png"), "Load legends", "Import legends from a Calenhad file");
    connect (_loadLegendsAction, &QAction::triggered, this, [=]() { _app -> loadFile (CalenhadFileType::CalenhadLegendFile); });
    _saveLegendsAction = createAction (QIcon(":/appicons/controls/loadLegends.png"), "Save legends", "Export legends to a separate file");
    connect (_saveLegendsAction, &QAction::triggered, this, [=]() { _app -> saveFileAs (CalenhadFileType::CalenhadLegendFile); });

    _projectPropertiesAction = createAction (QIcon(":/appicons/controls/properties.png"), tr("Properties..."), "Project properties", QKeySequence::Preferences);
    connect (_projectPropertiesAction, &QAction::triggered, _app, &Calenhad::projectProperties);

    _selectModeAction = createAction (QIcon(":/appicons/controls/select.png"), tr("Select mode"), "Select mode");
    _selectModeAction -> setCheckable (true);
    _selectModeAction -> setChecked (true);
    connect (_selectModeAction, &QAction::toggled, _app, [=] () { _app -> model() -> setMouseMode (QGraphicsView::DragMode::RubberBandDrag); });
    _mouseModeGroup -> addAction (_selectModeAction);
    _panModeAction = createAction (QIcon(":/appicons/controls/pan.png"), tr("Pan mode"), "Pan mode");
    _panModeAction -> setCheckable(true);
    _panModeAction -> setChecked (false);
    connect (_panModeAction, &QAction::toggled, _app, [=] () { _app -> model() -> setMouseMode (QGraphicsView::DragMode::ScrollHandDrag); });
    _mouseModeGroup -> addAction (_panModeAction);


    _xmlAction = createAction (QIcon (":/appicons/controls/xml.png"), tr("&XML"), "View model as an XML file", QKeySequence::NativeText);
    connect (_xmlAction, &QAction::triggered, _controller, &CalenhadController::showXml);

    _manageLegendsAction = createAction (QIcon(":/appicons/controls/legend.png"), tr("&Legends"), "Manage the list of map legends");
    connect (_manageLegendsAction, &QAction::triggered, _app, &Calenhad::manageLegends);

    _cutAction = createAction (QIcon(":/appicons/controls/cut.png"), tr("Cut"), "Cut selection to the clipboard", QKeySequence::Cut);
    _cutAction -> setEnabled(false);
    connect (_cutAction, &QAction::triggered, this, [=] () { _controller -> editAction (true, true); });

    _copyAction = createAction (QIcon(":/appicons/controls/copy.png"), tr("Copy"), "Copy selection to the clipboard", QKeySequence::Copy);
    _copyAction -> setEnabled(false);
    connect (_copyAction, &QAction::triggered, this, [=] () { _controller -> editAction (false, true); });

    _pasteAction = createAction (QIcon(":/appicons/controls/paste.png"), tr("Paste"), "Paste selection from the clipboard", QKeySequence::Paste);
    _pasteAction -> setEnabled(false);
    connect (_pasteAction, &QAction::triggered, _controller, &CalenhadController::paste);
    connect (QGuiApplication::clipboard(), &QClipboard::dataChanged, this, [=] () { setEditActionStatus (_controller -> model()); });

    _deleteAction = createAction (QIcon(":/appicons/controls/delete_selection.png"), tr("Delete selection"), "Delete selection");
    _deleteAction -> setEnabled (false);
    connect (_deleteAction, &QAction::triggered, this, [=] () { _controller -> editAction (true, false); });

    // Dialogs
    QDialog* _variablesDialog = new VariablesDialog();
    _variablesAction = createAction (QIcon(":/appicons/controls/variables.png"), tr("&Variables"), "Review and edit module parameter variables", QKeySequence());
    connect (_variablesAction, &QAction::triggered, _variablesDialog, &VariablesDialog::exec);

    _newWindowAction = createAction (QIcon (":/appicons/controls/window_new.png"), tr ("New window"), "Open a new window onto the model", QKeySequence());
    connect (_newWindowAction, &QAction::triggered, _controller, &CalenhadController::newWindow);

    _closeWindowAction = createAction (QIcon (":/appicons/controls/window_close.png"), tr ("Close window"), "Close the active window onto the model", QKeySequence());
    connect (_closeWindowAction, &QAction::triggered, _controller, &CalenhadController::closeWindow);

    _tileWindowsAction = createAction (QIcon (":/appicons/controls/windows_tile.png"), tr ("Tile windows"), "Arrange the open windows in tiles", QKeySequence());
    connect (_tileWindowsAction, &QAction::triggered, _controller, &CalenhadController::tileWindows);

    _cascadeWindowsAction = createAction (QIcon (":/appicons/controls/windows_cascade.png"), tr ("Tile windows"), "Arrange the open windows in a cascade", QKeySequence());
    connect (_cascadeWindowsAction, &QAction::triggered, _controller, &CalenhadController::cascadeWindows);


}

void CalenhadUi::connectView(nodeedit::CalenhadView *view) {
    connect (view, &CalenhadView::zoomInRequested, _zoomInAction, &QAction::trigger);
    connect (view, &CalenhadView::zoomOutRequested, _zoomOutAction, &QAction::trigger);
    connect (view, &CalenhadView::viewZoomed, this, &CalenhadUi::updateZoomActions);
}

void CalenhadUi::setActive (QWidget* widget, const bool& enabled) {
    if (widget -> parent()) {
        QWidget* p = dynamic_cast<QWidget*> (widget -> parent());
        if (p) {
            p -> setEnabled (enabled);
            setActive (p, enabled);
        }
    }
}

void CalenhadUi::clearTools() {
    QAction* checked = _moduleGroup -> checkedAction();
    if (checked) checked -> setChecked (false);
}


void CalenhadUi::updateZoomActions() {
    if (! _controller -> views().isEmpty ()) {
        CalenhadView* view = _controller -> activeView ();
        if (view) {
            _snapAction->setChecked (view -> gridVisible ());
            _gridAction->setChecked (view -> gridVisible ());
            double z = view -> currentZoom ();
            _zoomInAction -> setEnabled (z < CalenhadServices::preferences() -> calenhad_desktop_zoom_limit_zoomin); // 4
            _zoomOutAction -> setEnabled (z > CalenhadServices::preferences() -> calenhad_desktop_zoom_limit_zoomout); // 0.025);
        } else {
            _zoomInAction -> setEnabled (false);
            _zoomOutAction -> setEnabled (false);
        }
    }
}
