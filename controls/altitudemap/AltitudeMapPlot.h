//
// Created by martin on 31/03/17.
//

#ifndef CALENHAD_ALTITUDEMAPWIDGET_H
#define CALENHAD_ALTITUDEMAPWIDGET_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_canvas.h>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include "../qmodule/QAltitudeMap.h"

class QwtSymbol;

namespace calenhad {
    namespace controls {
        namespace altitudemap {


            class AltitudeMapFitter;

            class AltitudeMapPlot : public QwtPlot {
            Q_OBJECT
            public:
                AltitudeMapPlot (int resolution = 100, QWidget* parent = 0);

                ~AltitudeMapPlot ();

                void setEntries (const QVector<QPointF>& entries);

                QVector<QPointF> getEntries ();

                void mouseCanvasPressEvent (QMouseEvent* event);

                void mouseCanvasMoveEvent (QMouseEvent* event);

                void mouseCanvasReleaseEvent (QMouseEvent* event);

                QPointF toPlotCoordinates (const int& pixelX, const int& pixelY);

                const int noneSelected = -1;

                void setCurveType (CurveType type);

                CurveType curveType ();

            protected:
                void changeEvent (QEvent* e);

                QwtPlotCurve* _curve;
                QwtCurveFitter* _fitter;
                QwtSymbol* _symbol;
                QVector<QPointF> _entries;
                int _index;
                bool _drag;
                CurveType _curveType;
                int _resolution;

                bool isOnCanvas (QPointF point);

                void updatePoint (QPointF point);

                QVector<QPointF> remapTerrace ();

                bool canDeleteSelected ();

                void sortEntries (QVector<QPointF>& entries);
            };

            class AltitudeMapPlotCanvas : public QwtPlotCanvas {
            public:
                virtual void mousePressEvent (QMouseEvent* event) override;

                virtual void mouseMoveEvent (QMouseEvent* event) override;

                virtual void mouseReleaseEvent (QMouseEvent* event) override;
            };
        }
    }
}

#endif //CALENHAD_ALTITUDEMAPWIDGET_H
