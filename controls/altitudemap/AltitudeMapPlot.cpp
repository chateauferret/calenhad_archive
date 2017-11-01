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
#include "nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "TerraceMapFitter.h"
#include <QMouseEvent>

using namespace calenhad::controls::altitudemap;

AltitudeMapPlot::AltitudeMapPlot (int resolution, QWidget *parent) : QwtPlot (parent),
        _curve (new QwtPlotCurve()),
        _drag (false),
        _index (noneSelected),
        _symbol (new QwtSymbol),
        _fitter (nullptr),
        _resolution (resolution) {
    setTitle ("Altitude map");
    _canvas = new AltitudeMapPlotCanvas();
    setCanvas (_canvas);
    _panner = new QwtPlotPanner (_canvas);
    _zoomer = new QwtPlotMagnifier (_canvas);
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
    if (_fitter) { delete _fitter; }
    if (_zoomer) { delete _zoomer; }
    if (_panner) { delete _panner; }
}

void AltitudeMapPlot::changeEvent (QEvent *e) {

}

void AltitudeMapPlot::setEntries (const QVector<QPointF>& entries) {
    _entries = entries;
    QPen pen (Qt::red);
    pen.setWidth (2);
    _curve -> setSymbol (_symbol);
    _curve -> setCurveAttribute (QwtPlotCurve::Fitted, true);
    _curve -> setPen (pen);
    _curve -> setStyle (QwtPlotCurve::Lines);
    _curve -> setRenderHint (QwtPlotItem::RenderAntialiased, true);

    if (_curveType == CurveType::Altitude) {
        _curve -> setSamples (_entries);
    } else {
        _curve -> setSamples (remapTerrace());
    }
    _curve -> attach (this);

}

// for a terrace, the points always fall on the line x = y, but the owner computes values between the points such that the gradient
// from each point increases (or decreases) with the square of the distance from the point with the nearest lower x. This method remaps
// the y value to match the x in that case, but without destroying any y values already held, in case we want to revert to an altitude curve.
QVector<QPointF> AltitudeMapPlot::remapTerrace() {
    QVector<QPointF> remapped;
    for (QPointF point : _entries) {
        remapped << QPointF (point.x(), point.x());
    }
    return remapped;
}

QVector<QPointF> AltitudeMapPlot::getEntries () {
    if (_curveType == CurveType::Altitude) {
        return _entries;
    } else {
        return remapTerrace();
    }
}

