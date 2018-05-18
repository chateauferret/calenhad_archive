

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
		class Connection;
		class NodeBlock;
	}
	namespace pipeline {
		class CalenhadModel;
	}
	namespace qmodule {
		class Node;
		class Module;
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

			void setModel (calenhad::pipeline::CalenhadModel* model);

			calenhad::pipeline::CalenhadModel* model ();

			QList<CalenhadView*>* views ();

			void addView (CalenhadView* view);

			void doCommand (QUndoCommand* c);

			void addParamsWidget (QToolBar* toolbar, calenhad::qmodule::Node* node);
            bool canUndo();
            bool canRedo();
			void clearUndo ();

		public slots:

			void toolSelected (bool);

            void actionTriggered();
        signals:
            void canUndoChanged();
            void canRedoChanged();

		protected:
			QUndoStack* _undoStack;

			QList<CalenhadView*>* _views;
            calenhad::pipeline::CalenhadModel* _model;


            QAction* _zoomInAction, * _zoomOutAction;
        };

	}
}

#endif // QNODESEDITOR_H
