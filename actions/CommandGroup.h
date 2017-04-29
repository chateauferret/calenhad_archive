//
// Created by martin on 28/04/17.
//

#ifndef CALENHAD_COMMANDGROUP_H
#define CALENHAD_COMMANDGROUP_H


#include <QtWidgets/QUndoCommand>
#include <QtCore/QStack>

class CommandGroup : public QUndoCommand {
public:
    CommandGroup();
    virtual ~CommandGroup();
    virtual void undo () override;
    virtual void redo () override;
    void addCommand (QUndoCommand* command);

protected:
    QStack<QUndoCommand*> _commands;
};


#endif //CALENHAD_COMMANDGROUP_H
