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
#include "NodeBlock.h"
#include "Toolbox.h"
#include "Connection.h"
#include "../actions/CommandGroup.h"
#include "../actions/DuplicateNodeCommand.h"
#include "../CalenhadServices.h"
#include "../actions/DeleteNodeCommand.h"
#include "qmodule/NodeGroup.h"
#include "Calenhad.h"
#include "../preferences/PreferencesService.h"
#include <QGraphicsScene>
#include "../nodeedit/CalenhadView.h"
#include "../pipeline/CalenhadModel.h"
#include "../pipeline/ModuleFactory.h"
#include "Port.h"
#include "qmodule/Module.h"
#include <QAction>
#include <actions/ContextAction.h>
#include <actions/SelectionToClipboardCommand.h>
#include <actions/PasteCommand.h>
#include <QtGui/QGuiApplication>
#include <QClipboard>

using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::actions;

CalenhadController::CalenhadController (Calenhad* parent) : QObject (parent), _views (new QList<CalenhadView*>()), _undoStack (new QUndoStack()) {
    connect (_undoStack, &QUndoStack::canUndoChanged, this, &CalenhadController::canUndoChanged);
    connect (_undoStack, &QUndoStack::canRedoChanged, this, &CalenhadController::canRedoChanged);

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

}

void CalenhadController::setModel (CalenhadModel* model) {
    _model = model;
    connect (model, &QGraphicsScene::selectionChanged, this, [=] () {
        ((Calenhad*) parent()) -> setSelectionActionsEnabled (! (model -> selectedItems().isEmpty()));
    });

    connect (model, SIGNAL (showMessage (QString)), this, SLOT (showMessage (QString)));
}

CalenhadModel* CalenhadController::model () {
    return _model;
}

void CalenhadController::toolSelected (bool state) {
    QAction* tool = (QAction*) sender();
    if (state) {
        // activated
        _model -> setActiveTool (tool);
        int iconSize = CalenhadServices::preferences() -> calenhad_toolpalette_icon_size;
        for (QGraphicsView* view : _model -> views ()) {
            QPixmap* pixmap = CalenhadServices::modules() -> getIcon (tool -> data().toString ());
            QCursor cursor = QCursor ((*pixmap).scaled (iconSize, iconSize));
            view -> viewport () -> setCursor (cursor);
            for (NodeGroup* group : _model -> nodeGroups ()) {
                group -> handle() -> setCursor (cursor);
            }
        }
    } else {
        _model -> setActiveTool (nullptr);
        for (QGraphicsView* view : _model -> views ()) {
            view -> setDragMode (QGraphicsView::RubberBandDrag);
        }
    }
}

void CalenhadController::showMessage (QString message) {
    CalenhadServices::messages() -> message ("info", message);
}

void CalenhadController::actionTriggered() {
    // fire the selected action
    QAction* action = (QAction*) sender();
    ContextAction<QGraphicsItem>* ca = dynamic_cast<ContextAction<QGraphicsItem>*> (action);
    if (ca) {
        if (action->data () == CalenhadAction::DeleteConnectionAction) { doCommand (new DeleteConnectionCommand (static_cast<Connection*> (ca -> context()), _model)); }
        if (action->data () == CalenhadAction::DeleteModuleAction) { doCommand (new DeleteNodeCommand ((static_cast<NodeBlock*> (ca -> context())) -> node (), _model)); }
        if (action->data () == CalenhadAction::DuplicateModuleAction) { doCommand (new DuplicateNodeCommand ((static_cast<NodeBlock*> (ca -> context())) -> node(), _model)); }
    }
    if (action -> data() == CalenhadAction::ZoomInAction) { doCommand (new ZoomCommand (0.1, _views -> at (0))); }
    if (action -> data() == CalenhadAction::ZoomOutAction) { doCommand (new ZoomCommand (-0.1,  _views -> at (0))); }
    if (action -> data() == CalenhadAction::ZoomToFitAction) { doCommand (new ZoomToFitCommand ( _views -> at (0))); }
    if (action -> data() == CalenhadAction::ZoomToSelectionAction) { doCommand (new ZoomToSelectionCommand ( _views -> at (0))); }
    if (action -> data() == CalenhadAction::PasteAction) { doCommand (new PasteCommand (_model)); }
    if (action -> data() == CalenhadAction::DeleteSelectionAction || action -> data() == CalenhadAction::CutAction || action -> data() == CalenhadAction::CopyAction) {
        CommandGroup* group = new CommandGroup ();


        if (action -> data() == CalenhadAction::CutAction || action -> data() == CalenhadAction::DeleteSelectionAction) {
            for (QGraphicsItem* item : _model->selectedItems ()) {
                // to do - delete other kinds of node
                if (item->type () == QGraphicsItem::UserType + 3) { // block
                    Node* node = ((NodeBlock*) item)->node ();
                    // to do - generalise this to delete groups too
                    Module* module = dynamic_cast<Module*> (node);
                    if (module) {
                        DeleteNodeCommand* command = new DeleteNodeCommand (module, _model);
                        group->addCommand (command);
                    }
                }
            }
        }


        if (action -> data() == CalenhadAction::CutAction || action -> data() == CalenhadAction::CopyAction) {
            SelectionToClipboardCommand* command = new SelectionToClipboardCommand (_model);
            group -> addCommand (command);
        }

        doCommand (group);
        ((Calenhad*) parent()) -> setSelectionActionsEnabled (! (_model -> selectedItems().isEmpty()));
    }

    if (action -> data() == CalenhadAction::UndoAction) { _undoStack -> undo(); }
    if (action -> data() == CalenhadAction::RedoAction) { _undoStack -> redo(); }

}

void CalenhadController::doCommand (QUndoCommand* c) {
    _undoStack->push (c);
    showMessage (c -> text());
}

void CalenhadController::addParamsWidget (QToolBar* toolbar, Node* node) {
    Calenhad* mainWindow = (Calenhad*) parent();
    mainWindow -> addToolbar (toolbar, node);
}

bool CalenhadController::canUndo () {
    return _undoStack -> canUndo();
}


bool CalenhadController::canRedo () {
    return _undoStack -> canRedo();
}
