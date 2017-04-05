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
#include <iostream>

QNEBlock::QNEBlock (ComponentProxyWidget* parent) : QGraphicsPathItem (parent) {
//-- : QGraphicsPathItem(parent) {
    setFlag (QGraphicsItem::ItemIsMovable);
    setFlag (QGraphicsItem::ItemIsSelectable);
}

QNEPort* QNEBlock::addPort (const QString& name, int portType, int index) {
    QNEPort* port = new QNEPort (portType, index, name);
    return addPort (port);
}

QNEPort* QNEBlock::addPort (QNEPort* port) {
    port -> setParentItem ((ComponentProxyWidget*) parentItem());
    ((ComponentProxyWidget*) parentItem()) -> addPort (port);
    port -> setBlock (this);
    port -> initialise();
    return port;
}

void QNEBlock::save (QDataStream& ds) {
    ds << pos ();

    int count (0);
    foreach(QGraphicsItem* port_, childItems ()) {
        if (port_ -> type () != QNEPort::Type) {
            continue;
        }
        count++;
    }
    ds << count;
    foreach(QGraphicsItem* port_, childItems ()) {
        if (port_ -> type () != QNEPort::Type) {
            continue;
        }

        QNEPort* port = (QNEPort*) port_;
        ds << (quint64) port;
        ds << port -> portName ();
        ds << port -> portType ();
    }
}

void QNEBlock::load (QDataStream& ds, QMap<quint64, QNEPort*>& portMap) {
    QPointF p;
    ds >> p;
    setPos (p);
    int count;
    ds >> count;
    for (int i = 0; i < count; i++) {
        QString name;
        bool portType;
        quint64 ptr;

        ds >> ptr;
        ds >> name;
        ds >> portType;
        portMap[ptr] = addPort (name, portType, ptr);
    }
}

QNEBlock* QNEBlock::clone () {
    QNEBlock* b = new QNEBlock (0);
    this -> scene () -> addItem (b);
            foreach(QGraphicsItem* port_, childItems ()) {
            if (port_ -> type () == QNEPort::Type) {
                QNEPort* port = (QNEPort*) port_;
                b -> addPort (port -> portName (), port -> portType(), port -> ptr ());
            }
        }

    return b;
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
            QGraphicsItem* p = parentItem();
            foreach (QGraphicsItem* port_, parentItem() -> childItems()) {
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
            foreach (QGraphicsItem* port_, parentItem() -> childItems()) {
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

void QNEBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // do not paint the item

}

QRectF QNEBlock::boundingRect() const {
    return parentItem() -> boundingRect ();
}
