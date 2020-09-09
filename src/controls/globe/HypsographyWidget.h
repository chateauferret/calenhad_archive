//
// Created by martin on 30/10/17.
//

#ifndef CALENHAD_HYPSOGRAPHYWIDGET_H
#define CALENHAD_HYPSOGRAPHYWIDGET_H
#include <QtWidgets/QWidget>
#include <QOpenGLWidget>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include "CalenhadStatistics.h"


namespace calenhad {
    namespace module {
        class Module;
    }
    namespace mapping {
       class CalenhadMapWidget;
    }
    namespace controls {
        namespace globe {
            class HypsographyWidget : public QwtPlot {

            public:
                explicit HypsographyWidget (calenhad::mapping::CalenhadMapWidget* globe, QWidget* parent = nullptr);
                ~HypsographyWidget() override;

                CalenhadStatistics statistics ();

            public slots:
                void refresh();

            protected:
                calenhad::mapping::CalenhadMapWidget* _globe;
                QwtPlotCurve* _curve;

                calenhad::controls::globe::CalenhadStatistics _statistics;

                void showEvent (QShowEvent* e) override;
            };
        }
    }
}

#endif //CALENHAD_HYPSOGRAPHYWIDGET_H
