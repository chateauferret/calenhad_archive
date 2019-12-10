//
// Created by martin on 19/02/18.
//

#ifndef CALENHAD_QCOLORBUTTON_H
#define CALENHAD_QCOLORBUTTON_H

#include <QPushButton>
#include <QColor>

namespace calenhad {
    namespace controls {
        namespace legend {


            class QColorButton : public QPushButton {
            Q_OBJECT
            public:
                QColorButton (QWidget* parent);
                void setColor (const QColor& color);
                const QColor& color();

            public slots:
                void updateColor();
                void changeColor();

            private:
                QColor _color;
            };


        }
    }
}

#endif //CALENHAD_QCOLORBUTTON_H
