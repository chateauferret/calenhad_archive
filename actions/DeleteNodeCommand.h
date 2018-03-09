//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_DELETEMODULECOMMAND_H
#define CALENHAD_DELETEMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>

namespace calenhad {
    namespace qmodule {
        class Node;
    }
    namespace pipeline {
        class CalenhadModel;
    }

    namespace actions {

        class DeleteNodeCommand : public QUndoCommand {

        public:
            DeleteNodeCommand (calenhad::qmodule::Node* node, calenhad::pipeline::CalenhadModel* model);

            virtual ~DeleteNodeCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::qmodule::Node* _node;
            QString _xml;
        };
    }
}

#endif //CALENHAD_DELETEMODULECOMMAND_H
