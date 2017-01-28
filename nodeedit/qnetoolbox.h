 //
// Created by martin on 31/10/16.
//

#ifndef CALENHAD_QNETOOLUSER_H
#define CALENHAD_QNETOOLUSER_H
#include <QToolBar>
#include <QToolButton>
#include <QPushButton>
#include <QMenu>
#include "../qmodule/QModule.h"

 class ToolCollection : public QObject {
Q_OBJECT
public:
    ToolCollection();
    ~ToolCollection();
    virtual void addTool (QAction* tool);
    virtual void removeTool (QAction* tool);
    void clear();
    QAction* getTool (const QString& id);
    QList<QAction*> tools();
protected:
    QMap<QString, QAction*> _tools;
};

class ToolDrawer : public ToolCollection {
Q_OBJECT
public:
    ToolDrawer (const QString& name);
    ~ToolDrawer();
    QMenu* menu();
    QToolBar* toolbar();
    QString& name();
protected:
    QString _name;
};

class ToolGroup : public ToolCollection {
Q_OBJECT
    QAction* activeTool();
public:
    void addTool (QAction* tool) override;
public slots:
    void toolTriggered();
    void toolToggled (bool) ;
protected:
    QAction* _activeTool = nullptr;
};

enum CalenhadAction {
    UndoAction, RedoAction,
    ZoomInAction, ZoomOutAction, ZoomToFitAction, ZoomToSelectionAction,
     DeleteConnectionAction, DeleteModuleAction
};

Q_DECLARE_METATYPE (CalenhadAction)

class QNEToolBox : public QObject {
Q_OBJECT
public:

    QNEToolBox();
    ~QNEToolBox();
    QToolBar* toolbar (const QString& drawer);
    QMenu* menu (const QString& drawer);
    void addDrawer (ToolDrawer* drawer);
    void addGroup (ToolGroup* group);

private:
    QMap<QString, ToolDrawer*> _drawers = QMap<QString, ToolDrawer*>();
    QList<ToolGroup*> _groups  = QList<ToolGroup*>();
    QString _name;
};


#endif //CALENHAD_QNETOOLUSER_H
