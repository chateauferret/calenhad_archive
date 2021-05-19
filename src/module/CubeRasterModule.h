//
// Created by ferret on 11/10/2020.
//

#ifndef CALENHAD_CUBERASTERMODULE_H
#define CALENHAD_CUBERASTERMODULE_H
#include "RasterModule.h"
#include "CubeRasterDialog.h"

namespace calenhad {
    namespace module {

        class CubeRasterFile;
        class CubeRasterDialog;

        class CubeRasterModule : public RasterModule {
        public:

            explicit CubeRasterModule (const QString& type = CalenhadServices::preferences() -> calenhad_module_cuberaster);
            ~CubeRasterModule() override;
            bool isComplete() override;
            void invalidate() override;
            QList<CubeRasterFile*> files();

        public slots:
            void fileDialogRequested() override;

        protected slots:

            void inflate (const QDomElement& element) override;

            void serialize (QDomElement& element) override;

        protected:



            int _index;
            int _rotation;
            CubeRasterDialog* _dialog;
            QList<CubeRasterFile*> _files;

            QImage* image (int i);

            QList<QImage*> images ();
        };

    }
}

#endif //CALENHAD_CUBERASTERMODULE_H
