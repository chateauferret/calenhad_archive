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
#include "NodeBlock.h"
#include "Connection.h"
#include "src/CalenhadServices.h"
#include "Calenhad.h"
#include "../preferences/PreferencesService.h"
#include <QGraphicsScene>
#include "../nodeedit/CalenhadView.h"
#include "../pipeline/CalenhadModel.h"
#include "../pipeline/ModuleFactory.h"
#include "Port.h"
#include "module/Module.h"
#include <QAction>
#include <actions/ContextAction.h>
#include <QtGui/QGuiApplication>
#include <QClipboard>
#include <actions/GridCommand.h>
#include <actions/SnapToGridCommand.h>
#include "../controls/ModuleTree.h"
#include "../CalenhadUi.h"
#include "../nodeedit/CalenhadToolBar.h"

using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;
using namespace calenhad::module;
using namespace calenhad::notification;
using namespace calenhad::actions;
using namespace calenhad::controls;

CalenhadController::CalenhadController (Calenhad* parent) : QObject (parent),
    _ui (new CalenhadUi (this)),
    _views (new QList<CalenhadView*>()),
    _undoStack (new QUndoStack()),
    _model (nullptr),
    _moduleTree (nullptr) {
    connect (_undoStack, &QUndoStack::canUndoChanged, this, &CalenhadController::canUndoChanged);
    connect (_undoStack, &QUndoStack::canRedoChanged, this, &CalenhadController::canRedoChanged);
    _ui -> initialise();
}


void CalenhadController::addView (CalenhadView* view) {
    _views -> append (view);
    _ui -> connectView (view);
}

QList<CalenhadView*>* CalenhadController::views() {
    return _views;
}

CalenhadController::~CalenhadController() {
    delete _views;
}

void CalenhadController::setModel (CalenhadModel* model) {
    _model = model;
    connect (_model, &QGraphicsScene::selectionChanged, this, [=] () {
        _ui -> setEditActionStatus (_model);
    });
    updateZoomActions();
    setEditActionStatus();
}


CalenhadModel* CalenhadController::model() {
    return _model;
}

void CalenhadController::toolSelected() {
    QAction* tool = (QAction*) sender();
    if (tool -> isChecked()) {
        // activated
        _model -> setActiveTool (tool);
        for (QGraphicsView* view : _model -> views()) {
            QPixmap* pixmap = CalenhadServices::modules() -> getIcon (tool -> data().toString ());
            QCursor cursor = QCursor (*pixmap);
            view -> viewport() -> setCursor (cursor);
        }
    } else {
        _model -> setActiveTool (nullptr);
        for (QGraphicsView* view : _model -> views()) {
            view -> setDragMode (QGraphicsView::RubberBandDrag);
        }
    }
}

void CalenhadController::zoomIn() {
    if (_views -> at (0) -> currentZoom() < CalenhadServices::preferences() -> calenhad_desktop_zoom_limit_zoomin) {
        doCommand (new ZoomCommand (0.1, _views->at (0)));
    }
}

void CalenhadController::zoomOut() {
    if (_views->at(0)->currentZoom() > CalenhadServices::preferences()->calenhad_desktop_zoom_limit_zoomout) {
        doCommand(new ZoomCommand(-0.1, _views->at(0)));
    }
}

void CalenhadController::zoomToFit() {
    doCommand (new ZoomToFitCommand ( _views -> at (0)));
}

void CalenhadController::zoomToSelection() {
    doCommand (new ZoomToSelectionCommand ( _views -> at (0)));
}

void CalenhadController::toggleGrid() {
    doCommand (new GridCommand (_views -> at (0)));
    _ui -> updateZoomActions();
}

void CalenhadController::snapToGrid() {
    doCommand (new SnapToGridCommand (_views -> at (0)));
}

void CalenhadController::moduleTree() {
    if (!_moduleTree) {
        _moduleTree = new ModuleTree (_model);

        // keep the action's state consistent if we open or close the tree by other means
        connect (_moduleTree, &ModuleTree::treeShown, this, [=] () {  ((QAction*) sender()) -> setChecked (true); });
        connect (_moduleTree, &ModuleTree::treeHidden, this, [=] () {  ((QAction*) sender()) -> setChecked (false); });
    }
    _moduleTree -> setVisible (!_moduleTree->isVisible ());
}

void CalenhadController::paste() {
    QClipboard* clipboard = QGuiApplication::clipboard ();
    QString xml = clipboard -> text();
    QDomDocument doc;
    if (doc.setContent (xml)) {
        //std::cout << xml.toStdString () << "\n";
        _model -> preserve();
        _model -> inflate (doc, CalenhadFileType::CalenhadModelFragment);
        _model -> setChanged (true);
        _model -> setRestorePoint();
    }
    _ui -> setEditActionStatus (_model);
}

void CalenhadController::editAction (const bool& kill, const bool& yank) {
    QString xml = QString::null;
    if (yank) {
        xml = _model -> selectionToXml();
        QClipboard* clipboard = QGuiApplication::clipboard ();
        clipboard -> setText (xml);
    }

    if (kill) {
        _model -> preserve();
        _model -> setUndoEnabled (false);
        for (QGraphicsItem* item : _model -> selectedItems ()) {
            // to do - delete other kinds of node
            if (item -> type () == QGraphicsItem::UserType + 3) { // block
                Node* node = ((NodeBlock*) item)->node ();
                // to do - generalise this to delete groups too
                Module* module = dynamic_cast<Module*> (node);
                if (module) {
                    _model -> doDeleteNode (module);
                }
            }
        }
        _model -> setChanged (true);
        _model -> setUndoEnabled (true);
        _model -> setRestorePoint();
        _ui -> setEditActionStatus (_model);
    }

}

void CalenhadController::undo() {
    if (_undoStack->canUndo()) {
        _undoStack->undo();
    }
}

void CalenhadController::redo() {
    if (_undoStack -> canRedo()) {
        _undoStack -> redo();
    }
}


void CalenhadController::doCommand (QUndoCommand* c) {
    _undoStack -> push (c);
}

void CalenhadController::addParamsWidget (QToolBar* toolbar, Module *module) {
    Calenhad* mainWindow = (Calenhad*) parent();
    mainWindow -> addToolbar (toolbar, module);
}

bool CalenhadController::canUndo () {
    return _undoStack -> canUndo();
}

bool CalenhadController::canRedo () {
    return _undoStack -> canRedo();
}

void CalenhadController::clearUndo () {
    _undoStack -> clear();
    emit canUndoChanged();
    emit canRedoChanged();
}

void CalenhadController::disconnect (Connection *connection) {
    _model -> doDisconnectPorts (connection);
}

void CalenhadController::duplicateNode (Node *node) {
    _model -> doDuplicateNode (node);
}

void CalenhadController::deleteNode (Node *node) {
    _model -> doDeleteNode (node);
}


void CalenhadController::rememberFile (const QString& file) {
    QStringList files = CalenhadServices::recentFiles();
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
    _ui -> makeRecentFilesMenu ();
}

QMenu *CalenhadController::getContextMenu(QGraphicsItem *item) {
    return _ui -> makeContextMenu (item);
}

void CalenhadController::clearTools() {
    _ui -> clearTools();
}

QMenuBar *CalenhadController::getMenuBar() {
    return _ui -> getMenuBar();
}

void CalenhadController::updateZoomActions() {
    _ui -> updateZoomActions();
}

void CalenhadController::setEditActionStatus() {
    _ui -> setEditActionStatus (_model);
}

void CalenhadController::showXml() {
    _model -> showXml();
}

