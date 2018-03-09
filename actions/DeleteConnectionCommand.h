//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_DELETECONNECTIONCOMMAND_H
#define CALENHAD_DELETECONNECTIONCOMMAND_H


#include <QtWidgets/QUndoCommand>


namespace calenhad {
    namespace qmodule {
        class Node;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace nodeedit {
        class Connection;
        class Port;
    }

    namespace actions {

        class DeleteConnectionCommand : public QUndoCommand {


        public:
            DeleteConnectionCommand (calenhad::nodeedit::Connection* connection, calenhad::pipeline::CalenhadModel* model);

            virtual ~DeleteConnectionCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::nodeedit::Connection* _connection = nullptr;
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::nodeedit::Port* _from = nullptr;
            calenhad::nodeedit::Port* _to = nullptr;

        };
    }
}


#endif //CALENHAD_DELETECONNECTIONCOMMAND_H
