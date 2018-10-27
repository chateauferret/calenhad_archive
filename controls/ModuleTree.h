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
            ModuleTree (calenhad::pipeline::CalenhadModel* model);
            virtual ~ModuleTree();

        public slots:
            void buildTree();

        signals:
            void treeShown();
            void treeHidden();

        protected:

            QTreeWidgetItem* findTreeItem (calenhad::qmodule::NodeGroup* g);
            QTreeWidgetItem* findTreeItem (calenhad::qmodule::Module* m);
            void showEvent (QShowEvent* e) override;
            void hideEvent (QHideEvent* e) override;
            pipeline::CalenhadModel* _model;
            QTreeWidget* _tree;
            QIcon* _groupIcon;
            QMap<QTreeWidgetItem*, calenhad::qmodule::NodeGroup*> _groups;
            QMap<QTreeWidgetItem*, calenhad::qmodule::Module*> _modules;
        };
    }
}


#endif //MESSAGES_MODULEROSTER_H
