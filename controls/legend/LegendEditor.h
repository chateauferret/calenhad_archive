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

                /// get the number of sliders
                int getSliderCount ();

                /// set a color choose dlg
                void setColorChoose (QColorDialog* coldlg);

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

                /// get the position
                int updatePos (LegendEditorSlider* sl);

                double zoom();

                double pan();

                double maximum();

                double minimum();

                double valueAt (const double& pos);
                void deleteSlider (const int& index);

            signals:

                /// signal that hide is changed
                void legendChanged (const QVector<calenhad::legend::LegendEntry>);

            public slots:

                /// set the index and color of a slider
                void setSlider (const int& index, const QString& key, const QColor& col);

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

                /// sort the slider list
                static bool SliderSort (const LegendEditorSlider* a1, const LegendEditorSlider* a2);

                /// all poses with its sliders
                QList<LegendEditorSlider*> _sliders;

                /// the orientation
                int _orientation;

                /// bound space
                int bspace_;

                /// the widgets drawint the ramp, the sliders, the text
                LegendPreview* rampwid_;
                LegendEditorSliderPanel* slidewid_;
                LegendEditorScale* _scale;

                /// the index of the active slider
                int activeSlider_;

                /// a color chooser dlg
                calenhad::controls::legend::LegendEntryDialog* _dialog;

                /// flag to visualize the mapping
                bool visText_;

                /// color of the text
                QColor textColor_;

                /// the text accuracy
                int textAcc_;

                /// continous update?
                bool slideUpdate_;

                void showEvent (QShowEvent* e);

                void setView (double from, double to);

                void wheelEvent (QWheelEvent* e);

                void mouseMoveEvent (QMouseEvent* e) override;

                double posForValue (const double& value);

            };


        }
    }
}

#endif
