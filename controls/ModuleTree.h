//
// Created by martin on 25/10/18.
//

#ifndef MESSAGES_MODULEROSTER_H
#define MESSAGES_MODULEROSTER_H

#include <QWidget>
#include <QtWidgets/QTreeWidget>
#include "../pipeline/CalenhadModel.h"

namespace calenhad {
    namespace controls {
        class ModuleTree : public QWidget {
            Q_OBJECT

        public:
            explicit ModuleTree (calenhad::pipeline::CalenhadModel* model);
            ~ModuleTree() override;

        public slots:
            void buildTree();
            void selectItem (QTreeWidgetItem* item);
            void showMenu (const QPoint& pos);

        signals:
            void treeShown();
            void treeHidden();

        protected:
            QTreeWidgetItem* findTreeItem (calenhad::module::NodeGroup* g);
            QTreeWidgetItem* findTreeItem (calenhad::module::Module* m);
            void showEvent (QShowEvent* e) override;
            void hideEvent (QHideEvent* e) override;
            pipeline::CalenhadModel* _model;
            QTreeWidget* _tree;
            QIcon* _groupIcon;
            QMenu* _menu;
            QMap<QTreeWidgetItem*, calenhad::module::NodeGroup*> _groups;
            QMap<QTreeWidgetItem*, calenhad::module::Module*> _modules;
        };
    }
}


#endif //MESSAGES_MODULEROSTER_H
