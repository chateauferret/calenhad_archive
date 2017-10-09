//
// Created by martin on 08/10/17.
//

#ifndef CALENHAD_PASTECOMMAND_H
#define CALENHAD_PASTECOMMAND_H


#include <QtWidgets/QUndoCommand>

namespace calenhad {
    namespace qmodule {
        class QNode;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace actions {
        class PasteCommand : public QUndoCommand {
        public:
            PasteCommand (calenhad::pipeline::CalenhadModel* _model);

            virtual ~PasteCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::pipeline::CalenhadModel* _model;
            QString _oldXml;
        };
    }
}


#endif //CALENHAD_PASTECOMMAND_H
