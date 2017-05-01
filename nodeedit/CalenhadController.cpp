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

#include "CalenhadController.h"
#include "../actions/ZoomCommand.h"
#include "../actions/DeleteConnectionCommand.h"
#include "../actions/DeleteModuleCommand.h"
#include "Calenhad.h"
#include "qneblock.h"
#include "qnetoolbox.h"
#include "qneconnection.h"
#include "../qmodule/QModule.h"
#include "../actions/CommandGroup.h"
#include "../actions/DuplicateModuleCommand.h"
#include "../CalenhadServices.h"

CalenhadController::CalenhadController (Calenhad* parent) : QObject (parent), _views (new QList<CalenhadView*>()) {

    // tool groups - activating a toggle action deactivates all others in the same group (like a radio button)
    _addModuleGroup = new ToolGroup();
    Calenhad::toolbox -> addGroup (_addModuleGroup);

    // tool drawers - tools in the same drawer are grouped in the UI
    _addModuleDrawer = new ToolDrawer ("Modules");
    Calenhad::toolbox -> addDrawer (_addModuleDrawer);
    _viewDrawer = new ToolDrawer ("View");
    Calenhad::toolbox -> addDrawer (_viewDrawer);
    _editDrawer = new ToolDrawer ("Edit");
    Calenhad::toolbox -> addDrawer (_editDrawer);

    // create context menus
    _connectionContextMenu = new QMenu ("Connection");
    _moduleContextMenu = new QMenu ("Module");
    _outputPortContextMenu = new QMenu ("Output");
    _inputPortContextMenu = new QMenu ("Input");
    _defaultContextMenu = new QMenu ("Menu");
    _addModuleMenu = new QMenu ("Add module");

//    _connectionContextMenu -> addMenu (connectInputMenu());
//    _connectionContextMenu -> addMenu (connectOutputMenu());
//    _outputPortContextMenu -> addMenu (connectInputMenu());
//    _inputPortContextMenu -> addMenu (connectOutputMenu());

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
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_cache, "Cache value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_checkerboard, "Checkerboard pattern");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_invert, "Invert values");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_max, "Maximum value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_min, "Minimum value");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_voronoi, "Voronoi pattern");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_select, "Select input");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_turbulence, "Turbulence");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_icospheremap, "Icosphere map");
    addModuleTool (CalenhadServices::preferences() -> calenhad_module_altitudemap, "Altitude map");

    _zoomMenu = new QMenu ("Zoom");
    _defaultContextMenu -> addMenu (parent -> toolbox -> menu ("Modules"));
    _defaultContextMenu -> addMenu (_zoomMenu);

    // zoom actions
    zoomInAction = createTool (tr ("Zoom &in"), "Zoom in", CalenhadAction::ZoomInAction, _viewDrawer);
    zoomOutAction = createTool (tr ("Zoom &out"), "Zoom out", CalenhadAction::ZoomOutAction, _viewDrawer);
    zoomToFitAction = createTool (tr ("Zoom to &fit"), "Zoom to fit", CalenhadAction::ZoomToFitAction, _viewDrawer);
    zoomSelectionAction = createTool (tr ("Zoom to &selection"), "Zoom to selection", CalenhadAction::ZoomToSelectionAction, _viewDrawer);
    _zoomMenu -> addAction (zoomInAction);
    _zoomMenu -> addAction (zoomOutAction);
    _zoomMenu -> addAction (zoomToFitAction);
    _zoomMenu -> addAction (zoomSelectionAction);

    // undo/redo apparatus
    _undoStack = new QUndoStack();
    undoAction = createTool (tr ("Undo"), "Undo", CalenhadAction::UndoAction, _editDrawer);
    redoAction = createTool (tr ("Redo"), "Redo", CalenhadAction::RedoAction, _editDrawer);
    undoAction -> setEnabled (_undoStack -> canUndo());
    redoAction -> setEnabled (_undoStack -> canRedo());
    _defaultContextMenu -> addAction (undoAction);
    _defaultContextMenu -> addAction (redoAction);
    connect (_undoStack, &QUndoStack::canUndoChanged, this, [=] () { undoAction -> setEnabled (_undoStack -> canUndo()); });
    connect (_undoStack, &QUndoStack::canRedoChanged, this, [=] () { redoAction -> setEnabled (_undoStack -> canRedo()); });

    // connection actions
    deleteConnectionAction = createTool (tr ("Delete connection"), "Delete connection", CalenhadAction::DeleteConnectionAction, _editDrawer);
    _connectionContextMenu -> addAction (deleteConnectionAction);

    // other module actions
    deleteModuleAction = createTool (tr ("Delete module"), "Delete module", CalenhadAction::DeleteModuleAction, _editDrawer);
    deleteSelectionAction = createTool (tr ("Delete selection"), "Delete selection", CalenhadAction::DeleteSelectionAction, _editDrawer);
    deleteSelectionAction -> setEnabled (false);
    _moduleContextMenu -> addAction (deleteModuleAction);
    _moduleContextMenu -> addAction (deleteSelectionAction);

    duplicateModuleAction = createTool (tr ("Duplicate module"), "Duplicate module", CalenhadAction::DuplicateModuleAction, _editDrawer);
    _moduleContextMenu -> addAction (duplicateModuleAction);
}


