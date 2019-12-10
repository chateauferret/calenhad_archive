//
// Created by martin on 30/10/17.
//

#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QVBoxLayout>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include "HypsographyWidget.h"
#include "mapping/AbstractMapWidget.h"
#include "CalenhadGlobeConfigDialog.h"

using namespace calenhad::mapping;
using namespace calenhad::controls::globe;
using namespace calenhad::mapping;


HypsographyWidget::HypsographyWidget (AbstractMapWidget* globe, QWidget* parent) : QWidget (parent),
                                                                                   _globe (globe),
                                                                                   _plot (new QwtPlot()),
                                                                                   _statistics (globe -> statistics()) {
    setLayout (new QVBoxLayout());
    _plot -> setSizePolicy (QSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout() -> addWidget (_plot);
    _curve = new QwtPlotCurve ("Frequency");
    _curve -> setCurveFitter (NULL);
    _curve -> attach (_plot);
    for (int i = 0; i < 1000; i++) {
        _heights[i] = i / 1000.0 - 1.0;
    }

    _plot -> setAxisScale (QwtPlot::yLeft, 0, 100);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid -> attach (_plot);
    _plot -> setAxisTitle (QwtPlot::xBottom, "Frequency (cumulative)");
    _plot -> setAxisTitle (QwtPlot::yLeft, "Altitude");
}

HypsographyWidget::~HypsographyWidget() {

}

void HypsographyWidget::showEvent (QShowEvent* e) {
    refresh();
}

void HypsographyWidget::refresh() {
    GLfloat* buffer = _globe -> heightMapBuffer();
    if (! buffer) { return; }
    int n =  _globe -> heightMapSize().height() * _globe -> heightMapSize().width();

    _statistics = _globe -> statistics();
    for (int i = 0; i < 1000; i++) {
        _buckets [i] = 0.0;
    }

    for (int i = 0; i < n; i++) {
        if (! isnan (buffer [i])) {
            double normalised = (buffer[i] - _statistics._min) / _statistics.range ();
            int h = _globe -> heightMapSize().height();
            int w = _globe -> heightMapSize().width();
            int bucket = (int) (normalised * 1000);
            if (bucket >= 0 && bucket < 1000) {
                int row = i / w;
                double lat = (((double) row / (double) h)) * M_PI - (M_PI / 2.0);
                double area = std::cos (lat) * M_PI / 2;
                _buckets [bucket] += area;
            }
        }
    }

    QVector<QPointF> points;
    for (int i = 0; i < 1000; i++) {
        if (i > 0) {
            _buckets [i] += _buckets [i - 1];
        }
        QPointF point ((_statistics.range() * i / 1000.0) + _statistics._min, _buckets [i] * 100.0 / _statistics._count);
        points.append (point);
    }
    QPen pen (Qt::red);
    pen.setWidth (2);

    _curve -> setCurveAttribute (QwtPlotCurve::Fitted, true);
    _curve -> setPen (pen);
    _curve -> setStyle (QwtPlotCurve::Lines);
    _curve -> setRenderHint (QwtPlotItem::RenderAntialiased, true);
    _plot -> setAxisScale (QwtPlot::xBottom, _statistics._min, _statistics._max);
    _curve -> setSamples (points);
    _plot -> replot();

}

Statistics HypsographyWidget::statistics () {
    return _statistics;
}