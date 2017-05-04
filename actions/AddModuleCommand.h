//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_ADDMODULECOMMAND_H
#define CALENHAD_ADDMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
#include "../qmodule/QModule.h"


class AddModuleCommand : public QUndoCommand {

public:
    AddModuleCommand (QModule* module, const QPointF pos, CalenhadModel* model);
    virtual ~AddModuleCommand ();
    virtual void undo () override;
    virtual void redo () override;

protected:
    CalenhadModel* _model = nullptr;
    QModule* _module,  * _copy;
    QPointF _pos;
};


#endif //CALENHAD_ADDMODULECOMMAND_H
