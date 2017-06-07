//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_ADDMODULECOMMAND_H
#define CALENHAD_ADDMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
#include "../qmodule/QModule.h"


class AddNodeCommand : public QUndoCommand {

public:
    AddNodeCommand (QNode* node, const QPointF pos, CalenhadModel* model);
    virtual ~AddNodeCommand ();
    virtual void undo () override;
    virtual void redo () override;
    QNode* node ();
protected:
    CalenhadModel* _model = nullptr;
    QNode* _node,  * _copy;
    QPointF _pos;


};


#endif //CALENHAD_ADDMODULECOMMAND_H
