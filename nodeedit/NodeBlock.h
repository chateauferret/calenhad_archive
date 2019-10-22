/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef QNEBLOCK_H
#define QNEBLOCK_H

#include <QGraphicsPathItem>
#include <QRectF>
#include <QVector>
#include <QtWidgets/QLineEdit>

namespace calenhad {
	namespace module {
        class Node;
	}
	namespace controls {
		class QColoredIcon;
	}
	namespace nodeedit {
		class Port;
		class NodeNameValidator;
		class EditableLabel;

		class NodeGroupBlock;

		class NodeBlock : public QObject, public QGraphicsPathItem {
		Q_OBJECT
		public:
			enum {
				Type = QGraphicsItem::UserType + 3
			};

			NodeBlock (calenhad::module::Node* node, QGraphicsItem* parent = 0);

			virtual ~NodeBlock();

			Port* addPort (Port* port);

			void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

			int type() const { return Type; }

			QVector<Port*> inputs();

			Port* output();

			QVector<Port*> controls();

			virtual QRectF boundingRect() const;

			virtual void initialise();

			calenhad::module::Node* node();

			void assignIcon();

            void setText (const QString& text);
			calenhad::controls::QColoredIcon* icon ();

		public slots:

			virtual void nodeChanged();

			void mousePressEvent (QGraphicsSceneMouseEvent* event) override;

			void mouseReleaseEvent (QGraphicsSceneMouseEvent* event) override;

			void mouseMoveEvent (QGraphicsSceneMouseEvent* event) override;

			virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event) override;


		protected:
			QVariant itemChange (GraphicsItemChange change, const QVariant& value) override;

			calenhad::module::Node* _node;
			EditableLabel* _label;
			QBrush _brush;
			QPen _pen;
			QPixmap* _pixmap;

			virtual QPainterPath makePath();


			QSizeF _size;
			int _margin;


			calenhad::controls::QColoredIcon* _icon;
			QPixmap _iconImage;
            const QPixmap _endorsementOrright, _endorsementGoosed;
            QString _expression;

            calenhad::nodeedit::NodeNameValidator* _nameValidator;

			EditableLabel* _textLabel;
			QString _oldName;
			qreal _oldZ;
            int _inputs;
			unsigned int _spacing;
		};
	}
}

#endif // QNEBLOCK_H
