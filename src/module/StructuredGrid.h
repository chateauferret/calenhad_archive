//
// Created by martin on 03/11/17.
//

#ifndef CALENHAD_QRASTERMODULE_H
#define CALENHAD_QRASTERMODULE_H

#include "Module.h"

namespace calenhad {
    namespace controls {
        class QAngleControl;
    }
    namespace module {
        class StructuredGrid : public Module {
        public:
            explicit StructuredGrid (const QString& type = CalenhadServices::preferences() -> calenhad_module_raster);
            ~StructuredGrid() override;
            bool isComplete() override;
            QString glsl() override;

            int rasterSize();
            grid::CubicSphere *buffer() const;


        protected slots:

            void serialize (QDomElement& element) override;
            void inflate (const QDomElement& element) override;

        protected:
            calenhad::grid::CubicSphere* _buffer;
            void setRasterSize (const int& depth);
        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
