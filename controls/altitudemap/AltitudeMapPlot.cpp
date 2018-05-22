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
#include "TerraceMapFitter.h"
#include <QMouseEvent>
#include <CalenhadServices.h>
#include "../../preferences/PreferencesService.h"


using namespace calenhad::controls::altitudemap;

AltitudeMapPlot::AltitudeMapPlot (int resolution, QWidget *parent) : QwtPlot (parent),
        _curve (new QwtPlotCurve()),
        _drag (false),
        _dialog (nullptr),
        _index (noneSelected),
        _symbol (new QwtSymbol),
        _selectedSymbol (new QwtSymbol),
        _selectedMarker (new QwtPlotMarker()),
        _fixedSymbol (new QwtSymbol()),
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

    _symbol -> setStyle (QwtSymbol::Rect);
    int symbolSize = CalenhadServices::preferences() -> calenhad_altitudemap_marker_normal_size;
    _symbol -> setSize (symbolSize, symbolSize);
    _symbol -> setBrush (CalenhadServices::preferences() -> calenhad_altitudemap_marker_normal_color);
    _symbol -> setPen (CalenhadServices::preferences() -> calenhad_altitudemap_marker_normal_color.darker());
    _curve -> setSymbol (_symbol);

    _fixedSymbol -> setStyle (QwtSymbol::Rect);
    _fixedSymbol -> setSize (symbolSize, symbolSize);
    _fixedSymbol -> setBrush (CalenhadServices::preferences() -> calenhad_altitudemap_marker_fixed_color);
    _fixedSymbol -> setPen (CalenhadServices::preferences() -> calenhad_altitudemap_marker_fixed_color.darker());

    _selectedSymbol -> setStyle (QwtSymbol::Rect);
    int markerSize = CalenhadServices::preferences() -> calenhad_altitudemap_marker_selected_size;
    _selectedSymbol -> setSize (markerSize, markerSize);
    _selectedSymbol -> setBrush (CalenhadServices::preferences() -> calenhad_altitudemap_marker_selected_color);
    _selectedSymbol -> setPen (CalenhadServices::preferences() -> calenhad_altitudemap_marker_selected_color.darker());
    _selectedMarker -> setSymbol (_selectedSymbol);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid -> attach (this);
    resize (600, 400);

}

AltitudeMapPlot::~AltitudeMapPlot() {
    if (_selectedMarker) {  _selectedMarker -> detach(); }
    for (QwtPlotMarker* marker : _markers) { marker -> detach(); }
    _curve -> detach(); // automatically deletes the curve
    delete _symbol;
    delete _selectedSymbol;
    delete _fixedSymbol;
    if (_fitter) { delete _fitter; }
    if (_zoomer) { delete _zoomer; }
    if (_panner) { delete _panner; }
    if (_dialog) { delete _dialog; }

}

void AltitudeMapPlot::changeEvent (QEvent *e) {

}

void AltitudeMapPlot::setEntries (const QVector<AltitudeMapping> entries) {
    _entries = entries;
    QPen pen (CalenhadServices::preferences() -> calenhad_altitudemap_curve_color);
    pen.setWidth (CalenhadServices::preferences() -> calenhad_altitudemap_curve_width);
    QVector<QPointF> points = samples();

    _curve -> setCurveAttribute (QwtPlotCurve::Fitted, true);
    _curve -> setPen (pen);
    _curve -> setStyle (QwtPlotCurve::Lines);
    _curve -> setRenderHint (QwtPlotItem::RenderAntialiased, true);
    _curve -> setSamples (points);
    _curve -> attach (this);
}

// for a terrace, the points always fall on the line x = y, but the owner computes values between the points such that the gradient
// from each point increases (or decreases) with the square of the distance from the point with the nearest lower x. This method remaps
// the y value to match the x in that case, but without destroying any y values already held, in case we want to revert to an altitude curve.
QVector<AltitudeMapping> AltitudeMapPlot::remapTerrace (const QVector<AltitudeMapping>& entries) {
    QVector<AltitudeMapping> remapped;
    for (AltitudeMapping point : entries) {
        remapped << AltitudeMapping (point.x(), point.x());
    }
    return remapped;
}

