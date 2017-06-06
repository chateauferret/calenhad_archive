//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_DELETEMODULECOMMAND_H
#define CALENHAD_DELETEMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
#include "../qmodule/QModule.h"


class DeleteNodeCommand : public QUndoCommand {

public:
    DeleteNodeCommand (QNode* node, CalenhadModel* model);
    virtual ~DeleteNodeCommand ();
    virtual void undo () override;
    virtual void redo () override;

protected:
    CalenhadModel* _model = nullptr;
    QNode* _node;
    QString _xml;
};


#endif //CALENHAD_DELETEMODULECOMMAND_H
