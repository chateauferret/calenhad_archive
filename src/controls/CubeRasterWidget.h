//
// Created by ferret on 03/02/2021.
//

#ifndef CALENHAD_CUBERASTERWIDGET_H
#define CALENHAD_CUBERASTERWIDGET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>

namespace calenhad {
    namespace controls {

        class CubeRasterWidget : public QWidget {
        public:
            explicit CubeRasterWidget (QWidget* parent);
            ~CubeRasterWidget () override;

        private:
            QGridLayout* _layout;
        };

    }
}


#endif //CALENHAD_CUBERASTERWIDGET_H
