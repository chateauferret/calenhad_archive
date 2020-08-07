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

            virtual QImage* raster() = 0;

            bool isComplete() override;
            QString glsl() override;

        protected:

            QImage* _raster;
            QWidget* _rasterContent;

        protected slots:

            void serialize (QDomElement& element) override;
            void inflate (const QDomElement& element) override;

        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
