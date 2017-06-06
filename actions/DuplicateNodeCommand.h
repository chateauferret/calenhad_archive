//
// Created by martin on 28/04/17.
//

#ifndef CALENHAD_DUPLICATEMODULECOMMAND_H
#define CALENHAD_DUPLICATEMODULECOMMAND_H


#include <QtWidgets/QUndoCommand>
class QNode;
class CalenhadModel;

class DuplicateNodeCommand : public QUndoCommand {
public:
    DuplicateNodeCommand (QNode* node, CalenhadModel* model);
    virtual ~DuplicateNodeCommand();
    virtual void undo() override;
    virtual void redo() override;

protected:
    CalenhadModel* _model = nullptr;
    QNode* _node, * _copy;
};


#endif //CALENHAD_DUPLICATEMODULECOMMAND_H
