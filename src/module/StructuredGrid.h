//
// Created by martin on 03/11/17.
//

#ifndef CALENHAD_QRASTERMODULE_H
#define CALENHAD_QRASTERMODULE_H

#include "Module.h"

namespace calenhad {
    namespace grid {
        class CubicSphere;
    }
    namespace module {

    class StructuredGrid : public Module {
        public:
            explicit StructuredGrid (const QString& type);
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
        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
