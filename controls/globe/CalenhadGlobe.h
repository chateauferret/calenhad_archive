//
// Created by martin on 16/01/17.
//

#ifndef CALENHAD_CALENHADMARBLEWIDGET_H
#define CALENHAD_CALENHADMARBLEWIDGET_H


#include <marble/MarbleWidget.h>
#include <QtWidgets/QPushButton>
#include <QTimer>
#include <QSlider>
#include <qwt/qwt_slider.h>
#include "libnoise/model/sphere.h"
#include <marble/GeoDataLatLonBox.h>
#include <QtWidgets/QLabel>
#include <QtCore/QMutex>
#include "../../icosphere/Bounds.h"
#include "CalenhadGlobeConstants.h"

namespace Marble {
    class MarbleMap;
}

namespace GeographicLib {
    class Geodesic;
}

namespace calenhad {
    namespace mapping {
        class CalenhadLayer;
    }
    namespace qmodule {
        class QModule;
    }
    namespace legend {
        class Legend;
    }
    namespace controls {
        namespace globe {

            class CalenhadOverviewMap;
            class CalenhadNavigator;
            class CalenhadGlobeContextMenu;
            class CalenhadGlobeConfigDialog;
            class NavigationEvent;

            class CalenhadGlobe : public QWidget {//}; Marble::MarbleWidget {
            Q_OBJECT

            public:

                CalenhadGlobe (calenhad::qmodule::QModule* source, QWidget* parent);

                virtual ~CalenhadGlobe ();

                void resizeEvent (QResizeEvent* e) override;

                void mousePressEvent (QMouseEvent* e) override;

                void mouseDoubleClickEvent (QMouseEvent* event);

                void mouseMoveEvent (QMouseEvent* e) override;

                void mouseReleaseEvent (QMouseEvent* e) override;

                void paintEvent (QPaintEvent* e) override;

                const CalenhadGlobeDragMode dragMode ();

                const CalenhadGlobeDoubleClickMode doubleClickMode ();

                bool isCompassVisible ();

                bool isZoomBarVisible ();

                bool isOverviewVisible ();

                bool isGraticuleVisible ();

                bool isFloatItemVisible (const QString& nameId);

                void setSensitivity (const double& sensitivity);

                double sensitivity ();

                Marble::GeoDataCoordinates centre ();

                void zoomInTo (const Marble::GeoDataLatLonBox& target);

                void zoomOutFrom (const Marble::GeoDataLatLonBox& target);

                const Marble::Projection& projection ();

                bool isInView (const Marble::GeoDataCoordinates& coordinates);

                void setLegend (calenhad::legend::Legend* legend);

                calenhad::legend::Legend* legend ();

                CoordinatesFormat coordinatesFormat ();

                DatumFormat datumFormat ();

                calenhad::qmodule::QModule* module ();

            public slots:

                void setMouseDragMode (const CalenhadGlobeDragMode& mouseMode);

                void setMouseDoubleClickMode (const CalenhadGlobeDoubleClickMode& mouseMode);

                void showContextMenu (const QPoint& pos);

                void showConfigDialog ();

                void invalidate ();

                void setZoom (const double& zoom);

                void changeView ();

                void showOverviewMap (const bool& show = true);

                void showZoomSlider (const bool& show = true);

                void setFloatItemVisible (const bool& visible = true);

                void setGraticuleVisible (const bool& visible = true);

                void showNavigator (const bool& show = true);

                void navigate (const NavigationEvent& e);

                void updateConfig ();

                void goTo (const Marble::GeoDataCoordinates& target);

                void setProjection (const Marble::Projection& projection);

                void renderingStart ();

                void renderingFinished ();

            signals:

                void resized (const QSize& size);

                void viewChanged (const icosphere::Bounds&);

            protected:
                CalenhadOverviewMap* _overview;
                CalenhadNavigator* _navigator;
                CalenhadGlobeContextMenu* _contextMenu;
                Marble::MarbleMap* _map;
                calenhad::qmodule::QModule* _source;
                calenhad::mapping::CalenhadLayer* _layer = nullptr;
                QTimer _renderTimer;
                QTimer _progressTimer;
                QwtSlider* _zoomSlider;
                QPoint _moveFrom;
                GeographicLib::Geodesic* _geodesic;
                double _sensitivity = 0.2;
                CalenhadGlobeDragMode _mouseDragMode;
                CalenhadGlobeDoubleClickMode _mouseDoubleClickMode;
                double _zoom;
                CalenhadGlobeConfigDialog* _configDialog;
                noise::model::Sphere sphereModel;

                double zoom ();

                void setFloatItemVisible (const bool& visible, const QString& nameId);

                bool _zoomDrag;
                Marble::GeoDataLatLonBox _zoomBox;
                QLabel* _positionLabel;

                QString geoLocationString (Marble::GeoDataCoordinates pos);

                int zoomFactorToRadius (const double& zoom);

                double radiusToZoomFactor (const int& radius);

                Marble::Projection _projection;
                QMutex mutex;
                bool _rendering;
                int pass = 0;

                CoordinatesFormat _coordinatesFormat;
                DatumFormat _datumFormat;

                QString toTraditional (double ang, unsigned int num_dec_places = 2);
            };
        }
    }
}

Q_DECLARE_METATYPE (calenhad::controls::globe::CoordinatesFormat);
Q_DECLARE_METATYPE (calenhad::controls::globe::DatumFormat);



#endif //CALENHAD_CALENHADMARBLEWIDGET_H
