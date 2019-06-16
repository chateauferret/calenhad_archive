//
// Created by martin on 16/01/17.
//

#ifndef CALENHAD_CALENHADMARBLEWIDGET_H
#define CALENHAD_CALENHADMARBLEWIDGET_H

#include <QtWidgets/QPushButton>
#include <QTimer>
#include <QSlider>
#include <QDialog>
#include <qwt/qwt_slider.h>
#include <QtWidgets/QLabel>
#include <QtCore/QMutex>
#include <geoutils.h>
#include <mapping/CalenhadMapWidget.h>
#include "../../icosphere/Bounds.h"
#include "CalenhadGlobeConstants.h"
#include "../../mapping/CalenhadMapWidget.h"
#include "GlobeScaleWidget.h"

namespace GeographicLib {
    class Geodesic;
}

namespace calenhad {
    namespace mapping {
        namespace projection {
            class Projection;
        }
    }
}

using namespace calenhad::mapping::projection;
namespace calenhad {
    namespace qmodule {
        class Module;
    }
    namespace legend {
        class Legend;
    }

    namespace controls {
        namespace legend {
            class LegendEditorScale;
        }
        namespace globe {

            class CalenhadNavigator;
            class CalenhadGlobeContextMenu;
            class CalenhadGlobeConfigDialog;
            class NavigationEvent;
            class CalenhadGlobeDialog : public QDialog {
            Q_OBJECT

            public:

                CalenhadGlobeDialog (QWidget* parent, qmodule::Module* source);

                virtual ~CalenhadGlobeDialog ();

                void resizeEvent (QResizeEvent* e) override;

                bool isNavigatorVisible ();

                bool isZoomBarVisible ();

                bool isOverviewVisible ();

                bool isGraticuleVisible ();

                bool isScaleVisible ();

                calenhad::mapping::CalenhadMapWidget* globe ();

            public slots:

                void showContextMenu (const QPoint& pos);

                void showConfigDialog ();

                void invalidate ();

                void setZoom (const double& zoom);

                void showOverviewMap (const bool& show = true);

                void showZoomSlider (const bool& show = true);

                void setScalebarVisible (const bool& visible);

                void showNavigator (const bool& show = true);

                void updateConfig ();

                void initialise ();

                void captureGreyscale ();

            signals:

                void resized (const QSize& size);

                void viewChanged (const icosphere::Bounds&);



            protected:
                CalenhadNavigator* _navigator;
                CalenhadGlobeContextMenu* _contextMenu;
                QwtSlider* _zoomSlider;
                QPoint _moveFrom;
                GeographicLib::Geodesic* _geodesic;
                CalenhadGlobeConfigDialog* _configDialog;
                double zoom ();
                QLabel* _positionLabel;


                //Marble::Projection _projection;
                bool _graticuleVisible;
                calenhad::mapping::CalenhadMapWidget* _globe, *_overview;
                icosphere::Bounds _bounds;
                graph::Graph* _graph;
                calenhad::controls::globe::GlobeScaleWidget* _scale;

            };
        }
    }
}

Q_DECLARE_METATYPE (geoutils::CoordinatesFormat);
Q_DECLARE_METATYPE (calenhad::controls::globe::DatumFormat);



#endif //CALENHAD_CALENHADMARBLEWIDGET_H
