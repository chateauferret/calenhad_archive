//
// Created by martin on 28/04/17.
//

#ifndef CALENHAD_DUPLICATEMODULECOMMAND_H
#define CALENHAD_DUPLICATEMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>

namespace calenhad {
    namespace qmodule {
        class Node;
    }
    namespace pipeline {
        class CalenhadModel;
    }

    namespace actions {

        class DuplicateNodeCommand : public QUndoCommand {
        public:
            DuplicateNodeCommand (calenhad::qmodule::Node* node, calenhad::pipeline::CalenhadModel* model);

            virtual ~DuplicateNodeCommand();

            virtual void undo() override;

            virtual void redo() override;

        protected:
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::qmodule::Node* _node, * _copy;
        };
    }
}


#endif //CALENHAD_DUPLICATEMODULECOMMAND_H
