//
// Created by ferret on 14/10/2020.
//

#ifndef CALENHAD_CUBERASTERDIALOG_H
#define CALENHAD_CUBERASTERDIALOG_H
#include <QList>
#include <QDialog>
#include <QTableView>

namespace calenhad {
    namespace controls {
        class CubeRasterWidget;
    }
    namespace module {
        class CubeRasterFile;
        class CubeRasterModule;

        class CubeRasterDialog : public QDialog {

        public:
            explicit CubeRasterDialog (CubeRasterModule* parent);
            ~CubeRasterDialog() override;
        private:

            calenhad::controls::CubeRasterWidget* _widget;
        };
    }
}


#endif //CALENHAD_CUBERASTERDIALOG_H
