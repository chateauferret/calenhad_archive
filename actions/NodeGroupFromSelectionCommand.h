//
// Created by martin on 10/05/18.
//

#ifndef CALENHAD_NODEGROUPFROMSELECTIONCOMMAND_H
#define CALENHAD_NODEGROUPFROMSELECTIONCOMMAND_H

#include <QtWidgets/QUndoCommand>
#include "XmlCommand.h"

namespace calenhad {
    namespace qmodule {
        class Node;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace actions {
        class NodeGroupFromSelectionCommand : public XmlCommand {
        public:
            NodeGroupFromSelectionCommand (calenhad::pipeline::CalenhadModel* _model, const QString& oldXml);
            virtual ~NodeGroupFromSelectionCommand();
            virtual void undo() override;
            virtual void redo() override;

        };
    }
}

#endif //CALENHAD_NODEGROUPFROMSELECTIONCOMMAND_H
