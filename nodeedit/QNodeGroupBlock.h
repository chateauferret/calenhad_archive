//
// Created by martin on 07/06/17.
//

#ifndef CALENHAD_QNODEGROUPBLOCK_H
#define CALENHAD_QNODEGROUPBLOCK_H


#include "QNodeBlock.h"
#include "../controls/SizeGripItem.h"

namespace calenhad {
    namespace qmodule {
        class QModule;
    }
    namespace nodeedit {

        enum NodeGroupHandle {
            TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight, NoHandle
        };

        class QNodeGroupBlock : public QNodeBlock {
        Q_OBJECT

        public:
            enum {
                Type = QGraphicsItem::UserType + 8
            };

            QNodeGroupBlock (calenhad::qmodule::QNode* node, QGraphicsItem* parent = 0);

            virtual ~QNodeGroupBlock ();

            void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

            virtual QRectF boundingRect () const;


            void mouseReleaseEvent (QGraphicsSceneMouseEvent* event) override;


            void setHighlight (bool highlighted);

            void setRect (const QRectF& rect);

        public slots:

            void nodeChanged () override;

        protected:
            QPainterPath makePath ();

            QRectF _rect;
            bool _highlighted;

        };


        class NodeGroupResizer : public calenhad::controls::SizeGripItem::Resizer {
        public:
            virtual void operator() (QGraphicsItem* item, const QRectF& rect) {
                QNodeGroupBlock* group = dynamic_cast<QNodeGroupBlock*> (item);
                if (group) {
                    group->setRect (rect);
                }
            }
        };

    }
}
#endif //CALENHAD_QNODEGROUPBLOCK_H
