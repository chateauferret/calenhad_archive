//
// Created by martin on 02/05/17.
//

#ifndef CALENHAD_CALENHADOVERVIEW_H
#define CALENHAD_CALENHADOVERVIEW_H

#include "icosphere/Bounds.h"
#include "CalenhadGlobeConstants.h"
#include "../../mapping/CalenhadMapWidget.h"
#include <QWidget>
#include <memory>
#include "../../geoutils.h"

namespace calenhad {
    namespace pipeline {
        class ImageRenderJob;
    }
    namespace qmodule {
        class QModule;
    }
    namespace controls {
        namespace globe {

            enum OverviewPreviewType {
                WholeWorld, ExplorerBounds
            };

            class CalenhadMapView : public calenhad::mapping::CalenhadMapWidget {
            Q_OBJECT

            public:
                CalenhadMapView (QWidget* parent = 0);

                virtual ~CalenhadMapView ();

                calenhad::qmodule::QModule* source ();

                void setSource (calenhad::qmodule::QModule* qm);

                void zoomInTo (const icosphere::Bounds& target);

                void zoomOutFrom (const icosphere::Bounds& target);

                CalenhadGlobeDoubleClickMode mouseDoubleClickMode();
                CalenhadGlobeDragMode mouseDragMode();

                void setDatumFormat (DatumFormat format);
                DatumFormat datumFormat();
                void setCoordinatesFormat (CoordinatesFormat format);
                CoordinatesFormat coordinatesFormat();
                void setSensitivity (double sensitivity);
                double sensitivity();
                void goTo (const geoutils::Geolocation& geolocation);

            public slots:

                void render ();
                void setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mode);
                void setMouseDragMode (const CalenhadGlobeDragMode& mode);

            signals:
                void zoomRequested (const double& zoom);


            protected:
                icosphere::Bounds _bounds;
                OverviewPreviewType _previewType;
                double _ratio;
                calenhad::qmodule::QModule* _source;
                void showEvent (QShowEvent* e) override;

                icosphere::Bounds _zoomBox;
                bool _zoomDrag;

                double _sensitivity = 0.2;
                CoordinatesFormat _coordinatesFormat;
                DatumFormat _datumFormat;

                void mousePressEvent (QMouseEvent* e);

                void mouseDoubleClickEvent (QMouseEvent* e);

                void mouseMoveEvent (QMouseEvent* e);

                void mouseReleaseEvent (QMouseEvent* e);

                QPoint _moveFrom;
                CalenhadGlobeDragMode _mouseDragMode;
                CalenhadGlobeDoubleClickMode _mouseDoubleClickMode;

                QString geoLocationStringDecimal (const geoutils::Geolocation& loc);

                QString geoLocationStringTraditional (const geoutils::Geolocation& loc);

                void wheelEvent (QWheelEvent* event);
            };
        }
    }
}


#endif //CALENHAD_CALENHADOVERVIEW_H
