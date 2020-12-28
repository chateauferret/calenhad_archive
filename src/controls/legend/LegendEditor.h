/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** QColorRampEditor is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/

#ifndef __QCOLORRAMPEDITOR_H__
#define __QCOLORRAMPEDITOR_H__

#include <QWidget>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>

#include <iostream>
#include <qwt/qwt_scale_widget.h>
#include "legend/LegendEntry.h"
#include "LegendEntryDialog.h"


namespace calenhad {
    namespace legend {
        class Legend;
    }

    namespace controls {
        namespace legend {

            class LegendEditorSlider;
            class LegendEditorSliderPanel;
            class LegendEditorScale;
            class LegendPreview;
            class QSlidersWidget;
            class QSliderTextWidget;
            class LegendEditor : public QWidget {
            Q_OBJECT

            public:

                /// Basic Constructor
                LegendEditor (calenhad::legend::Legend* legend, QWidget* parent = 0, int orientation = Qt::Horizontal);

                /// Destructor
                virtual ~LegendEditor ();

                // define friends to access protected members
                friend class LegendPreview;
                friend class LegendEditorSliderPanel;
                friend class LegendEditorScale;

                /// set a color choose dlg
                void setColorChoose (QColorDialog* coldlg);
                double valueAt (const int& i);
                /// set the update emitting when sliding
                void setSlideUpdate (bool val);

                /// return the Ramp definition
                calenhad::legend::Legend* legend ();

                /// set flag for visualize
                void setMappingTextVisualize (bool);

                /// set the text color
                void setMappingTextColor (QColor);

                /// set the text color
                void setMappingTextAccuracy (int);

                /// get the value of a slider
                qreal updateKey (LegendEditorSlider* sl);


                double zoom();

                double pan();

                double maximum() const;

                double minimum() const;

                double valueAt (const double& pos);

            signals:

                /// signal that hide is changed
                void legendChanged (const QVector<calenhad::legend::LegendEntry>);

            protected slots:

                /// resize required
                virtual void resizeEvent (QResizeEvent* e);

                /// detect a mouse is pressed
                virtual void mousePressEvent (QMouseEvent* e);

                /// on update the ramp because sliders are changed
                virtual void updateRamp ();

                void zoom (const int& steps);

                void pan (const int& steps);


            protected:

                calenhad::legend::Legend* _legend;

                double _zoom;
                double _pan;


                /// the orientation
                int _orientation;


                /// the widgets drawint the ramp, the sliders, the text
                LegendPreview* _rampWidget;
                LegendEditorSliderPanel* _sliderWidget;
                LegendEditorScale* _scale;

                /// the index of the active slider
                int activeSlider_;

                /// flag to visualize the mapping
                bool visText_;

                /// color of the text
                QColor textColor_;

                /// the text accuracy
                int textAcc_;

                /// continous update?
                bool _slideUpdate;

                void showEvent (QShowEvent* e);

                void setView (double from, double to);

                void wheelEvent (QWheelEvent* e);

                void mouseMoveEvent (QMouseEvent* e) override;

                double posForValue (const double& value);

                int orientation () const;

                int _bspace;
            };


        }
    }
}

#endif
