//
// Created by martin on 28/04/17.
//

#ifndef CALENHAD_DUPLICATEMODULECOMMAND_H
#define CALENHAD_DUPLICATEMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
class QModule;
class CalenhadModel;

class DuplicateModuleCommand : public QUndoCommand {
public:
    DuplicateModuleCommand (QModule* module, CalenhadModel* model);
    virtual ~DuplicateModuleCommand();
    virtual void undo() override;
    virtual void redo() override;

protected:
    CalenhadModel* _model = nullptr;
    QModule* _module, * _copy;
};


#endif //CALENHAD_DUPLICATEMODULECOMMAND_H
