//
// Created by martin on 16/01/17.
//

#ifndef CALENHAD_CALENHADGLOBEWIDGET_H
#define CALENHAD_CALENHADGLOBEWIDGET_H

#include <QtWidgets/QPushButton>
#include <QTimer>
#include <QSlider>
#include <QDialog>
#include <qwt/qwt_slider.h>
#include <QtWidgets/QLabel>
#include <QtCore/QMutex>
#include <geoutils.h>
#include "../../icosphere/Bounds.h"
#include "CalenhadGlobeConstants.h"
#include "mapping/CalenhadMapWidget.h"
#include "GlobeScaleWidget.h"

namespace GeographicLib {
    class Geodesic;
}

namespace calenhad {
    namespace nodeedit {
        class CalenhadToolBar;
    }
    namespace mapping {
        namespace projection {
            class Projection;
        }
    }
}

using namespace calenhad::mapping::projection;
namespace calenhad {
    namespace module {
        class Module;
    }
    namespace legend {
        class Legend;
    }

    namespace mapping {
        class CalenhadMapWidget;
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
            class CalenhadGlobeWidget : public QDialog {
            Q_OBJECT

            public:

                CalenhadGlobeWidget (QWidget* parent, module::Module* source);

                virtual ~CalenhadGlobeWidget ();

                void resizeEvent (QResizeEvent* e) override;

                bool isNavigatorVisible ();

                bool isZoomBarVisible ();

                bool isOverviewVisible ();

                bool isGraticuleVisible ();

                bool isScaleVisible ();

                calenhad::mapping::CalenhadMapWidget* globe ();

                calenhad::nodeedit::CalenhadToolBar* mouseToolBar();
                calenhad::nodeedit::CalenhadToolBar* viewToolBar();
                calenhad::nodeedit::CalenhadToolBar* mapWidgetsToolBar();

            public slots:

                void showContextMenu (const QPoint& pos);

                void showConfigDialog ();

                void invalidate ();

                void setZoom (const double& zoom);

                void showOverviewMap (const bool& show = true);

                void showZoomSlider (const bool& show = true);

                void setScalebarVisible (const bool& visible);

                void showNavigator (const bool& show = true);
                void showGraticule (const bool& show);
                void projectionSelected (const bool& selected);
                void updateConfig ();

                void initialise ();

                void captureGreyscale ();

            signals:

                void resized (const QSize& size);

                void viewChanged (const icosphere::Bounds&);

            protected:
                CalenhadNavigator* _navigator;
                QMenu* _contextMenu;
                QwtSlider* _zoomSlider;
                QPoint _moveFrom;
                GeographicLib::Geodesic* _geodesic;
                CalenhadGlobeConfigDialog* _configDialog;
                double zoom ();
                QLabel* _positionLabel{};
                calenhad::nodeedit::CalenhadToolBar* makeToolBar (const QString& name);

                QMenu* makeGlobeContextMenu  (const QPoint& pos);
                bool _graticuleVisible{};
                calenhad::mapping::CalenhadMapWidget* _globe, *_overview;
                icosphere::Bounds _bounds;
                graph::Graph* _graph;
                calenhad::controls::globe::GlobeScaleWidget* _scale{};

                calenhad::nodeedit::CalenhadToolBar* _viewToolbar{}, * _mouseToolbar{}, * _mapWidgetsToolbar{};
                QAction* _propertiesAction{}, * _mousePanAction{}, * _mouseZoomAction{}, * _mouseGotoAction{}, * _mousePlaceAction{},
                        * _showGraticuleAction{}, * _disableDoubleClickAction{}, * _disableDragAction{},
                        * _showNavigatorAction{}, * _showScaleAction{}, * _showOverviewAction{}, * _showZoomSliderAction{};
            };
        }
    }
}

Q_DECLARE_METATYPE (geoutils::CoordinatesFormat);
Q_DECLARE_METATYPE (calenhad::controls::globe::DatumFormat);



#endif // CALENHAD_CALENHADGLOBEWIDGET_H
