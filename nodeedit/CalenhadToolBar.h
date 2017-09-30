//
// Created by martin on 29/09/17.
//

#ifndef CALENHAD_CALENHADTOOLBAR_H
#define CALENHAD_CALENHADTOOLBAR_H

#include <QtWidgets/QToolBar>

namespace calenhad {
    namespace nodeedit {
        class CalenhadToolBar : public QToolBar {
        public:
            CalenhadToolBar (const QString& name, QWidget* parent = 0);
            virtual ~CalenhadToolBar();

            public slots:
            void arrange (Qt::DockWidgetArea area);

        };
    }
}


#endif //CALENHAD_CALENHADTOOLBAR_H
