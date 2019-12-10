//
// Created by martin on 17/06/17.
//

#ifndef CALENHAD_QDRAGGABLEICON_H
#define CALENHAD_QDRAGGABLEICON_H


#include <QtWidgets/QLabel>
#include <QGraphicsColorizeEffect>

namespace calenhad {
    namespace controls {

        class QColoredIcon : public QLabel {

        public:
            QColoredIcon (QWidget* parent = 0);

            virtual ~QColoredIcon ();

            void setColor (const QColor& color);

            void setMouseOverColor (const QColor& color);

        protected:
            QColor _color, _mouseOverColor;
            QGraphicsColorizeEffect* _colorizer;

            void enterEvent (QEvent* event) override;

            void leaveEvent (QEvent* event) override;


        };
    }
}


#endif //CALENHAD_QDRAGGABLEICON_H
