//
// Created by martin on 25/12/17.
//

#ifndef CALENHAD_CREATECONNECTIONCOMMAND_H
#define CALENHAD_CREATECONNECTIONCOMMAND_H


#include <QtWidgets/QUndoCommand>


namespace calenhad {
    namespace qmodule {
        class QNode;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace nodeedit {
        class QNEConnection;
        class QNEPort;
    }

    namespace actions {

        class CreateConnectionCommand : public QUndoCommand {


        public:

            CreateConnectionCommand (nodeedit::QNEPort* from, nodeedit::QNEPort* to, pipeline::CalenhadModel* model);

            virtual ~CreateConnectionCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::nodeedit::QNEConnection* _connection = nullptr;
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::nodeedit::QNEPort* _from = nullptr;
            calenhad::nodeedit::QNEPort* _to = nullptr;

        };
    }
}


#endif //CALENHAD_CREATECONNECTIONCOMMAND_H
