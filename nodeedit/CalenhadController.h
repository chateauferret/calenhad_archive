

#ifndef QNODESEDITOR_H
#define QNODESEDITOR_H

#include <QObject>
#include <QToolBar>
#include <iostream>
#include <QUndoStack>
#include <QPointF>
#include <QGraphicsItem>
#include <QtWidgets/QMenuBar>

namespace calenhad {
	namespace nodeedit {
		class QNEConnection;
		class QNodeBlock;
	}
	namespace pipeline {
		class CalenhadModel;
	}
	namespace qmodule {
		class QNode;
		class QModule;
	}
	namespace nodeedit {
		class CalenhadView;
		class Calenhad;
		class ToolDrawer;
		class ToolGroup;

		class CalenhadController : public QObject {
		Q_OBJECT


		public:


			explicit CalenhadController (Calenhad* parent = 0);

			virtual ~CalenhadController ();

			void setModel (calenhad::pipeline::CalenhadModel* scene);

			calenhad::pipeline::CalenhadModel* model ();

			QList<CalenhadView*>* views ();

			void addView (CalenhadView* view);

			void doCommand (QUndoCommand* c);

			void addParamsWidget (QToolBar* toolbar, calenhad::qmodule::QNode* node);

			void addMenus (QMenuBar* menuBar);

			QMenu* getContextMenu (QGraphicsItem* item);

			QMenu* getContextMenu ();

			QMenu* getContextMenu (calenhad::qmodule::QNode* node);

			void setSelectionActionsEnabled (const bool& enabled);

			QWidget* nodeRoster ();


		public slots:

			void toolSelected (bool);

			void showMessage (QString message);

			void clearTools ();

			void actionTriggered ();


		private:
			QList<CalenhadView*>* _views;
			ToolDrawer* _viewDrawer;
			ToolDrawer* _editDrawer;
			QUndoStack* _undoStack;
			ToolDrawer* _addModuleDrawer;
			ToolGroup* _addModuleGroup;
			calenhad::pipeline::CalenhadModel* _model;
			QGraphicsItem* _contextItem;
			calenhad::qmodule::QNode* _contextNode;

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

			QWidget* _nodeRoster;
		};

	}
}

#endif // QNODESEDITOR_H
