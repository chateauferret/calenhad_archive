//
// Created by martin on 30/10/17.
//

#ifndef CALENHAD_HYPSOGRAPHYWIDGET_H
#define CALENHAD_HYPSOGRAPHYWIDGET_H
#include <QtWidgets/QWidget>
#include <QOpenGLWidget>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <mapping/Statistics.h>

namespace calenhad {
    namespace mapping {
        class CalenhadMapWidget;
    }
    namespace controls {
        namespace globe {

            class HypsographyWidget : public QWidget {

            public:
                HypsographyWidget (calenhad::mapping::CalenhadMapWidget* globe, QWidget* parent = 0);
                virtual ~HypsographyWidget();
                calenhad::mapping::Statistics statistics();

            public slots:
                void refresh();

            protected:
                calenhad::mapping::CalenhadMapWidget* _globe;
                QwtPlot* _plot;
                QwtPlotCurve* _curve;
                double _buckets [1000];
                double _heights [1000];
                calenhad::mapping::Statistics _statistics;
            };
        }
    }
}

#endif //CALENHAD_HYPSOGRAPHYWIDGET_H
