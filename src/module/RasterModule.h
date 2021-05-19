//
// Created by martin on 07/10/2019.
//

#ifndef CALENHAD_RASTERFILEMODULE_H
#define CALENHAD_RASTERFILEMODULE_H

#include "StructuredGrid.h"

namespace calenhad {
    namespace mapping {
        struct Raster;
        struct Extent;
    }

    namespace module {

        class RasterModule : public StructuredGrid {

        public:

            explicit RasterModule (const QString& type = CalenhadServices::preferences() -> calenhad_module_raster);
            ~RasterModule() override;
            virtual void fileDialogRequested();

            QString glsl() override;

            virtual void openFile (const QString& filename);
            bool isComplete() override;
            calenhad::grid::CubicSphere* raster();

            void fetch (grid::CubicSphere* buffer) override ;


        protected slots:

            void inflate (const QDomElement& element) override;

            void serialize (QDomElement& element) override;

        protected:

            QString _filename;
            calenhad::grid::CubicSphere* _buffer;
            QWidget* _rasterContent;
            QFormLayout* _rasterLayout{};
            QVector<calenhad::mapping::Raster*> _rasters;
            QLabel* _filenameLabel;

            void assimilateRaster (QImage* image);
            void updateBuffer ();
        };

    }
}


#endif //CALENHAD_RASTERFILEMODULE_H
