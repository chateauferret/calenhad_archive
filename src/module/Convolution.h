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
        class Convolution : public Module {
        public:
            explicit Convolution (const QString& type = CalenhadServices::preferences() -> calenhad_module_raster);
            ~Convolution() override;
            bool isComplete() override;
            QString glsl() override;


        protected slots:

            void serialize (QDomElement& element) override;
            void inflate (const QDomElement& element) override;

        protected:
            calenhad::grid::CubicSphere* _buffer;

        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
