//
// Created by martin on 07/10/2019.
//

#ifndef CALENHAD_RASTERFILEMODULE_H
#define CALENHAD_RASTERFILEMODULE_H

#include "Convolution.h"

namespace calenhad {
    namespace module {


        class RasterFileModule : public Convolution {

        public:

            explicit RasterFileModule (const QString& type = CalenhadServices::preferences() -> calenhad_module_raster_file);

            void fileDialogRequested();

            void openFile (const QString& filename);

            QImage* raster() override;


        protected slots:

            void inflate (const QDomElement& element) override;

            void serialize (QDomElement& element) override;

        protected:

            QString _filename;

            QFormLayout* _rasterLayout{};

            QLabel* _filenameLabel;

        };

    }
}


#endif //CALENHAD_RASTERFILEMODULE_H
