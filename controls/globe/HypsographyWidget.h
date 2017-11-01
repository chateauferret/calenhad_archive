//
// Created by martin on 30/10/17.
//

#ifndef CALENHAD_HYPSOGRAPHYWIDGET_H
#define CALENHAD_HYPSOGRAPHYWIDGET_H
#include <QtWidgets/QWidget>
#include <QOpenGLWidget>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

namespace calenhad {
    namespace mapping {
        class CalenhadMapWidget;
    }
    namespace controls {
        namespace globe {

            class Statistics {
            public:
                Statistics (const double& min, const double& max, const double& sum, const int& count);
                double _min, _max, _sum;
                int _count;
                double range();
                double mean();
                bool ok();
            };

            class HypsographyWidget : public QWidget {

            public:
                HypsographyWidget (calenhad::mapping::CalenhadMapWidget* globe, QWidget* parent = 0, HypsographyWidget* context = 0);
                virtual ~HypsographyWidget();
                Statistics statistics();

            public slots:
                void refresh();

            protected:
                calenhad::mapping::CalenhadMapWidget* _globe;
                QwtPlot* _plot;
                QwtPlotCurve* _curve;
                double _buckets [1000];
                double _heights [1000];
                Statistics _statistics;
                HypsographyWidget* _context;
            };
        }
    }
}

#endif //CALENHAD_HYPSOGRAPHYWIDGET_H
