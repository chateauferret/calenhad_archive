//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_DELETEMODULECOMMAND_H
#define CALENHAD_DELETEMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
#include "../qmodule/QModule.h"


class DeleteModuleCommand : public QUndoCommand {

public:
    DeleteModuleCommand (QModule* module, CalenhadModel* model);
    virtual ~DeleteModuleCommand ();
    virtual void undo () override;
    virtual void redo () override;

protected:
    CalenhadModel* _model = nullptr;
    QModule* _module;
    QString _xml;
};


#endif //CALENHAD_DELETEMODULECOMMAND_H
