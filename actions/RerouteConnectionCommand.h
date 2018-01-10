//
// Created by martin on 09/01/18.
//

#ifndef CALENHAD_REROUTECONNECTIONCOMMAND_H
#define CALENHAD_REROUTECONNECTIONCOMMAND_H

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

        class RerouteConnectionCommand : public QUndoCommand {


        public:

            RerouteConnectionCommand (nodeedit::QNEPort* from, nodeedit::QNEPort* oldPort, nodeedit::QNEPort* newPort, pipeline::CalenhadModel* model);

            virtual ~RerouteConnectionCommand ();

            virtual void undo () override;

            virtual void redo () override;

        protected:
            calenhad::pipeline::CalenhadModel* _model = nullptr;
            calenhad::nodeedit::QNEPort* _oldPort = nullptr;
            calenhad::nodeedit::QNEPort* _newPort = nullptr;
            calenhad::nodeedit::QNEPort* _from = nullptr;

        };
    }
}


#endif //CALENHAD_REROUTECONNECTIONCOMMAND_H
