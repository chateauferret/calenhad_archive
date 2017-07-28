//
// Created by martin on 23/01/17.
//

#ifndef CALENHAD_ZOOMACTION_H
#define CALENHAD_ZOOMACTION_H

#include <QUndoCommand>
#include <QGraphicsItem>
#include <QList>

namespace calenhad {
    namespace nodeedit {
        class CalenhadView;
    }
    namespace actions {
        class ZoomCommand : public QUndoCommand {

        public:
            ZoomCommand (double delta, calenhad::nodeedit::CalenhadView* view);

            virtual ~ZoomCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            double _delta, _previousZoom;
            calenhad::nodeedit::CalenhadView* _view;
        };

        class ZoomToFitCommand : public ZoomCommand {
        public:
            ZoomToFitCommand (calenhad::nodeedit::CalenhadView* view);

            void redo () override;

        protected:
            void zoomToFit (QList<QGraphicsItem*> list);
        };


        class ZoomToSelectionCommand : public ZoomToFitCommand {
        public:
            ZoomToSelectionCommand (calenhad::nodeedit::CalenhadView* view);

            void redo () override;
        };
    }
}
#endif //CALENHAD_ZOOMACTION_H
