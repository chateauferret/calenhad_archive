//
// Created by martin on 18/10/18.
//

#ifndef MESSAGES_SNAPTOGRIDACTION_H
#define MESSAGES_SNAPTOGRIDACTION_H


#include <QtWidgets/QUndoCommand>

namespace calenhad {
    namespace nodeedit {
        class CalenhadView;
    }
    namespace actions {
        class SnapToGridCommand : public QUndoCommand {

        public:
            SnapToGridCommand (calenhad::nodeedit::CalenhadView* view);

            virtual ~SnapToGridCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::nodeedit::CalenhadView* _view;
        };


    }
}

#endif //MESSAGES_SNAPTOGRIDACTION_H
