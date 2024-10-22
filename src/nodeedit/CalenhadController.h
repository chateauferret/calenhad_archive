

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
    class CalenhadUi;
	namespace nodeedit {
		class Connection;
		class NodeBlock;
	}
	namespace pipeline {
		class CalenhadModel;
	}
	namespace module {
		class Node;
		class Module;
	}
	namespace controls {
		class ModuleTree;
	}
	namespace nodeedit {
		class CalenhadView;
		class Calenhad;


		class CalenhadController : public QObject {
		Q_OBJECT

        public:

			explicit CalenhadController (Calenhad* parent = nullptr);

			~CalenhadController () override;

			void setModel (calenhad::pipeline::CalenhadModel* model);

            void deleteNode(module::Node *node);

            QMenu *getContextMenu(QGraphicsItem *item);
            QMenuBar* getMenuBar();
            void updateZoomActions();
            void duplicateNode(module::Node *node);
            void clearTools();
            void disconnect(Connection *connection);

            calenhad::pipeline::CalenhadModel* model ();

			QList<CalenhadView*> views ();

			void addView (CalenhadView* view);

            CalenhadView* activeView ();
			void doCommand (QUndoCommand* c);
            void rememberFile(const QString &file);

            void addParamsWidget (QToolBar* toolbar, module::Module *module);
            bool canUndo();
            bool canRedo();
			void clearUndo ();


		public slots:

			void toolSelected();
            void zoomIn();
            void zoomOut();
            void zoomToFit();
            void zoomToSelection();
            void toggleGrid();
            void snapToGrid();
            void moduleTree();
            void paste();
            void editAction(const bool &kill, const bool &yank);
            void undo();
            void redo();
            void showXml();

            void newWindow();
            void closeWindow();
            void tileWindows();
            void cascadeWindows();

        signals:
            void canUndoChanged();
            void canRedoChanged();

		protected:
			QUndoStack* _undoStack;

            calenhad::pipeline::CalenhadModel* _model;
            CalenhadUi* _ui;
            calenhad::controls::ModuleTree* _moduleTree;

            void setEditActionStatus();

        };
	}
}

#endif // QNODESEDITOR_H
