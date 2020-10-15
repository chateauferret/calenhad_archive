//
// Created by ferret on 14/10/2020.
//

#ifndef CALENHAD_CUBERASTERDIALOG_H
#define CALENHAD_CUBERASTERDIALOG_H
#include <QList>
#include <QDialog>

namespace calenhad {
    namespace module {
        class CubeRasterFile;
        class CubeRasterModule;

        class CubeRasterDialog : public QDialog {

        public:
            explicit CubeRasterDialog (CubeRasterModule* parent);
            ~CubeRasterDialog();


            QList<QImage*> images ();

            QImage* image (int i);

            void populate (const QList<CubeRasterFile*>& list);
            CubeRasterFile* file (const int& i);

        private:

            QList<CubeRasterFile*> _files;

        };
    }
}


#endif //CALENHAD_CUBERASTERDIALOG_H
