//
// Created by martin on 06/01/17.
//

#ifndef CALENHAD_CALENHADMODEL_H
#define CALENHAD_CALENHADMODEL_H


#include <QtWidgets/QGraphicsScene>
#include "../qmodule/QModule.h"
#include "../libnoiseutils/nullmodule.h"
#include "../pipeline/ModuleFactory.h"

class CalenhadController;

namespace icosphere {
    class Icosphere;
}

class CalenhadModel : public QGraphicsScene {
Q_OBJECT
public:
    CalenhadModel();
    virtual ~CalenhadModel();
    QModule* findModule (const QString& name);
    void save (QDataStream& ds);
    void load (QDataStream& ds);
    void addModule (const QPointF& initPos, const ModuleType& type, const QString& name = QString::null);
    void deleteModule (QModule* module);
    bool canConnect (QNEPort* output, QNEPort* input);
    void connectPorts (QNEPort* output, QNEPort* input);
    void disconnectPorts (QNEConnection* connection);
    bool eventFilter (QObject* o, QEvent* e);
    void setActiveTool (QAction* tool);
    QGraphicsItem* itemAt (const QPointF& pos);
    void setController (CalenhadController* controller);
    CalenhadController* controller();
    void mousePressEvent (QGraphicsSceneMouseEvent* event) override;

signals:
    void showMessage (QString);
    void moduleAdded (QModule*);

protected:

    bool existsPath (QNEBlock* from, QNEBlock* to);
    NullModule* nullModule = new NullModule();
    ModuleFactory _moduleFactory;
    QPointF lastClick;

    QAction* _activeTool = nullptr;
    QNEConnection* conn;
    QNEPort* _port = nullptr; // last port we coloured in as dropping on
    CalenhadController* _controller = nullptr;

};

#endif //CALENHAD_CALENHADMODEL_H
