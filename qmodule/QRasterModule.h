//
// Created by martin on 03/11/17.
//

#ifndef CALENHAD_QRASTERMODULE_H
#define CALENHAD_QRASTERMODULE_H

#include "QModule.h"
#include "../icosphere/Bounds.h"

namespace calenhad {
    namespace controls {
        class QAngleControl;
    }
    namespace qmodule {
        class QRasterModule : public QModule {
        public:
            QRasterModule (QModule* parent = 0);
            virtual ~QRasterModule();
            void setRaster (const QImage& raster);
            QImage* raster();
            void initialise() override;
            bool isComplete() override;
            icosphere::Bounds bounds();
        protected:
            QString _filename;
            QImage* _raster;
            QLabel* _filenameLabel;
            QFormLayout* _rasterLayout;
            QGridLayout* _boundsLayout;
            QWidget* _rasterContent, * _boundsContent;
            icosphere::Bounds _bounds;
            calenhad::controls::QAngleControl* _northBoundsText, * _southBoundsText, * _eastBoundsText, * _westBoundsText;

        protected slots:
            void fileDialogRequested();
            void openFile (const QString& filename);
            void serialize (QDomDocument& doc);
            void inflate (const QDomElement& element);
            void updateBounds();
        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
