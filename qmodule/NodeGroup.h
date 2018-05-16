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

            bool isWithin (const QPoint& point);

            QString nodeType () override;


            void initialise () override;

            calenhad::nodeedit::NodeBlock* makeHandle() override;

            void inflate (const QDomElement& element) override;

            void serialize (QDomElement& element) override;

        signals:

            void changedOctaves (const int&);

        };
    }
}


#endif //CALENHAD_QWIDGETGROUP_H
