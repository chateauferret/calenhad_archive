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

#ifndef QNODESEDITOR_H
#define QNODESEDITOR_H

#include <QObject>
#include <QToolBar>
#include <iostream>
#include <QUndoStack>



class QGraphicsScene;
class QNEConnection;
class QGraphicsItem;
class QPointF;
class QNEBlock;
class CalenhadView;
class CalenhadModel;
class QModule;
class ToolDrawer;
class ToolGroup;

class CalenhadController : public QObject {
Q_OBJECT
public:


	explicit CalenhadController (QObject* parent = 0);
	virtual ~CalenhadController();
	void setModel (CalenhadModel* scene);
    CalenhadModel* model();
    QList<CalenhadView*>* views();
    void addView (CalenhadView* view);

    QMenu* getContextMenu (QGraphicsItem* item);
    QMenu* getContextMenu();
    QMenu* getContextMenu (QModule* module);

public slots:
    void toolSelected (bool);
    void showMessage (QString message);
    void clearTools();
    void actionTriggered();

private:
    QList<CalenhadView*>* _views;
    ToolDrawer* _viewDrawer;
    ToolDrawer* _editDrawer;
    QUndoStack* _undoStack;
    ToolDrawer* _addModuleDrawer;
    ToolGroup* _addModuleGroup;
	CalenhadModel* _model;
    QGraphicsItem* _contextItem;
    QModule* _contextModule;
	void addModuleTool (const QString& name, const QString& tooltip);

	QMenu* _moduleContextMenu;
	QMenu* _connectionContextMenu;
	QMenu* _outputPortContextMenu;
	QMenu* _inputPortContextMenu;
	QMenu* _defaultContextMenu;
	QMenu* _zoomMenu;
	QMenu* _addModuleMenu;

	void makeContextMenus ();

	QAction* undoAction, * redoAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* zoomToFitAction;
    QAction* zoomSelectionAction;
    QAction* deleteConnectionAction;
    QAction* deleteModuleAction;

    QAction* createTool (const QString& caption, const QString& statusTip, const QVariant& id, ToolDrawer* drawer, const bool& toggle = false);
};

#endif // QNODESEDITOR_H
