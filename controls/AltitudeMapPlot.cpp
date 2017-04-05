//
// Created by martin on 31/03/17.
//

#include <QtCore/QEvent>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_curve_fitter.h>
#include <iostream>
#include "AltitudeMapPlot.h"
#include "AltitudeMapFitter.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"
#include <QMouseEvent>

AltitudeMapPlot::AltitudeMapPlot (QWidget *parent) : QwtPlot (parent), _curve (new QwtPlotCurve()), _drag (false), _index (noneSelected), _fitter (new AltitudeMapFitter()), _symbol (new QwtSymbol) {
    setTitle ("Altitude map");
    setCanvas (new AltitudeMapPlotCanvas());
    canvas() -> setMouseTracking (true);
    setAxisScale ( QwtPlot::yLeft, -1.0, 1.0);
    setAxisScale ( QwtPlot::xBottom, -1.0, 1.0);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid -> attach (this);
    resize (600, 400);

    _symbol -> setStyle(QwtSymbol::Rect);
    _symbol -> setSize(10, 10);
    _symbol -> setBrush (Qt::red);
    _symbol -> setPen (Qt::black);
}

AltitudeMapPlot::~AltitudeMapPlot() {
    _curve -> detach(); // automatically deletes the curve
    delete _symbol;
    delete _fitter;

}

void AltitudeMapPlot::changeEvent (QEvent *e) {

}

void AltitudeMapPlot::setEntries (const QVector<QPointF>& entries) {
    _entries = entries;
    _fitter -> setSplineSize (100);
    std::cout << "\n";
    //_fitter -> setFitMode (_fitter -> Spline);
    _curve -> setCurveFitter (_fitter);
    QPen pen( Qt::red);
    pen.setWidth (2);
    _curve -> setSymbol (_symbol);
    _curve -> setCurveAttribute (QwtPlotCurve::Fitted, true);
    _curve -> setPen (pen);
    _curve -> setStyle (QwtPlotCurve::Lines);
    _curve -> setRenderHint (QwtPlotItem::RenderAntialiased, true);
    _curve -> setSamples (_entries);
    _curve -> attach (this);
}

QVector<QPointF> AltitudeMapPlot::getEntries () {
    std::cout << "Found " << _entries.size () << " entries" << "\n";
     return _entries;
}

void AltitudeMapPlot::mouseCanvasPressEvent (QMouseEvent* event) {
    double x = invTransform (xBottom, event -> pos().x());
    double y = invTransform (yLeft, event -> pos().y());
    if (_index == noneSelected) {
        _entries.append (QPointF (x, y));
        std::sort (_entries.begin(), _entries.end(), [] (const QPointF & a, const QPointF & b) -> bool { return a.x() < b.x(); });
        _curve -> setSamples (_entries);// (x, y, _entries.size());
    }
    replot();
}


bool AltitudeMapPlot::isOnCanvas (QPointF point) {
    // return true if dropping a control point here would delete it - that is if it's outside the margins of the canvas and if there would still be enough points
    // left to make a curve.
    return (point.x() >= - 1 && point.x() <= 1 && point.y() >= - 1 && point.y() <= 1);
}

void AltitudeMapPlot::mouseCanvasReleaseEvent (QMouseEvent* event) {
    // if the control point was dragged off the canvas, we delete it, provided we retain enough control points to make a curve
    if (_entries.size() > 4) {
        QPointF point = toPlotCoordinates (event->x (), event->y ());
        if (!isOnCanvas (point)) {
            _entries.remove (_index);
            _index = noneSelected;
            _curve->setSamples (_entries);
            replot ();
        }
    }
}

void AltitudeMapPlot::mouseCanvasMoveEvent (QMouseEvent* event) {
    double dist;
    if (event -> buttons() && Qt::LeftButton) {
        if (_index != noneSelected) {
            QPointF point = toPlotCoordinates (event -> x(), event -> y());
            if (! isOnCanvas (point)) {
                if (_entries.size() > 4) {
                    // we can delete the point so show what the curve looks like without it
                    canvas ()->setCursor (Qt::ForbiddenCursor);
                    QVector<QPointF> tempEntries = _entries;
                    tempEntries.remove (_index);
                    std::sort (tempEntries.begin (), tempEntries.end (), [] (const QPointF& a, const QPointF& b) -> bool { return a.x () < b.x (); });
                    _curve->setSamples (tempEntries);
                } else {
                    // moved off the canvas, but can't drop the control point because of the minimum number of control points required.
                    // So keep the control point and keep it on the canvas.
                    if (point.x() < -1 ) { point.setX (-1); }
                    if (point.x() > 1 ) { point.setX (1); }
                    if (point.y() < -1 ) { point.setY (-1); }
                    if (point.y() > 1 ) { point.setY (1); }
                    // if the  control point stays on the canvas we just move it about, reordering the points if one is moved past another in the x direction
                    updatePoint (point);
                    _index = _curve -> closestPoint (event->pos (), &dist);
                }
                replot ();
            } else {
                canvas() -> setCursor (Qt::ClosedHandCursor);
                // if the  control point stays on the canvas we just move it about, reordering the points if one is moved past another in the x direction
                updatePoint (point);
                _index = _curve -> closestPoint (event->pos (), &dist);
            }
        }
    } else {
        _index = _curve -> closestPoint (event -> pos (), &dist);
        if (dist < 8) {
            canvas() -> setCursor (Qt::OpenHandCursor);
        } else {
            canvas() -> setCursor (Qt::CrossCursor);
            _index = noneSelected;
        }
    }
}

// Updates the coordinates of the point identified by _index to move it to the coordinates of the supplied QPointF.
void AltitudeMapPlot::updatePoint (QPointF point) {
    _entries.replace (_index, point);
    std::sort (_entries.begin(), _entries.end(), [] (const QPointF& a, const QPointF& b) -> bool { return a.x() < b.x(); });
    _curve -> setSamples (_entries);
    replot ();
}

QPointF AltitudeMapPlot::toPlotCoordinates (const int& pixelX, const int& pixelY) {
    double x = invTransform (xBottom, pixelX);
    double y = invTransform (yLeft, pixelY);
    return QPointF (x, y);
}


void AltitudeMapPlotCanvas::mousePressEvent (QMouseEvent* event) {
    dynamic_cast<AltitudeMapPlot*> (plot()) -> mouseCanvasPressEvent (event);
}

void AltitudeMapPlotCanvas::mouseMoveEvent (QMouseEvent* event) {
    dynamic_cast<AltitudeMapPlot*> (plot()) -> mouseCanvasMoveEvent (event);
}

void AltitudeMapPlotCanvas::mouseReleaseEvent (QMouseEvent* event) {
    dynamic_cast<AltitudeMapPlot*> (plot()) -> mouseCanvasReleaseEvent (event);
}