QVector<AltitudeMapping> AltitudeMapPlot::remapTerrace() {
    return remapTerrace (_entries);
}

QVector<AltitudeMapping> AltitudeMapPlot::getEntries() {
    if (_curveType == CurveType::Altitude) {
        return _entries;
    } else {
        return remapTerrace();
    }
}


QVector<QPointF> AltitudeMapPlot::samples () {
    QVector<AltitudeMapping> mappings = getEntries();
    QVector<QPointF> samples;
    for (AltitudeMapping mapping : mappings) {
        samples << mapping.point();
    }
    return samples;
}

void AltitudeMapPlot::mouseCanvasPressEvent (QMouseEvent* event) {
    double x = invTransform (xBottom, event -> pos().x ());
    double y = invTransform (yLeft, event -> pos().y ());
    double dist;
    _index = _curve -> closestPoint (event -> pos(), &dist);
    if (dist < 8) {
        canvas ()->setCursor (Qt::ClosedHandCursor);
        showAltitudeMapping (event -> pos());
    } else {
        _index = noneSelected;
        _entries.append (AltitudeMapping (x, y));
    }
    plotPoints();
}

void AltitudeMapPlot::showAltitudeMapping (const QPoint& p) {
    if (!_dialog) {
        _dialog = new AltitudeMappingDialog (this);
        connect (_dialog, &QDialog::accepted, this, &AltitudeMapPlot::updateFromDialog);
        connect (_dialog, &QDialog::rejected, this, &AltitudeMapPlot::plotPoints);
    }
    _dialog -> setIndex (_index);
    _dialog -> setMapping (_entries.at (_index));
    plotPoints();
    QPoint q (mapToGlobal (pos()).x() + p.x() + 32, mapToGlobal (pos()).y() + p.y() + 24);
    _dialog -> move (q);
    _dialog -> show ();
}

void AltitudeMapPlot::plotPoints() {
    std::sort (_entries.begin(), _entries.end(), [] (const AltitudeMapping& a, const AltitudeMapping& b) -> bool { return a.x() < b.x(); });
    _curve -> setSamples (samples ());

    replot();

   // clear any existing selection marker
    if (_selectedMarker) {
        _selectedMarker -> detach();
    }

    // define a separate marker for the selected entry so that it's highlighted
    if (_dialog && _dialog -> isVisible () && _dialog -> index() != noneSelected) {
        _selectedMarker -> attach (this);
        _selectedMarker -> setValue (_entries.at (_dialog -> index()).point());
    }

    // fixed markers for points that can't be draggged about
    for (QwtPlotMarker* marker : _markers) {
        marker -> detach();
    }
    for (AltitudeMapping mapping : _entries) {
        _markers.clear();
        if (mapping.isComputed()) {
            QwtPlotMarker* marker = new QwtPlotMarker;
            marker -> setSymbol (_fixedSymbol);
            marker -> attach (this);
            _markers.append (marker);
            marker -> setValue (mapping.point());
        }
    }

}

void AltitudeMapPlot::updateFromDialog() {
    AltitudeMapping mapping = _dialog -> mapping();
    if (mapping.isValid()) {
        _index = _dialog->index ();

        if (_index >= 0 && _index < _entries.size ()) {
            _entries.replace (_index, mapping);
            std::sort (_entries.begin (), _entries.end (), [] (const AltitudeMapping& a, const AltitudeMapping& b) -> bool { return a.x () < b.x (); });
            _curve->setSamples (samples ());
            plotPoints();
        }
    }
}

bool AltitudeMapPlot::isOnCanvas (const QPointF& point) {
    // return true if dropping a control point here would delete it - that is if it's outside the margins of the canvas and if there would still be enough points
    // left to make a curve.
    QwtInterval xInterval = axisInterval (xBottom);
    QwtInterval yInterval = axisInterval (yLeft);
    return point.x() > xInterval.minValue() && point.x() < xInterval.maxValue() && point.y() > yInterval.minValue() && point.y() < yInterval.maxValue();
}