QAction* CalenhadController::createTool (const QString& caption, const QString& statusTip, const QVariant& id, ToolDrawer* drawer, const bool& toggle) {
    QAction* tool = new QAction (caption, this);
    tool -> setStatusTip (statusTip);
    tool -> setCheckable (toggle);
    tool -> setData (id);

    drawer -> addTool (tool);
    if (toggle) {
        connect (tool, SIGNAL (toggled (bool)), this, SLOT (toolSelected (bool)));
    } else {
        connect (tool, SIGNAL (triggered()), this, SLOT (actionTriggered ()));
    }
    return tool;
}

void CalenhadController::addView (CalenhadView* view) {
    _views -> append (view);
}

QList<CalenhadView*>* CalenhadController::views() {
    return _views;
}

CalenhadController::~CalenhadController() {
    if (_views) {
        delete _views;
    }
    delete _addModuleGroup;
    delete _viewDrawer;
    delete _editDrawer;
    delete _addModuleDrawer;
}

// to do - provide further lookup for name to localisation file
void CalenhadController::addModuleTool (const QString& name, const QString& tooltip) {
    QAction* tool = createTool (name, tooltip, name, _addModuleDrawer, true);
    tool -> setCheckable (true);
    _addModuleGroup -> addTool (tool);
}

void CalenhadController::setModel (CalenhadModel* s) {
    _model = s;
    connect (s, &QGraphicsScene::selectionChanged, this, [=] () {
        setSelectionActionsEnabled (! (s -> selectedItems().isEmpty()));
    });

    connect (s, SIGNAL (showMessage (QString)), this, SLOT (showMessage (QString)));
}

CalenhadModel* CalenhadController::model () {
    return _model;
}

void CalenhadController::clearTools() {
    _addModuleGroup -> toolToggled (false);
}

void CalenhadController::toolSelected (bool state) {
    QAction* tool = (QAction*) sender();
    if (state) {
        // activated
        _model -> setActiveTool (tool);
        for (QGraphicsView* view : _model -> views ()) {
            view -> viewport () -> setCursor (Qt::CrossCursor);
            view -> setDragMode (QGraphicsView::NoDrag);
        }
    } else {
        _model -> setActiveTool (nullptr);
        for (QGraphicsView* view : _model -> views ()) {
            view -> setDragMode (QGraphicsView::RubberBandDrag);
        }
    }
}

void CalenhadController::showMessage (QString message) {
    CalenhadServices::messages() -> message ("", message);
}

