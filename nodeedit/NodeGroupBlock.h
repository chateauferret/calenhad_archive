//
// Created by martin on 07/06/17.
//

#ifndef CALENHAD_QNODEGROUPBLOCK_H
#define CALENHAD_QNODEGROUPBLOCK_H


#include <qmodule/NodeGroup.h>
#include "NodeBlock.h"
#include "../controls/SizeGripItem.h"

namespace calenhad {
    namespace qmodule {
        class Module;
        class NodeGroup;
    }
    namespace nodeedit {

        enum NodeGroupHandle {
            TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight, NoHandle
        };


        class NodeGroupResizer : public calenhad::controls::SizeGripItem::Resizer {
        public:
            virtual void operator() (QGraphicsItem* item, const QRectF& rect);
        };

        class NodeGroupBlock : public NodeBlock {
        Q_OBJECT

        public:
            enum {
                Type = QGraphicsItem::UserType + 8
            };

            NodeGroupBlock (calenhad::qmodule::NodeGroup* node, QGraphicsItem* parent = 0);

            virtual ~NodeGroupBlock ();

            void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

            virtual QRectF boundingRect () const;


            void mouseReleaseEvent (QGraphicsSceneMouseEvent* event) override;


            void setHighlight (bool highlighted);

            void setRect (const QRectF& rect);

        public slots:

            void nodeChanged () override;

        protected:
            QPainterPath makePath ();
            calenhad::controls::SizeGripItem* _rectSizeGripItem;
            QRectF _rect;
            bool _highlighted;

        };
    }
}
#endif //CALENHAD_QNODEGROUPBLOCK_H
