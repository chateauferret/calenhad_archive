

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

			void setModel (calenhad::pipeline::CalenhadModel* model);

			calenhad::pipeline::CalenhadModel* model ();

			QList<CalenhadView*>* views ();

			void addView (CalenhadView* view);

			void doCommand (QUndoCommand* c);

			void addParamsWidget (QToolBar* toolbar, calenhad::qmodule::QNode* node);
            bool canUndo();
            bool canRedo();

		public slots:

			void toolSelected (bool);

			void showMessage (QString message);

            void actionTriggered();
        signals:
            void canUndoChanged();
            void canRedoChanged();

		protected:
            QList<CalenhadView*>* _views;
            calenhad::pipeline::CalenhadModel* _model;

            QUndoStack* _undoStack;

		};

	}
}

#endif // QNODESEDITOR_H
