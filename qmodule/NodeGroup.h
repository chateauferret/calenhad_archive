//
// Created by martin on 06/06/17.
//

#ifndef CALENHAD_QWIDGETGROUP_H
#define CALENHAD_QWIDGETGROUP_H


#include <nodeedit/NodeGroupBlock.h>
#include "Node.h"

namespace calenhad {
    namespace qmodule {

        class NodeGroup : public Node {
        Q_OBJECT
        public:
            NodeGroup (QWidget* parent = 0);
            virtual ~NodeGroup ();

            enum {
                Type = QGraphicsItem::UserType + 5
            };

            void setRect (const QRect& rect);

            QRect rect ();

            bool isWithin (const QPoint& point);

            QString nodeType () override;


            void initialise () override;

            calenhad::nodeedit::NodeBlock* makeHandle() override;

        signals:

            void changedOctaves (const int&);

        protected:
            QRect _rect;

            void addInputPorts () override;


        };
    }
}


#endif //CALENHAD_QWIDGETGROUP_H
