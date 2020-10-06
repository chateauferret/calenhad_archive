//
// Created by martin on 30/10/17.
//

#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QVBoxLayout>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_grid.h>
#include "HypsographyWidget.h"
#include "mapping/CalenhadMapWidget.h"
#include "CalenhadGlobeConfigDialog.h"
#include "../grid/CubicSphere.h"

using namespace calenhad::mapping;
using namespace calenhad::controls::globe;
using namespace calenhad::mapping;
using namespace calenhad::grid;


HypsographyWidget::HypsographyWidget (CalenhadMapWidget* globe, QWidget* parent) : QwtPlot (parent), _globe (globe) {

    setSizePolicy (QSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding));
    _curve = new QwtPlotCurve ("Frequency");
    _curve -> setCurveFitter (nullptr);
    _curve -> attach (this);

    setAxisScale (QwtPlot::xBottom, 0, 100);
    QwtPlotGrid* grid = new QwtPlotGrid();
    grid -> attach (this);
    setAxisTitle (QwtPlot::xBottom, "Frequency (cumulative)");
    setAxisTitle (QwtPlot::yLeft, "QuarticSpline");
}

HypsographyWidget::~HypsographyWidget() {

}

void HypsographyWidget::showEvent (QShowEvent* e) {
   // refresh();
}

void HypsographyWidget::refresh() {

    QVector<QPointF> points;
    points.reserve (1000);

    CubicSphere* buffer = _globe -> buffer();
    unsigned _buckets [1000];
    
    if (! buffer) { return; }
    _statistics = CalenhadServices::compute() -> statistics();

    // our buckets need to be cumulative
    _buckets [0] = _statistics._buckets [0];
    for (int i = 0; i < 1000; i++) {
        if (i >= 1) {
            _buckets [i] = _buckets [i - 1] + _statistics._buckets [i];
        }
        QPointF point (_buckets [i] * 100.0 / _statistics._valueCount, _statistics._range * ((double) i / 1000.0) + _statistics._minValue);
        points.append (point);
    }
    QPen pen (Qt::red);
    pen.setWidth (2);
    _curve -> setPen (pen);
    setAxisScale (QwtPlot::yLeft, _statistics._minValue, _statistics._maxValue);

    _curve -> setCurveAttribute (QwtPlotCurve::Fitted, true);
    _curve -> setStyle (QwtPlotCurve::Lines);
    _curve -> setRenderHint (QwtPlotItem::RenderAntialiased, true);
    _curve -> setSamples (points);
    replot();

}

CalenhadStatistics HypsographyWidget::statistics () {
    return _statistics;
}

