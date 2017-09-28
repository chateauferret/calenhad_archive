//
// Created by martin on 27/09/17.
//

#ifndef CALENHAD_LEGENDEDITORSLIDER_H
#define CALENHAD_LEGENDEDITORSLIDER_H

#include <QWidget>
#include <QColor>

namespace calenhad {
    namespace controls {
        namespace legend {

            class LegendEditorSlider : public QWidget {
            Q_OBJECT
            public:

                /// Constructor
                LegendEditorSlider (int orientation = Qt::Horizontal, QColor col = Qt::black, QWidget* parent = 0);

                /// set the color of the slider
                void setColor (QColor col);

                /// get the color
                QColor color ();
                qreal value() const;

                void setValue (const qreal& value);

            protected:

                // the value
                qreal _value;

                /// the color of the slider
                QColor _color;

                /// the orientation
                int _orientation;

            protected slots:

                /// paint the widget
                virtual void paintEvent (QPaintEvent* event);

            };

        }
    }
}

#endif //CALENHAD_LEGENDEDITORSLIDER_H
