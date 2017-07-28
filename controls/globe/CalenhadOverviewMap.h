//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008 Torsten Rahn <tackat@kde.org>
//

#ifndef MARBLEOVERVIEWMAP_H
#define MARBLEOVERVIEWMAP_H

#include <QHash>
#include <QColor>
#include <QPixmap>
#include <QMouseEvent>
#include <memory>

#include "marble/GeoDataLatLonAltBox.h"
#include "marble/AbstractFloatItem.h"
#include "marble/DialogConfigurationInterface.h"
#include "marble/MarbleModel.h"
#include "CalenhadOverviewInterface.h"
#include "CalenhadPreview.h"

namespace Marble {
    class MarbleWidget;
}

namespace calenhad {
    namespace qmodule {
        class QModule;
    }
    namespace mapping {
        class CalenhadLayer;
    }
    namespace pipeline {
        class ImageRenderJob;
    }
    namespace controls {
        namespace globe {
            class OverviewMapConfigWidget;

/**
 * @short The class that creates an overview map.
 *
 */

            class CalenhadGlobe;
            class CalenhadOverviewMap : public CalenhadPreview {

            public:
                CalenhadOverviewMap ();

                explicit CalenhadOverviewMap (calenhad::qmodule::QModule* module, CalenhadGlobe* globe);

                virtual ~CalenhadOverviewMap ();

                void paintEvent (QPaintEvent* e);

            public slots:

                void setBounds (const icosphere::Bounds& bounds) override;

                void mouseDoubleClickEvent (QMouseEvent* e) override;


                QSize renderSize () override;

            protected:
                bool eventFilter (QObject* object, QEvent* e) override;

                calenhad::pipeline::ImageRenderJob* prepareRender () override;

                GeoDataCoordinates toLatLon (QPoint pos);

            protected slots:

                virtual void jobComplete (std::shared_ptr<QImage> image) override;


            private:

                //  QDialog* configDialog ();

                void drawGrid ();

                void drawBoundingBox ();

                int _renderWidth;
                int _renderHeight;

                void recolour ();
            };
        }
    }
}

#endif