void AltitudeMapPlot::mouseCanvasReleaseEvent (QMouseEvent* event) {
    // if the control point was dragged off the canvas, we delete it, provided the deletion preserves the curve's integrity as defined by canDeleteSelected()
    if (canDeleteSelected()) {
        QPointF point = toPlotCoordinates (event -> x(), event -> y());
        if (!isOnCanvas (point)) {
            _entries.remove (_index);
            _index = noneSelected;
            _curve -> setSamples (samples());
            plotPoints();
        }
    }

    // put the mouse cursor back how it was
    if (_index == noneSelected) {
        canvas() -> setCursor (Qt::CrossCursor);
    } else {
        if (_entries.at (_index).isComputed()) {
            canvas()->setCursor (Qt::PointingHandCursor);
        } else {
            canvas() -> setCursor (Qt::OpenHandCursor);
        }
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
                    canvas() -> setCursor (Qt::ForbiddenCursor);
                    QVector<QPointF> tempEntries;
                    for (int i = 0; i < _entries.size(); i++) {
                        if (i != _index) {
                            tempEntries << AltitudeMapping (_entries.at (i)).point();
                        }
                    }
                    std::sort (tempEntries.begin(), tempEntries.end(), [] (const QPointF& a, const QPointF& b) -> bool { return a.x() < b.x(); });
                    if (_curveType == CurveType::Altitude) {
                        _curve -> setSamples (tempEntries);
                    } else {
                        QVector<QPointF> points;
                        QVector<AltitudeMapping> mappings = remapTerrace();
                        for (AltitudeMapping mapping : mappings) {
                            points << mapping.point();
                        }
                        _curve -> setSamples (points);
                    }
                } else {
                    // moved off the canvas, but can't drop the control point because of the minimum number of control points required.
                    // So keep the control point and keep it on the canvas.
                    QwtInterval xInterval = axisInterval (xBottom);
                    QwtInterval yInterval = axisInterval (yLeft);
                    if (point.x() < xInterval.minValue() ) { point.setX (xInterval.minValue()); }
                    if (point.x() > xInterval.maxValue() ) { point.setX (xInterval.maxValue()); }
                    if (point.y() < yInterval.minValue() ) { point.setY (yInterval.minValue()); }
                    if (point.y() > yInterval.maxValue() ) { point.setY (yInterval.maxValue()); }
                    // if the  control point stays on the canvas we just move it about, reordering the points if one is moved past another in the x direction
                    updatePoint (point);
                }
                plotPoints();
            } else {
                canvas() -> setCursor (Qt::ClosedHandCursor);

                // if the  control point stays on the canvas we just move it about, reordering the points if one is moved past another in the x direction
                updatePoint (point);
                _index = _curve -> closestPoint (event -> pos(), &dist);
            }
        }
    } else {
        _index = _curve -> closestPoint (event -> pos(), &dist);
        if (dist < 8) {
            if (_entries.at (_index).isComputed()) {
                canvas() -> setCursor (Qt::PointingHandCursor);
            } else {
                canvas() -> setCursor (Qt::OpenHandCursor);
            }
            _panner -> setEnabled (false);
        } else {
            canvas() -> setCursor (Qt::CrossCursor);
            _index = noneSelected;
            _panner -> setEnabled (true);
        }
    }
}

// Updates the coordinates of the point identified by _index to move it to the coordinates of the supplied QPointF.
void AltitudeMapPlot::updatePoint (QPointF& point) {
    if (! _entries.at (_index).isComputed()) {

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
            std::sort (_entries.begin (), _entries.end (), [] (const AltitudeMapping& a, const AltitudeMapping& b) -> bool { return a.x () < b.x (); });
        }
        _curve->setSamples (samples ());
        plotPoints ();
    }
}

QPointF AltitudeMapPlot::toPlotCoordinates (const int& pixelX, const int& pixelY) {
    double x = invTransform (xBottom, pixelX);
    double y = invTransform (yLeft, pixelY);
    return QPointF (x, y);
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
    _curve -> setSamples (samples());
    plotPoints();
}

// Tell whether the point selected (the one identified by _index) may be deleted, returning false if to do so would leave the curve unusable.
// A point cannot be deleted if it is at either end of the curve or if too few points would remain for y to be computed for any value of x.
bool AltitudeMapPlot::canDeleteSelected () {
   return  _entries.size() > 4;
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