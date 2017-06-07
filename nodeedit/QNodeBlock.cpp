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

#include "QNodeBlock.h"
#include <QPainter>
#include "Calenhad.h"
#include "../qmodule/QModule.h"
#include "EditableLabel.h"
#include "../CalenhadServices.h"
#include "qneconnection.h"
#include "QNodeGroupBlock.h"
#include "../qmodule/QNodeGroup.h"


QNodeBlock::QNodeBlock (QNode* node, QGraphicsItem* parent) : QGraphicsPathItem (parent), _node (node), _label (nullptr) {

}

void QNodeBlock::initialise() {
    setFlag (QGraphicsItem::ItemIsMovable);
    setFlag (QGraphicsItem::ItemIsSelectable);
    setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
    setCursor (Qt::OpenHandCursor);

    // name label
    _label = new EditableLabel (this);
    _label -> setText (_node -> name());
    _label -> setDefaultTextColor (CalenhadServices::preferences() -> calenhad_module_text_color_normal);
    _label -> setPos (10 - (_label -> boundingRect().width() / 2 ), 34);

    connect (_label, &EditableLabel::textEdited, this, [=] () {
        if (_node -> name() != _label -> toPlainText()) {
            _node -> propertyChangeRequested ("name", _label -> toPlainText ());
        }
    });
    connect (_node, &QNode::nameChanged, this, [=] () { _label -> setText (_node -> name()); });
    setPath (makePath ());
}

QPainterPath QNodeBlock::makePath() {
    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (0, 0, 32, 32);
    setPen (QPen (CalenhadServices::preferences() -> calenhad_module_text_color_normal, CalenhadServices::preferences() -> calenhad_port_border_weight));
    setBrush (CalenhadServices::preferences() -> calenhad_module_brush_color_normal);
    p.addPolygon (polygon);
    return p;
}

void QNodeBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option)

    _brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_module_brush_color_selected : CalenhadServices::preferences() -> calenhad_module_brush_color_normal);
    painter -> setBrush (_brush);
    _pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_module_text_color_selected : CalenhadServices::preferences() -> calenhad_module_text_color_normal);
    painter -> setPen (_pen);


    painter -> drawPath (path());

}

QNEPort* QNodeBlock::addPort (QNEPort* port) {
    port -> setParentItem (this);

    int vertMargin = 2;
    QFontMetrics fm (scene() -> font());
    int yInput = port -> radius();
    int yOutput = yInput;
    int w = fm.width (port -> portName());
    int h = fm.height();
    foreach (QGraphicsItem* port_, childItems()) {
        if (port_->type () == QNEPort::Type) {
            QNEPort* port = (QNEPort*) port_;
            if (port -> portType () == QNEPort::OutputPort) {
                port -> setPos (32 + port -> radius (), yOutput + vertMargin);
                yOutput += h;
            } else {
                port -> setPos (-port -> radius (), yInput + vertMargin);
                yInput += h;
            }
        }
    }

    port -> setBlock (this);
    port -> initialise();
    connect (port, &QNEPort::connected, _node, &QNode::invalidate);
    connect (port, &QNEPort::disconnected, _node, &QNode::invalidate);
    return port;
}

QVector<QNEPort*> QNodeBlock::ports() {
    QVector<QNEPort*> res;
            foreach(QGraphicsItem* port_, childItems ()) {
                if (port_ -> type () == QNEPort::Type) {
                res.append ((QNEPort*) port_);
            }
        }
    return res;
}

QVariant QNodeBlock::itemChange (GraphicsItemChange change, const QVariant& value) {
    Q_UNUSED(change);
    return value;
}

QVector<QNEPort*> QNodeBlock::inputs() {
    QVector<QNEPort*> res;
            foreach (QGraphicsItem* port_, childItems()) {
            if (port_ -> type() == QNEPort::Type) {
                QNEPort* port = (QNEPort*) port_;
                if (port -> portType() != QNEPort::InputPort) {
                    res.append (port);
                }
            }
        }
    return res;
}

QVector<QNEPort*> QNodeBlock::outputs() {
    QVector<QNEPort*> res;
            foreach (QGraphicsItem* port_, childItems()) {
            if (port_ -> type () == QNEPort::Type) {
                QNEPort* port = (QNEPort*) port_;
                if (port -> portType() != QNEPort::OutputPort) {
                    res.append (port);
                }
            }
        }
    return res;
}

QVector<QNEPort*> QNodeBlock::controls() {
    QVector<QNEPort*> res;
            foreach (QGraphicsItem* port_, parentItem() -> childItems()) {
            if (port_ -> type () == QNEPort::Type) {
                QNEPort* port = (QNEPort*) port_;
                if (port -> portType() != QNEPort::ControlPort) {
                    res.append (port);
                }
            }
        }
    return res;
}

void QNodeBlock::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {
    setZValue (10000);
    QGraphicsItem::mouseMoveEvent (event);
    if (event->buttons() | Qt::LeftButton) {
        setCursor (Qt::ClosedHandCursor);
    } else {
        setCursor (Qt::OpenHandCursor);
    }
    event -> accept();
}

void QNodeBlock::mousePressEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent (event);
    _oldZ = zValue();
}

void QNodeBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    std::cout << "Mouse release \n";
    setCursor (Qt::OpenHandCursor);
    QNodeGroupBlock* target = nullptr;
    // find out if the node was dropped on a group and assign to that group if so
    QList<QGraphicsItem*> items = collidingItems (Qt::ContainsItemShape);
    for (QGraphicsItem* item : collidingItems()) {
        QNodeGroupBlock* block = dynamic_cast<QNodeGroupBlock*> (item);
        if (block && item != this) {
            if (!target) {
                target = (QNodeGroupBlock*) item;
            } else {
                // where groups are nested, find out which of the drop targets is the lower level and use that
                target = target->collidesWithItem (item, Qt::ContainsItemShape) ? (QNodeGroupBlock*) item : target;
                // if a group is nested within another group, see that they are stacked with subgroups on top of enclosing groups
                if (zValue () < target->zValue ()) {
                    setZValue (target->zValue () + 1);
                }
            }
        }
    }

    // assign this node to the target group, if there is one
    if (target) {
        _node -> setGroup ((QNodeGroup*) target -> node());
        std::cout << "Assigned " << node() -> name ().toStdString () << " to group " << target -> node() -> name().toStdString () << "\n";
    } else {
        setZValue (_oldZ);
    }

    QGraphicsItem::mouseReleaseEvent (event);
}

QRectF QNodeBlock::boundingRect() const {
    QRectF r = QGraphicsPathItem::boundingRect();
        foreach (QGraphicsItem* port_, childItems ()) {
            if (port_ -> type() == QNEPort::Type) {
                r = r.united (port_ -> boundingRect());
            }
        }
    r = r.united (_label -> boundingRect());
    return r;
}

QNode* QNodeBlock::node () {
    return _node;
}

void QNodeBlock::mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event) {
    _node -> showParameters (true);
}

void QNodeBlock::nodeChanged () {
    _label -> setPlainText (_node -> name());
    _label -> setPos (32 - (_label -> boundingRect().width() / 2 ), 38);
    _node -> invalidate();
}
