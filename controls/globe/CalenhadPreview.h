//
// Created by martin on 02/05/17.
//

#ifndef CALENHAD_CALENHADOVERVIEW_H
#define CALENHAD_CALENHADOVERVIEW_H

#include "marble/GeoDataLatLonBox.h"
#include "icosphere/Bounds.h"
#include "CalenhadGlobeConstants.h"
#include <QWidget>

namespace calenhad {
    namespace pipeline {
        class ImageRenderJob;
    }
    namespace qmodule {
        class QModule;
    }
    namespace controls {
        namespace globe {



            using namespace Marble;

            class CalenhadPreview : public QWidget {
            Q_OBJECT
            public:
                CalenhadPreview (calenhad::qmodule::QModule* module, QWidget* parent = 0);

                virtual ~CalenhadPreview ();

                bool isRendered ();

                void showEvent (QShowEvent*) override;

                calenhad::qmodule::QModule* source ();

                void setSource (calenhad::qmodule::QModule* qm);

                std::shared_ptr<QImage> image ();

                virtual void initialise ();

                virtual QSize renderSize ();

                icosphere::Bounds bounds ();

            public slots:

                void render ();

                virtual void setBounds (const icosphere::Bounds& bounds);

            signals:

                void renderComplete (std::shared_ptr<QImage> image);

            protected:
                virtual calenhad::pipeline::ImageRenderJob* prepareRender ();

                std::shared_ptr<QImage> _image;
                QPixmap _pixmap;
                icosphere::Bounds _bounds;
                OverviewPreviewType _previewType;
                bool _isRendered;
                double _ratio;
                calenhad::qmodule::QModule* _source;
            protected slots:

                virtual void jobComplete (std::shared_ptr<QImage> image);
            };
        }
    }
}


#endif //CALENHAD_CALENHADOVERVIEW_H
