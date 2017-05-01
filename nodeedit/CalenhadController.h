

#ifndef QNODESEDITOR_H
#define QNODESEDITOR_H

#include <QObject>
#include <QToolBar>
#include <iostream>
#include <QUndoStack>
#include "Calenhad.h"


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


	explicit CalenhadController (Calenhad* parent = 0);
	virtual ~CalenhadController();
	void setModel (CalenhadModel* scene);
    CalenhadModel* model();
    QList<CalenhadView*>* views();
    void addView (CalenhadView* view);
	void doCommand (QUndoCommand* c);
	void addParamsWidget (QToolBar* toolbar, QNode* node);
    void addMenus (QMenuBar* menuBar);
    QMenu* getContextMenu (QGraphicsItem* item);
    QMenu* getContextMenu();
    QMenu* getContextMenu (QModule* module);
    void setSelectionActionsEnabled (const bool& enabled);
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
	//void makeContextMenus ();

	QAction* undoAction, * redoAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* zoomToFitAction;
    QAction* zoomSelectionAction;
    QAction* deleteConnectionAction;
    QAction* deleteModuleAction;
	QAction* deleteSelectionAction;
    QAction* duplicateModuleAction;

    QAction* createTool (const QString& caption, const QString& statusTip, const QVariant& id, ToolDrawer* drawer, const bool& toggle = false);


};

#endif // QNODESEDITOR_H
