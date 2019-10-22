//
// Created by martin on 25/10/18.
//

#include "ModuleTree.h"
#include "../module/Module.h"
#include "../module/NodeGroup.h"
#include <QTreeWidget>
#include <QList>
#include <CalenhadServices.h>
#include "../controls/QColoredIcon.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"

using namespace calenhad::controls;
using namespace calenhad::module;
using namespace calenhad::pipeline;

calenhad::controls::ModuleTree::ModuleTree (calenhad::pipeline::CalenhadModel* model) : QWidget(), _menu (nullptr) {
    _model = model;
    QLabel* nameLabel = new QLabel();
    nameLabel -> setText ("Module roster");

    setWindowFlags (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint);

    QPushButton* closeButton = new QPushButton();
    closeButton -> setIcon (QIcon (":/appicons/controls/close.png"));
    _groupIcon = new QIcon (":/appicons/controls/group.png");
    connect (closeButton, &QPushButton::pressed, this, &ModuleTree::close);
    connect (_model, &CalenhadModel::modelChanged, this, &ModuleTree::buildTree);
    _tree = new QTreeWidget();
    _tree -> setColumnCount (1);
    _tree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    _tree -> setContextMenuPolicy(Qt::CustomContextMenu);
    QVBoxLayout* layout = new QVBoxLayout();
    layout -> addWidget (_tree);
    setLayout (layout);
    buildTree();
    connect (_tree, &QTreeWidget::itemDoubleClicked, this, &ModuleTree::selectItem);
    connect (_tree, &QWidget::customContextMenuRequested, this, &ModuleTree::showMenu);

}

ModuleTree::~ModuleTree() {
    delete _groupIcon;
    delete _menu;
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
                    groupItem -> setText (0, g -> name ());
                    groupItem -> setIcon (0, *_groupIcon);
                    groupItem -> setToolTip (0, g -> documentation());
                    item = new QTreeWidgetItem (groupItem);
                } else {
                    item = new QTreeWidgetItem (parent.first ());
                }
            } else {
                item = new QTreeWidgetItem (_tree);
            }
            QPixmap* pixmap = CalenhadServices::modules() -> getIcon (m -> nodeType());
            QIcon icon (*pixmap);
            item -> setIcon (0, icon);
            item -> setText (0, m -> name());
            item -> setToolTip (0, m -> description());
            _modules.insert (item, m);
        }
    }
}

void ModuleTree::selectItem (QTreeWidgetItem* item) {
    Module* module = _modules.find (item).value();
    if (module) {
        module -> handle() -> setSelected (! (module -> handle() -> isSelected()));
    } else {
        NodeGroup* group = _groups.find (item).value();
        if (group) {
            bool state = true;
            for (Module* module : group -> modules()) {
                if (! module -> handle() -> isSelected()) {
                    state = false;
                }
            }
            for (Module* module : group -> modules()) {
                module -> handle() -> setSelected (! state);
            }
        }
    }
}

void ModuleTree::showMenu (const QPoint& pos) {
    QTreeWidgetItem* item = _tree -> itemAt (pos);
    if (item) {

        Module* module = _modules.find (item).value ();
        QGraphicsItem* handle = nullptr;
        if (module) {
            handle = module->handle ();
        }

        if (_menu) {
            delete _menu;
        }
        _menu = new QMenu();
        QAction* selectAction = new QAction ("Select");
        selectAction -> setCheckable (true);
        selectAction -> setChecked (handle && handle -> isSelected());
        connect (selectAction, &QAction::triggered, this, [=] () {
            if (handle) {
                bool state = !handle->isSelected ();
                item -> setSelected (state);
                handle -> setSelected (state);
                selectAction -> setChecked (state);
            }
        });
        _menu->addAction (selectAction);

        QAction* deleteAction = new QAction ("Delete");
        connect (deleteAction, &QAction::triggered, this, [=] () { _model->doDeleteNode (module); buildTree(); });
        _menu->addAction (deleteAction);

        QAction* propertiesAction = new QAction ("Properties");
        propertiesAction -> setCheckable (true);
        propertiesAction -> setChecked (module && module -> isModuleDetailVisible());
        connect (propertiesAction, &QAction::toggled, this, [=] (bool visible) {
            module -> showModuleDetail (visible);
            propertiesAction -> setChecked (visible);
            std::cout << visible << "\n";
        });
        _menu -> addAction (propertiesAction);

        QAction* gotoAction = new QAction ("Go to");
        connect (gotoAction, &QAction::triggered, this, [=] () { _model -> goTo (module); });
        _menu -> addAction (gotoAction);
        _menu -> exec (_tree -> mapToGlobal (pos));
    }
}