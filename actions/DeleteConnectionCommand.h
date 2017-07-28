//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_DELETECONNECTIONCOMMAND_H
#define CALENHAD_DELETECONNECTIONCOMMAND_H


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

        class DeleteConnectionCommand : public QUndoCommand {


        public:
            DeleteConnectionCommand (calenhad::nodeedit::QNEConnection* connection, calenhad::pipeline::CalenhadModel* model);

            virtual ~DeleteConnectionCommand ();

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


#endif //CALENHAD_DELETECONNECTIONCOMMAND_H
