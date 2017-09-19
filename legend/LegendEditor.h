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
#include "Legend.h"


namespace calenhad {
    namespace legend {
        class Legend;
        class QColorRampEditorSlider;
        class QRampWidget;
        class QSlidersWidget;
        class QSliderTextWidget;

        class LegendEditor : public QWidget {
        Q_OBJECT


        public:

            /// Basic Constructor
            LegendEditor (Legend* legend, QWidget* parent = 0, int orientation = Qt::Horizontal);

            /// Destructor
            virtual ~LegendEditor ();

            // define friends to access protected members
            friend class QRampWidget;

            friend class QSlidersWidget;

            friend class QSliderTextWidget;

            /// get the number of sliders
            int getSliderCount ();

            /// set a color choose dlg
            void setColorChoose (QColorDialog* coldlg);

            /// set the update emitting when sliding
            void setSlideUpdate (bool val);

            /// return the Ramp definition
            Legend* legend ();

            /// set flag for visualize
            void setMappingTextVisualize (bool);

            /// set the text color
            void setMappingTextColor (QColor);

            /// set the text color
            void setMappingTextAccuracy (int);

            /// get the value of a slider
            qreal updateValue (QColorRampEditorSlider* sl);

            /// get the position
            int updatePos (QColorRampEditorSlider* sl);

            // get a pixmap summary
            QIcon icon ();

        signals:

            /// signal that hide is changed
            void legendChanged (const QList<LegendEntry>&);

        public slots:

            /// set the color of a slider to zero
            void setSliderColor (int index, QColor col);

        protected slots:

            /// resize required
            virtual void resizeEvent (QResizeEvent* e);

            /// detect a mouse is pressed
            virtual void mousePressEvent (QMouseEvent* e);

            /// on update the ramp because sliders are changed
            virtual void updateRamp ();

        protected:

            Legend* _legend;

            /// sort the slider list
            static bool SliderSort (const QColorRampEditorSlider* a1, const QColorRampEditorSlider* a2);

            /// all poses with its sliders
            QList<QColorRampEditorSlider*> _sliders;

            /// the orientation
            int _orientation;

            /// bound space
            int bspace_;

            /// min and max value from initialization
            const qreal min = -1.0, max = 1.0;

            /// the widgets drawint the ramp, the sliders, the text
            QRampWidget* rampwid_;
            QSlidersWidget* slidewid_;
            QSliderTextWidget* textwid_;

            /// the index of the active slider
            int activeSlider_;

            /// a color chooser dlg
            QColorDialog* _colorChooser;

            /// flag to visualize the mapping
            bool visText_;

            /// color of the text
            QColor textColor_;

            /// the text accuracy
            int textAcc_;

            /// continous update?
            bool slideUpdate_;

            void showEvent (QShowEvent* e);
        };

// -----------------------------------------------------------
// QColorRampEditorSlider ------------------------------------
// -----------------------------------------------------------
        class QColorRampEditorSlider : public QWidget {
        Q_OBJECT
        public:

            /// Constructor
            QColorRampEditorSlider (int orientation = Qt::Horizontal, QColor col = Qt::black, QWidget* parent = 0);

            /// set the color of the slider
            void setColor (QColor col);

            /// get the color
            QColor getColor ();

            /// the value
            qreal val;

        protected slots:

            /// paint the widget
            virtual void paintEvent (QPaintEvent* event);

        protected:

            /// the color of the slider
            QColor color_;

            /// the orientation
            int ori_;
        };

        class QRampWidget : public QWidget {
        public:
            QRampWidget (QWidget* parent = NULL);

            LegendEditor* _editor;

        protected:
            void paintEvent (QPaintEvent* e);
        };

        class QSlidersWidget : public QWidget {
        Q_OBJECT
        public:
            /// Constructor
            QSlidersWidget (QWidget* parent = NULL);

            LegendEditor* _editor;

        protected slots:

            /// detect a mouse is pressed
            virtual void mousePressEvent (QMouseEvent* e);

            /// detect a mouse is moved
            virtual void mouseMoveEvent (QMouseEvent* e);

            /// detect a mouse is released
            virtual void mouseReleaseEvent (QMouseEvent* e);

            /// detect a mouse is released
            virtual void mouseDoubleClickEvent (QMouseEvent* e);

        protected:
            /// the active slider
            int activeSlider_;
        };

        class QSliderTextWidget : public QWidget {
        public:
            QSliderTextWidget (QWidget* parent = NULL);

            LegendEditor* rampeditor_;

        protected:

            void paintEvent (QPaintEvent* e);
        };
    }
}

#endif
