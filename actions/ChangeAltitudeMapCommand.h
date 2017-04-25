//
// Created by martin on 25/04/17.
//

#ifndef CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
#define CALENHAD_CHANGEALTITUDEMAPCOMMAND_H


#include <QtCore/QString>
#include <QtWidgets/QUndoCommand>
#include "../qmodule/QAltitudeMap.h"

class ChangeAltitudeMapCommand : public QUndoCommand {
public:
    ChangeAltitudeMapCommand (QAltitudeMap* node, const QString& oldXml, const QString& newXml);
    virtual ~ChangeAltitudeMapCommand();
    virtual void redo() override;
    virtual void undo() override;

protected:
    QString _oldXml, _newXml;
    QAltitudeMap* _node;
};


#endif //CALENHAD_CHANGEALTITUDEMAPCOMMAND_H
