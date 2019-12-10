//
// Created by martin on 03/11/17.
//

#ifndef CALENHAD_QRASTERMODULE_H
#define CALENHAD_QRASTERMODULE_H

#include "Module.h"
#include "../icosphere/Bounds.h"

namespace calenhad {
    namespace controls {
        class QAngleControl;
    }
    namespace module {
        class RasterModule : public Module {
        public:
            explicit RasterModule (const QString& type = CalenhadServices::preferences ()->calenhad_module_raster);
            ~RasterModule() override;

            virtual QImage* raster() = 0;
            void initialise() override;
            bool isComplete() override;
            grid::Bounds bounds();
            QString glsl() override;

        protected:

            QImage* _raster;


            QGridLayout* _boundsLayout;
            QWidget* _rasterContent, * _boundsContent;
            grid::Bounds _bounds;
            calenhad::controls::QAngleControl* _northBoundsText, * _southBoundsText, * _eastBoundsText, * _westBoundsText;

        protected slots:

            void serialize (QDomElement& element) override;
            void inflate (const QDomElement& element) override;
            void updateBounds();

            void setBounds (grid::Bounds bounds);

            bool isBoundsValid ();
        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
