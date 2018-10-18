//
// Created by martin on 17/10/18.
//

#ifndef MESSAGES_GRIDACTION_H
#define MESSAGES_GRIDACTION_H


#include <QUndoCommand>
#include <QGraphicsItem>
#include <QList>

namespace calenhad {
    namespace nodeedit {
        class CalenhadView;
    }
    namespace actions {
        class GridCommand : public QUndoCommand {

        public:
            GridCommand (calenhad::nodeedit::CalenhadView* view);

            virtual ~GridCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            double _delta, _previousZoom;
            calenhad::nodeedit::CalenhadView* _view;
        };


    }
}
#endif //MESSAGES_GRIDACTION_H
