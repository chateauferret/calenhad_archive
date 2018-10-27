//
// Created by martin on 25/10/18.
//

#include "ModuleTree.h"
#include "../qmodule/Module.h"
#include "../qmodule/NodeGroup.h"
#include <QTreeWidget>
#include <QList>
#include "../controls/QColoredIcon.h"

using namespace calenhad::controls;
using namespace calenhad::qmodule;

calenhad::controls::ModuleTree::ModuleTree (calenhad::pipeline::CalenhadModel* model) {
    _model = model;
    QLabel* nameLabel = new QLabel();
    nameLabel -> setText ("Module roster");

    QPushButton* closeButton = new QPushButton();
    closeButton -> setIcon (QIcon (":/appicons/controls/close.png"));
    _groupIcon = new QIcon (":/appicons/controls/group.png");
    connect (closeButton, &QPushButton::pressed, this, &ModuleTree::close);

    _tree = new QTreeWidget (this);
    _tree -> setColumnCount (1);
    buildTree();

}

ModuleTree::~ModuleTree() {
    delete _groupIcon;
}

QTreeWidgetItem* ModuleTree::findTreeItem (NodeGroup* g) {
    for (QTreeWidgetItem* item : _groups.keys()) {
        if (_groups.find (item).value() == g) {
            return item;
        }
    }
    return nullptr;
}


QTreeWidgetItem* ModuleTree::findTreeItem (Module* m) {
    for (QTreeWidgetItem* item : _modules.keys()) {
        if (_modules.find (item).value() == m) {
            return item;
        }
    }
    return nullptr;
}


void ModuleTree::showEvent (QShowEvent* e) {
    buildTree();
    emit treeShown();
}

void ModuleTree::hideEvent (QHideEvent* e) {
    buildTree();
    emit treeHidden();
}

void ModuleTree::buildTree() {
    _tree -> clear();
    if (_model) {
        for (Module* m : _model -> modules()) {
            NodeGroup* g =  m -> group();
            QTreeWidgetItem* item;
            if (g) {
                QList<QTreeWidgetItem*> parent = _tree->findItems (g->name (), Qt::MatchExactly, 0);
                if (parent.isEmpty ()) {
                    QTreeWidgetItem* groupItem = new QTreeWidgetItem (_tree);
                    groupItem->setText (0, g->name ());
                    item = new QTreeWidgetItem (groupItem);
                } else {
                    item = new QTreeWidgetItem (parent.first ());
                }
            } else {
                item = new QTreeWidgetItem (_tree);
            }
            //item -> setIcon (0, );
            item -> setText (0, m -> name());
            item -> setToolTip (0, m -> description());
            _modules.insert (item, m);
        }
    }

}