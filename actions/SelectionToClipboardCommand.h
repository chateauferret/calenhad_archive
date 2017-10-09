//
// Created by martin on 08/10/17.
//

#ifndef CALENHAD_SELECTIONTOCLIPBOARDCOMMAND_H
#define CALENHAD_SELECTIONTOCLIPBOARDCOMMAND_H

#include <QtWidgets/QUndoCommand>

namespace calenhad {
    namespace qmodule {
        class QNode;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace actions {
        class SelectionToClipboardCommand : public QUndoCommand {
        public:
            SelectionToClipboardCommand (calenhad::pipeline::CalenhadModel* _model);
            virtual ~SelectionToClipboardCommand();
            virtual void undo() override;
            virtual void redo() override;

        protected:
            calenhad::pipeline::CalenhadModel* _model;
            QString _oldXml;
        };
    }
}


#endif //CALENHAD_SELECTIONTOCLIPBOARDCOMMAND_H
