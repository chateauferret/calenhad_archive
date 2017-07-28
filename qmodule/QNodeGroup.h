//
// Created by martin on 06/06/17.
//

#ifndef CALENHAD_QWIDGETGROUP_H
#define CALENHAD_QWIDGETGROUP_H


#include "QNode.h"

namespace calenhad {
    namespace qmodule {

        class QNodeGroup : public QNode {
        Q_OBJECT
        public:
            QNodeGroup (QWidget* parent = 0);

            virtual ~QNodeGroup ();

            enum {
                Type = QGraphicsItem::UserType + 5
            };

            void setRect (const QRect& rect);

            QRect rect ();

            bool isWithin (const QPoint& point);

            QString nodeType () override;

            QNodeGroup* clone () override;

            void initialise () override;

        signals:

            void changedOctaves (const int&);

        protected:
            QRect _rect;

            void addInputPorts () override;


            QSpinBox* _octaveSpin;

            int getOctaveCount ();

            void setOctaveCount (const int& value);

            int _octaves;
        };
    }
}


#endif //CALENHAD_QWIDGETGROUP_H
