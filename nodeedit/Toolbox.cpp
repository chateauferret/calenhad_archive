//
// Created by martin on 31/10/16.
//

#include <QtWidgets/QGraphicsView>
#include <iostream>
#include "Toolbox.h"
#include "CalenhadToolBar.h"

using namespace calenhad::nodeedit;

ToolBox::ToolBox() : QObject(),
    _drawers  (QMap<QString, ToolDrawer*>()),
    _groups  (QList<ToolGroup*>()) {
}

ToolBox::~ToolBox() { }

CalenhadToolBar* ToolBox::toolbar (const QString& drawer) {
    return _drawers.value (drawer) -> toolbar();
}

QMenu* ToolBox::menu (const QString& drawer) {
    return _drawers.value (drawer) -> menu();
}

void ToolBox::addDrawer (ToolDrawer* drawer) {
    _drawers.insert (drawer -> name(), drawer);
}

void ToolBox::addGroup (ToolGroup* group) {
    _groups.append (group);
}

ToolCollection::ToolCollection() : QObject() {

}

ToolCollection::~ToolCollection() {

}

void ToolCollection::addTool (QAction* tool) {
    QString key = tool -> data().toString();
    _tools.insert (key, tool);
}

void ToolCollection::removeTool (QAction* tool) {
    _tools.remove (tool -> data().toString());
}

void ToolCollection::clear () {
    _tools.clear();
}

QAction* ToolCollection::getTool (const QString& id) {
    return _tools.value (id);
}

QList<QAction*> ToolCollection::tools() {
    return _tools.values();
}

QAction* ToolGroup::activeTool() {
    return _activeTool;
}

ToolDrawer::ToolDrawer (const QString& name) : ToolCollection(), _name (name) {

}

ToolDrawer::~ToolDrawer() {
    clear();
}

QString& ToolDrawer::name() {
    return _name;
}

CalenhadToolBar* ToolDrawer::toolbar() {
    CalenhadToolBar* toolbar = new CalenhadToolBar (_name, (QWidget*) parent());
    toolbar -> setOrientation (Qt::Vertical);
    for (QAction* action : _tools.values()) {
        toolbar -> addAction (action);
    }
    return toolbar;
}

QMenu* ToolDrawer::menu() {
    QMenu* _menu = new QMenu();
    _menu -> setTitle (_name);
    for (QAction* action : _tools.values()) {
        _menu -> addAction (action);
    }
    return _menu;
}

void ToolGroup::addTool (QAction* tool)  {
    ToolCollection::addTool (tool);
    if (tool -> isCheckable ()) {
        connect (tool, &QAction::toggled, this, &ToolGroup::toolToggled);
    } else {
        connect (tool, &QAction::triggered, this, &ToolGroup::toolToggled);
    }
}

void ToolGroup::toolToggled (bool state) {
    QAction* tool = (QAction*) sender();
    if (state) {
        _activeTool = tool;
        for (QAction* a : _tools) {
            if (a != tool) {
                a -> setChecked (false);
            }
        }
    } else {
        QAction* tool = _activeTool;
        toolTriggered();
    }
    _activeTool = nullptr;
}

void ToolGroup::toolTriggered() {
    for (QAction* a : _tools) {
        a -> setChecked (false);
    }
}