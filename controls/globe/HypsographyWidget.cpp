//
// Created by martin on 30/10/17.
//

#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QVBoxLayout>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include "HypsographyWidget.h"
#include "../../mapping/CalenhadMapWidget.h"

using namespace calenhad::mapping;
using namespace calenhad::controls::globe;


HypsographyWidget::HypsographyWidget (CalenhadMapWidget* globe, QWidget* parent, HypsographyWidget* context) : QWidget (parent),
    _globe (globe),
    _plot (new QwtPlot()),
    _statistics (Statistics (0.0, 0.0, 0.0, 0)),
    _context (context) {
    setLayout (new QVBoxLayout());
    layout() -> addWidget (_plot);
    _curve = new QwtPlotCurve ("Frequency");
    _curve -> setCurveFitter (NULL);
    _curve -> attach (_plot);
    for (int i = 0; i < 1000; i++) {
        _heights[i] = i / 1000.0 - 1.0;
    }
    connect (_globe, &CalenhadMapWidget::rendered, this, &HypsographyWidget::refresh);
    _plot -> setAxisScale (QwtPlot::yLeft, 0, 100);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid -> attach (_plot);
    _plot -> setAxisTitle (QwtPlot::xBottom, "Frequency (cumulative)");
    _plot -> setAxisTitle (QwtPlot::yLeft, "Altitude");
    refresh();
}

HypsographyWidget::~HypsographyWidget() {

}

void HypsographyWidget::refresh() {
    GLfloat* buffer = _globe -> heightMapBuffer();
    if (! buffer) { return; }
    int n =  _globe -> heightMapSize().height() * _globe -> heightMapSize().width();
    double _min = 0.0, _max = 0.0, _sum = 0.0;
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (! isnan (buffer [i])) {
            if (buffer[i] < _min) { _min = buffer[i]; }
            if (buffer[i] > _max) { _max = buffer[i]; }
            _sum += buffer[i];
            count++;
        }
    }

    _statistics = Statistics (_min, _max, _sum, count);
    for (int i = 0; i < 1000; i++) {
        _buckets [i] = 0.0;
    }

    for (int i = 0; i < n; i++) {
        if (! isnan (buffer [i])) {
            double normalised = (buffer[i] - _min) / _statistics.range ();
            int h = _globe -> heightMapSize().height();
            int w = _globe -> heightMapSize().width();
            int bucket = (int) (normalised * 1000);
            if (bucket >= 0 && bucket < 1000) {
                int row = i / w;
                double lat = (((double) row / (double) h)) * M_PI - (M_PI / 2.0);
                double area = std::cos (lat) * M_PI / 2;
                std::cout << i << " " << h << " " << lat << " " << area <<" \n";
                _buckets [bucket] += area;
            }
        }
    }

    QVector<QPointF> points;
    if (_context) { points.append (QPointF (_context -> statistics()._min, 0.0)); }
    for (int i = 0; i < 1000; i++) {
        if (i > 0) {
            _buckets [i] += _buckets [i - 1];
        }
        QPointF point ((_statistics.range() * i / 1000.0) + _min, _buckets [i] * 100.0 / count);
        points.append (point);
    }
    if (_context) { points.append (QPointF (_context -> statistics()._max, 100.0)); }
    QPen pen (Qt::red);
    pen.setWidth (2);

    _curve -> setCurveAttribute (QwtPlotCurve::Fitted, true);
    _curve -> setPen (pen);
    _curve -> setStyle (QwtPlotCurve::Lines);
    _curve -> setRenderHint (QwtPlotItem::RenderAntialiased, true);
    if (_context) {
        _plot -> setAxisScale (QwtPlot::xBottom, _context -> statistics()._min, _context -> statistics()._max);
    } else {
        _plot->setAxisScale (QwtPlot::xBottom, _min, _max);
    }
    _curve -> setSamples (points);
    _plot -> replot();

}

Statistics HypsographyWidget::statistics () {
    return _statistics;
}

Statistics::Statistics (const double& min, const double& max, const double& sum, const int& count) : _min (min), _max (max), _sum (sum), _count (count) {

}

double Statistics::range () {
    return _max - _min;
}

double Statistics::mean () {
    return _sum / _count;
}

bool Statistics::ok () {
    return _max > _min && _count > 0;
}
