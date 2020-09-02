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
#include <src/geoutils.h>
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
    namespace graph {
        class Graph;
    }
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
            class GlobeScaleWidget;
            class CalenhadNavigator;
            class CalenhadGlobeContextMenu;
            class CalenhadGlobeConfigDialog;
            class NavigationEvent;
            class CalenhadGlobeDialog;

            class CalenhadGlobeWidget : public QWidget {
            Q_OBJECT


            public:

                CalenhadGlobeWidget (CalenhadGlobeDialog* parent, module::Module* source);

                virtual ~CalenhadGlobeWidget ();

                void resizeEvent (QResizeEvent* e) override;

                bool isNavigatorVisible ();

                bool isZoomBarVisible ();

                bool isOverviewVisible ();
                void updateModules();


                bool isGraticuleVisible ();

                bool isScaleVisible ();
                void setModule (calenhad::module::Module* module);
                calenhad::mapping::CalenhadMapWidget* globe ();

                calenhad::nodeedit::CalenhadToolBar* mouseToolBar();
                calenhad::nodeedit::CalenhadToolBar* viewToolBar();
                calenhad::nodeedit::CalenhadToolBar* mapWidgetsToolBar();

            public slots:

                void showContextMenu (const QPoint& pos);
                void moduleSelected (const QString& name);
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

                void captureGreyscale ();

            signals:

                void resized (const QSize& size);

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
                QMenu* _projectionMenu;
                graph::Graph* _graph;
                calenhad::controls::globe::GlobeScaleWidget* _scale{};
                QComboBox* _selectModuleCombo{}, * _selectLegendCombo{}, * _selectProjectionCombo;
                calenhad::nodeedit::CalenhadToolBar* _viewToolbar{}, * _mouseToolbar{}, * _mapWidgetsToolbar{};
                QAction* _propertiesAction{}, * _mousePanAction{}, * _mouseZoomAction{}, * _mouseGotoAction{}, * _mousePlaceAction{},
                        * _showGraticuleAction{}, * _disableDoubleClickAction{}, * _disableDragAction{}, *_exportImagesAction{},
                        * _showNavigatorAction{}, * _showScaleAction{}, * _showOverviewAction{}, * _showZoomSliderAction{};
            };
        }
    }
}

Q_DECLARE_METATYPE (geoutils::CoordinatesFormat);
Q_DECLARE_METATYPE (calenhad::controls::globe::DatumFormat);



#endif // CALENHAD_CALENHADGLOBEWIDGET_H