QMenu* CalenhadController::getContextMenu (QGraphicsItem* item) {
    _contextItem = item;
    if (! item) {
        return _defaultContextMenu;
    }
    else if (item -> type() == QNEConnection::Type) {
        return _connectionContextMenu;
    }
    else if (item -> type() == QNEBlock::Type) {
        QNEBlock* handle = (QNEBlock*) item;
        QModule* module = handle -> module();
        return getContextMenu (module);
    }
    else if (item -> type() == QNEPort::Type) {
        QNEPort* port = (QNEPort*) item;
        if (port -> portType() == QNEPort::OutputPort) {
            return _outputPortContextMenu;
        } else {
           return  _inputPortContextMenu;
        }
    }
    return _defaultContextMenu;
}

QMenu* CalenhadController::getContextMenu (QModule* module) {
    _contextModule = module;
    return _moduleContextMenu;
}

QMenu* CalenhadController::getContextMenu() {
    return _defaultContextMenu;
}

void CalenhadController::actionTriggered () {

    // fire the selected action
    QAction* action = (QAction*) sender();
    if (action -> data() == CalenhadAction::ZoomInAction) { doCommand (new ZoomCommand (0.1, _views -> at (0))); }
    if (action -> data() == CalenhadAction::ZoomOutAction) { doCommand (new ZoomCommand (-0.1,  _views -> at (0))); }
    if (action -> data() == CalenhadAction::ZoomToFitAction) { doCommand (new ZoomToFitCommand ( _views -> at (0))); }
    if (action -> data() == CalenhadAction::ZoomToSelectionAction) { doCommand (new ZoomToSelectionCommand ( _views -> at (0))); }
    if (action -> data() == CalenhadAction::DeleteConnectionAction) { doCommand (new DeleteConnectionCommand (static_cast<QNEConnection*> (_contextItem), _model)); }

    // to do - delete other kinds of node
    if (action -> data() == CalenhadAction::DeleteModuleAction) { doCommand (new DeleteModuleCommand (_contextModule,  _model)); }
    if (action -> data() == CalenhadAction::DuplicateModuleAction) { doCommand (new DuplicateModuleCommand (_contextModule,  _model)); }
    if (action -> data() == CalenhadAction::DeleteSelectionAction) {
        CommandGroup* group = new CommandGroup ();
        for (QGraphicsItem* item : _model->selectedItems ()) {
            // to do - delete other kinds of node
            if (item->type () == QGraphicsItem::UserType + 3) { // block
                DeleteModuleCommand* command = new DeleteModuleCommand (((QNEBlock*) item)->module (), _model);
                group->addCommand (command);
            }
        }
        doCommand (group);
    }

    if (action -> data() == CalenhadAction::UndoAction) { _undoStack -> undo(); }
    if (action -> data() == CalenhadAction::RedoAction) { _undoStack -> redo(); }

}

void CalenhadController::doCommand (QUndoCommand* c) {
    _undoStack->push (c);
    double z = _views->at (0)->currentZoom ();
    zoomInAction->setEnabled (z < 4.0);
    zoomOutAction->setEnabled (z > 0.025);
}

void CalenhadController::addParamsWidget (QToolBar* toolbar, QNode* node) {
    Calenhad* mainWindow = (Calenhad*) parent();
    mainWindow -> addToolbar (toolbar, node);
}

void CalenhadController::addMenus (QMenuBar* menuBar) {
    QMenu* editMenu = menuBar -> findChild<QMenu*>("editMenu");
    if (editMenu) {
        editMenu -> addAction (deleteSelectionAction);
        editMenu -> addSeparator();
        editMenu -> addAction (undoAction);
        editMenu -> addAction (redoAction);
    }

}

void CalenhadController::setSelectionActionsEnabled (const bool& enabled) {
    deleteSelectionAction -> setEnabled (enabled);
}
