//
// Created by martin on 18/04/17.
//

#ifndef CALENHAD_CHANGEMODULECOMMAND_H
#define CALENHAD_CHANGEMODULECOMMAND_H

#include <QtWidgets/QUndoCommand>
#include <QtCore/QVariant>

class QModule;

class ChangeModuleCommand : public QUndoCommand {
public:
    ChangeModuleCommand (QModule* module, const QString& property, const QVariant& oldValue, const QVariant& newValue);
    virtual ~ChangeModuleCommand();
    virtual void redo() override;
    virtual void undo() override;

protected:
    QString _property;
    QVariant _oldValue, _newValue;
    QModule* _module;

};


#endif //CALENHAD_CHANGEMODULECOMMAND_H
