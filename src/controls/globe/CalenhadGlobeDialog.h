//
// Created by martin on 09/12/18.
//

#ifndef MESSAGES_CALENHADGLOBEDIALOG_H
#define MESSAGES_CALENHADGLOBEDIALOG_H
#include <QMainWindow>
#include "../../module/Module.h"
#include "CalenhadGlobeWidget.h"

namespace calenhad {
    namespace module {
        class Module;
    }
    namespace nodeedit {
        class CalenhadToolBar;
    }
    namespace pipeline {
        class CalenhadModel;
    }
    namespace controls {
        namespace globe {
            class CalenhadGlobeWidget;

            class CalenhadGlobeDialog : public QMainWindow {
            Q_OBJECT




            public:
                CalenhadGlobeDialog (QWidget *parent);
                ~CalenhadGlobeDialog() override;
                CalenhadGlobeWidget* widget();
                void setModel (calenhad::pipeline::CalenhadModel* model);
                void selectModule (module::Module *module);

                calenhad::pipeline::CalenhadModel* model() const;

            protected:
                CalenhadGlobeWidget* _widget;
                calenhad::pipeline::CalenhadModel* _model{};
                void closeEvent (QCloseEvent* e) override;
                void showEvent (QShowEvent* e) override;
                QDockWidget* _mainDock, * _mouseDock, * _widgetDock;

            };

        }
    }
}



#endif //MESSAGES_CALENHADGLOBEDIALOG_H
