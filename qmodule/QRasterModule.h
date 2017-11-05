//
// Created by martin on 03/11/17.
//

#ifndef CALENHAD_QRASTERMODULE_H
#define CALENHAD_QRASTERMODULE_H

#include "QModule.h"

namespace calenhad {
    namespace qmodule {
        class QRasterModule : public QModule {
        public:
            QRasterModule (QModule* parent = 0);
            virtual ~QRasterModule();
            void setRaster (const QPixmap& raster);

            void initialise () override;
        protected:
            QString _filename;
            QPixmap* _raster;

        protected slots:
            void fileDialogRequested();

            void openFile (const QString& filename);
        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