void AltitudeMapPlot::mouseCanvasPressEvent (QMouseEvent* event) {
    double x = invTransform (xBottom, event -> pos().x());
    double y = invTransform (yLeft, event -> pos().y());
    double dist;
    _index = _curve -> closestPoint (event->pos (), &dist);
    if (dist < 8) {
        canvas()->setCursor (Qt::ClosedHandCursor);
    } else {
        _index = noneSelected;
        _entries.append (QPointF (x, y));
        sortEntries (_entries);
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
    // if the control point was dragged off the canvas, we delete it, provided the deletion preserves the curve's integrity as defined by canDeleteSelected()
    if (canDeleteSelected()) {
        QPointF point = toPlotCoordinates (event -> x (), event -> y ());
        if (!isOnCanvas (point)) {
            _entries.remove (_index);
            _index = noneSelected;
            _curve -> setSamples (_entries);
            replot ();
        }
    }

    // put the mouse cursor back how it was
    if (_index == noneSelected) {
        canvas() -> setCursor (Qt::CrossCursor);
    } else {
        canvas() -> setCursor (Qt::OpenHandCursor);
    }
}

void AltitudeMapPlot::mouseCanvasMoveEvent (QMouseEvent* event) {
    double dist;
    if (event -> buttons() && Qt::LeftButton) {
        if (_index != noneSelected) {
            QPointF point = toPlotCoordinates (event -> x(), event -> y());
            if (! isOnCanvas (point)) {
                // don't allow deletion of the first or last points in the curve or if there would be too few points left to define it
                if (canDeleteSelected()) {
                    // we can delete the point so show what the curve would look like without it - but keep this in a temporary object so that we can revert
                    // the change if the user brings the point back onto the canvas.
                    canvas () -> setCursor (Qt::ForbiddenCursor);
                    QVector<QPointF> tempEntries;
                    for (int i = 0; i < _entries.size(); i++) {
                        if (i != _index) {
                            tempEntries << QPointF (_entries.at (i));
                        }
                    }
                    sortEntries (tempEntries);
                    _curve -> setSamples (tempEntries);
                } else {
                    // moved off the canvas, but can't drop the control point because of the minimum number of control points required.
                    // So keep the control point and keep it on the canvas.
                    if (point.x() < -1 ) { point.setX (-1); }
                    if (point.x() > 1 ) { point.setX (1); }
                    if (point.y() < -1 ) { point.setY (-1); }
                    if (point.y() > 1 ) { point.setY (1); }
                    // if the  control point stays on the canvas we just move it about, reordering the points if one is moved past another in the x direction
                    updatePoint (point);
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
            _panner -> setEnabled (false);
        } else {
            canvas() -> setCursor (Qt::CrossCursor);
            _index = noneSelected;
            _panner -> setEnabled (true);
        }
    }
}

// Updates the coordinates of the point identified by _index to move it to the coordinates of the supplied QPointF.
void AltitudeMapPlot::updatePoint (QPointF point) {

    // the X values of the first and last points in the curve to be clamped to -1 and 1 respectively so that there are no gaps at either end.
    // We can still move the point in the Y direction though.
    if (_index == 0) {
        point.setX (-1);
    }
    if (_index == _entries.size() - 1) {
        point.setX (1);
    }

    // no two points are to have the same X value
    if (_index > 0) {
        if (point.x () == _entries.at (_index - 1).x ()) {
            point.setX (_entries.at (_index - 1).x () + (2.0d / _resolution));
        }
    }

    if (_index < _entries.size () - 1) {
        if (point.x () == _entries.at (_index + 1).x ()) {
            point.setX (_entries.at (_index + 1).x () - (2.0d / _resolution));
        }
    }


    // update the point with the new position and reproduce the curve showing the change.
    _entries.replace (_index, point);
    if (isOnCanvas (point)) {
        sortEntries (_entries);
    }
    _curve -> setSamples (_entries);
    replot ();
}

QPointF AltitudeMapPlot::toPlotCoordinates (const int& pixelX, const int& pixelY) {
    double x = invTransform (xBottom, pixelX);
    double y = invTransform (yLeft, pixelY);
    return QPointF (x, y);
}

void AltitudeMapPlot::sortEntries (QVector<QPointF>& entries) {
    std::sort (entries.begin(), entries.end(), [] (const QPointF& a, const QPointF& b) -> bool { return a.x() < b.x(); });
}

void AltitudeMapPlot::setCurveType (CurveType type) {
    _curveType = type;
    if (type == CurveType::Altitude) {
        AltitudeMapFitter* f = new AltitudeMapFitter();
        _fitter = f;
        f -> setSplineSize (_resolution);
    } else {
        TerraceMapFitter* f = new TerraceMapFitter();
        f -> attachPlot (this);
        _fitter = f;
        f -> setResolution (_resolution);
        f -> setInverted (type == CurveType::InvertedTerrace);
    }
    _curve -> setCurveFitter (_fitter);
    _curve -> setSamples (_entries);
    replot();
}

// Tell whether the point selected (the one identified by _index) may be deleted, returning false if to do so would leave the curve unusable.
// A point cannot be deleted if it is at either end of the curve or if too few points would remain for y to be computed for any value of x.
bool AltitudeMapPlot::canDeleteSelected () {
   return  _entries.size() > 4 && _index > 0 && _index < _entries.size() - 1;
}

CurveType AltitudeMapPlot::curveType () {
    return _curveType;
}



AltitudeMapPlotCanvas::AltitudeMapPlotCanvas() : QwtPlotCanvas() {

}

AltitudeMapPlotCanvas::~AltitudeMapPlotCanvas() {

}

void AltitudeMapPlotCanvas::mouseDoubleClickEvent (QMouseEvent* event) {
    dynamic_cast<AltitudeMapPlot*> (plot()) -> mouseCanvasPressEvent (event);
}

void AltitudeMapPlotCanvas::mouseMoveEvent (QMouseEvent* event) {
    dynamic_cast<AltitudeMapPlot*> (plot()) -> mouseCanvasMoveEvent (event);
}

void AltitudeMapPlotCanvas::mouseReleaseEvent (QMouseEvent* event) {
    dynamic_cast<AltitudeMapPlot*> (plot()) -> mouseCanvasReleaseEvent (event);
}