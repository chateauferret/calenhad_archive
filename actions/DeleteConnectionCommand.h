//
// Created by martin on 24/01/17.
//

#ifndef CALENHAD_DELETECONNECTIONCOMMAND_H
#define CALENHAD_DELETECONNECTIONCOMMAND_H


#include <QtWidgets/QUndoCommand>
#include "../nodeedit/qneport.h"
#include "../pipeline/CalenhadModel.h"

class DeleteConnectionCommand : public QUndoCommand {


public:
    DeleteConnectionCommand (QNEConnection* connection, CalenhadModel* model);
    virtual ~DeleteConnectionCommand ();
    virtual void undo () override;
    virtual void redo () override;

protected:
    QNEConnection* _connection = nullptr;
    CalenhadModel* _model = nullptr;
    QNEPort* _from = nullptr;
    QNEPort* _to = nullptr;

};


#endif //CALENHAD_DELETECONNECTIONCOMMAND_H
