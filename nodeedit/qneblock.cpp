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

#include "qneblock.h"
#include <QPainter>
#include "Calenhad.h"
#include "../qmodule/QModule.h"
#include "EditableLabel.h"
#include "../CalenhadServices.h"
#include "qneconnection.h"


QNEBlock::QNEBlock (QNode* node, QGraphicsItem* parent) : QGraphicsPathItem (parent), _node (node), _label (nullptr) {
    setFlag (QGraphicsItem::ItemIsMovable);
    setFlag (QGraphicsItem::ItemIsSelectable);

    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (0, 0, 32, 32);
    setPen (QPen (CalenhadServices::preferences() -> calenhad_handle_text_color_normal, CalenhadServices::preferences() -> calenhad_port_border_weight));
    setBrush (CalenhadServices::preferences() -> calenhad_handle_brush_color_normal);
    p.addPolygon (polygon);
    setPath (p);
    setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
    setCursor (Qt::OpenHandCursor);

    // name label
    _label = new EditableLabel (this);
    _label -> setText (_node -> name());
    _label -> setDefaultTextColor (CalenhadServices::preferences() -> calenhad_handle_text_color_normal);
    _label -> setPos (16 - (_label -> boundingRect().width() / 2 ), 38);

    connect (_label, &EditableLabel::textEdited, this, [=] () {
        if (_node -> name() != _label -> toPlainText()) {
            _node -> propertyChangeRequested ("name", _label -> toPlainText ());
        }
    });
    connect (_node, &QNode::nameChanged, this, [=] () { _label -> setText (_node -> name()); });

}

void QNEBlock::initialise() {

}

void QNEBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option)

    _brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_handle_brush_color_selected : CalenhadServices::preferences() -> calenhad_handle_brush_color_normal);
    painter -> setBrush (_brush);
    _pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_handle_text_color_selected : CalenhadServices::preferences() -> calenhad_handle_text_color_normal);
    painter -> setPen (_pen);


    painter -> drawPath (path());

}

QNEPort* QNEBlock::addPort (QNEPort* port) {
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

QVector<QNEPort*> QNEBlock::ports() {
    QVector<QNEPort*> res;
            foreach(QGraphicsItem* port_, childItems ()) {
                if (port_ -> type () == QNEPort::Type) {
                res.append ((QNEPort*) port_);
            }
        }
    return res;
}

QVariant QNEBlock::itemChange (GraphicsItemChange change, const QVariant& value) {
    Q_UNUSED(change);
    return value;
}

QVector<QNEPort*> QNEBlock::inputs() {
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

QVector<QNEPort*> QNEBlock::outputs() {
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

QVector<QNEPort*> QNEBlock::controls() {
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

void QNEBlock::mouseMoveEvent (QGraphicsSceneMouseEvent * event) {

    QGraphicsItem::mouseMoveEvent (event);
    if (event->buttons() | Qt::LeftButton) {
        setCursor (Qt::ClosedHandCursor);
    } else {
        setCursor (Qt::OpenHandCursor);
    }
    event -> accept();
}

void QNEBlock::mousePressEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent (event);

}

void QNEBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent (event);
}

QRectF QNEBlock::boundingRect() const {
    QRectF r = QGraphicsPathItem::boundingRect();
        foreach (QGraphicsItem* port_, childItems ()) {
            if (port_ -> type() == QNEPort::Type) {
                r = r.united (port_ -> boundingRect());
            }
        }
    r = r.united (_label -> boundingRect());
    return r;
}

QNode* QNEBlock::node () {
    return _node;
}

void QNEBlock::mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event) {
    _node -> showParameters (true);
}

void QNEBlock::moduleChanged () {
    _label -> setPlainText (_node -> name());
    _label -> setPos (32 - (_label -> boundingRect().width() / 2 ), 38);
    _node -> invalidate();
}
