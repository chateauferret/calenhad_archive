//
// Created by martin on 09/01/18.
//

#ifndef CALENHAD_REROUTECONNECTIONCOMMAND_H
#define CALENHAD_REROUTECONNECTIONCOMMAND_H

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

        class RerouteConnectionCommand : public QUndoCommand {


        public:

            RerouteConnectionCommand (nodeedit::Port* from, nodeedit::Port* oldPort, nodeedit::Port* newPort, pipeline::CalenhadModel* model);

            virtual ~RerouteConnectionCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::nodeedit::Port* _oldPort = nullptr;
            calenhad::nodeedit::Port* _newPort = nullptr;
            calenhad::nodeedit::Port* _from = nullptr;

        };
    }
}


#endif //CALENHAD_REROUTECONNECTIONCOMMAND_H
