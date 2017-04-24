//
// Created by martin on 18/04/17.
//

#ifndef CALENHAD_CHANGEMODULECOMMAND_H
#define CALENHAD_CHANGEMODULECOMMAND_H

#include <QtWidgets/QUndoCommand>
#include <QtCore/QVariant>

class QNode;

class ChangeModuleCommand : public QUndoCommand {
public:
    ChangeModuleCommand (QNode* node, const QString& property, const QVariant& oldValue, const QVariant& newValue, const int& portIndex = -1, const int& portType = -1);
    virtual ~ChangeModuleCommand();
    virtual void redo() override;
    virtual void undo() override;

protected:
    QString _property;
    QVariant _oldValue, _newValue;
    QNode* _node;
    int _portIndex;
    int _portType;
};


#endif //CALENHAD_CHANGEMODULECOMMAND_H
