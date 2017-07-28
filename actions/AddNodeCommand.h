//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_ADDMODULECOMMAND_H
#define CALENHAD_ADDMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
#include <QtCore/QPointF>

namespace calenhad {
    namespace qmodule {
        class QNode;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace actions {

        class AddNodeCommand : public QUndoCommand {

        public:
            AddNodeCommand (calenhad::qmodule::QNode* node, const QPointF pos, calenhad::pipeline::CalenhadModel* model);

            virtual ~AddNodeCommand ();

            virtual void undo () override;

            virtual void redo () override;

            calenhad::qmodule::QNode* node ();

        protected:
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::qmodule::QNode* _node, * _copy;
            QPointF _pos;


        };
    }
}

#endif //CALENHAD_ADDMODULECOMMAND_H
