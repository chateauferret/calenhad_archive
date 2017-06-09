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


    connect (_label, &EditableLabel::textEdited, this, [=] () {
        if (_node -> name() != _label -> toPlainText()) {
            _node -> propertyChangeRequested ("name", _label -> toPlainText ());
        }
    });
    connect (_node, &QNode::nameChanged, this, [=] () { _label -> setText (_node -> name()); });
    setPath (makePath ());
}

QPainterPath QNodeBlock::makePath() {
    _label -> setPos (10 - (_label -> boundingRect().width() / 2 ), 34);
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

    QGraphicsItem::mouseMoveEvent (event);
    if (event->buttons() | Qt::LeftButton) {
        setCursor (Qt::ClosedHandCursor);

        // highlight a group if we are dragging over it
        QList<QGraphicsItem*> items = collidingItems (Qt::ContainsItemShape);
        QList<QGraphicsItem*>::iterator i = items.begin ();
        while ( i != items.end() && ! (dynamic_cast<QNodeGroupBlock*> (*i))) {
            i++;
        }
        QNodeGroupBlock* target = i == items.end() ? nullptr : (QNodeGroupBlock*) *i;
        for (QGraphicsItem* item : scene ()->items ()) {
            if (dynamic_cast<QNodeGroupBlock*> (item)) {
                ((QNodeGroupBlock*) item) -> setHighlight (item == target);
            }
        }

        // keep item on top of any group it's being dragged over
        items = collidingItems (Qt::IntersectsItemShape);
        i = items.begin ();
        while ( i != items.end() && ! (dynamic_cast<QNodeGroupBlock*> (*i))) {
            i++;
        }
        target = i == items.end() ? nullptr : (QNodeGroupBlock*) *i;
        if (! target) {
            if (! parent()) { setZValue (-1000); }
        } else {
            for (QGraphicsItem* item : scene ()->items ()) {
                if (dynamic_cast<QNodeGroupBlock*> (item)) {
                    if (! parent ()) { setZValue (target->zValue() + 1); }
                }
            }
        }
        scene ()->update ();
    } else {
        setCursor (Qt::OpenHandCursor);
    }
    //event -> accept();
}

void QNodeBlock::mousePressEvent (QGraphicsSceneMouseEvent *event) {
    detach();
    setCursor (Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent (event);
    _oldZ = zValue();
    std::cout << node() -> name().toStdString () << " z = " << zValue () << "\n";
}

void QNodeBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    std::cout << "Mouse release \n";
    setCursor (Qt::OpenHandCursor);
    assignGroup();
    for (QGraphicsItem* item : scene() -> items()) {
        if (dynamic_cast<QNodeGroupBlock*> (item)) {
            ((QNodeGroupBlock*) item) -> setHighlight (false);
        }
    }

    QGraphicsItem::mouseReleaseEvent (event);
}

void QNodeBlock::assignGroup() {
    detach();
    QList<QGraphicsItem*> items = collidingItems (Qt::ContainsItemShape);
    QList<QGraphicsItem*>::iterator i = items.begin ();
    while ( i != items.end() && ! (dynamic_cast<QNodeGroupBlock*> (*i))) {
        i++;
    }
    QNodeGroupBlock* target = i == items.end() ? nullptr : (QNodeGroupBlock*) *i;
    // assign this node to the target group, if there is one (and if not already assigned)
    setZValue (0);
    if (target) {
        _node -> setGroup ((QNodeGroup*) target -> node ());
        attach (target);
    } else {
        _node -> setGroup (nullptr);
    }
    std::cout << "Assigned group " << (_node -> group() == nullptr ? "Null " : _node -> group() -> name().toStdString ()) << "\n";
}

void QNodeBlock::detach() {
    QPointF p = scenePos();
    setParentItem (nullptr);
    setPos (p);
    setSelected (false);
}

void QNodeBlock::attach (QGraphicsItem* target) {
    if (target -> parentItem() != this && target != parentItem()) {
        QPointF p = mapToItem (target, pos ());
        setParentItem (target);
        setPos (mapFromParent (p));
    }
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
