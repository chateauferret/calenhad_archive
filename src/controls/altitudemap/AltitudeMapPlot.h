//
// Created by martin on 31/03/17.
//

#ifndef CALENHAD_ALTITUDEMAPWIDGET_H
#define CALENHAD_ALTITUDEMAPWIDGET_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot_marker.h>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include "module/AltitudeMap.h"
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_magnifier.h>
#include "AltitudeMapping.h"
#include "AltitudeMappingDialog.h"

class QwtSymbol;

namespace calenhad {
    namespace controls {
        namespace altitudemap {


            class AltitudeMapFitter;
            class AltitudeMapPlotCanvas : public QwtPlotCanvas {
            public:
                AltitudeMapPlotCanvas();
                virtual ~AltitudeMapPlotCanvas();
                virtual void mouseDoubleClickEvent (QMouseEvent* event) override;

                virtual void mouseMoveEvent (QMouseEvent* event) override;

                virtual void mouseReleaseEvent (QMouseEvent* event) override;

            protected:
                QwtPlotMagnifier* _zoomer;
                QwtPlotPanner* _panner;

            };

            class AltitudeMapPlot : public QwtPlot {
            Q_OBJECT
            public:
                AltitudeMapPlot (int resolution = 100, QWidget* parent = 0);

                ~AltitudeMapPlot ();

                void setEntries (const QVector<AltitudeMapping> entries);

                QVector<AltitudeMapping> getEntries ();

                void mouseCanvasPressEvent (QMouseEvent* event);

                void mouseCanvasMoveEvent (QMouseEvent* event);

                void mouseCanvasReleaseEvent (QMouseEvent* event);

                QPointF toPlotCoordinates (const int& pixelX, const int& pixelY);

                const int noneSelected = -1;

                void setCurveType (CurveType type);

                CurveType curveType ();

            public slots:
                void updateFromDialog ();
                void plotPoints ();

            protected:
                void changeEvent (QEvent* e);

                QwtPlotCurve* _curve;
                QwtCurveFitter* _fitter;
                QwtSymbol* _symbol, * _selectedSymbol, * _fixedSymbol;
                QVector<AltitudeMapping> _entries;
                int _index;
                bool _drag;
                CurveType _curveType;
                int _resolution;

                bool isOnCanvas (const QPointF& point);

                void updatePoint (QPointF& point);

                QVector<AltitudeMapping> remapTerrace ();
                QVector<AltitudeMapping> remapTerrace (const QVector<AltitudeMapping>& entries);
                bool canDeleteSelected ();


                AltitudeMapPlotCanvas* _canvas;
                QwtPlotPanner* _panner;
                QwtPlotMagnifier* _zoomer;

                QVector<QPointF> samples ();

                void showAltitudeMapping (const QPoint& pos);



                AltitudeMappingDialog* _dialog;
                QwtPlotMarker* _selectedMarker;
                QVector<QwtPlotMarker*> _markers;
            };


        }
    }
}

#endif //CALENHAD_ALTITUDEMAPWIDGET_H
