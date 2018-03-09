//
// Created by martin on 25/12/17.
//

#ifndef CALENHAD_CREATECONNECTIONCOMMAND_H
#define CALENHAD_CREATECONNECTIONCOMMAND_H


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

        class CreateConnectionCommand : public QUndoCommand {


        public:

            CreateConnectionCommand (nodeedit::Port* from, nodeedit::Port* to, pipeline::CalenhadModel* model);

            virtual ~CreateConnectionCommand ();

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


#endif //CALENHAD_CREATECONNECTIONCOMMAND_H
