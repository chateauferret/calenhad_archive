//
// Created by martin on 19/10/17.
//

#ifndef CALENHAD_COLORBUTTON_H
#define CALENHAD_COLORBUTTON_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QColorDialog>

namespace calenhad {
    namespace controls {
        class ColorButton : public QPushButton {
        Q_OBJECT
        public:
            ColorButton (QWidget* parent);
            virtual ~ColorButton();
            QColor color();
            void setColor (const QColor& color);

        signals:
            void colorSelected (const QColor& color);

        protected:
            QColor _color;
            QColorDialog* _dialog;

            void showEvent (QShowEvent* event) override;
            void paintEvent (QPaintEvent* event) override;
        };
    }
}



#endif //CALENHAD_COLORBUTTON_H
