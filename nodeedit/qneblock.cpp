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
#include "../CalenhadServices.h"
#include "../qmodule/QModule.h"


QNEBlock::QNEBlock (QModule* module, QGraphicsItem* parent) : QGraphicsPathItem (parent), _module (module), _nameProxy (new QGraphicsProxyWidget()), _nameEdit (nullptr) {
//-- : QGraphicsPathItem(parent) {
    setFlag (QGraphicsItem::ItemIsMovable);
    setFlag (QGraphicsItem::ItemIsSelectable);

        QPainterPath p;
        QPolygonF polygon;


        polygon = QRectF (0, 0, 32, 32);
        setPen (QPen (CalenhadServices::preferences() -> calenhad_handle_text_color_normal, CalenhadServices::preferences() -> calenhad_port_border_weight));
        setBrush (CalenhadServices::preferences() -> calenhad_handle_brush_color_normal);
        p.addPolygon (polygon);
        setPath (p);
        setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
}


QNEPort* QNEBlock::addPort (QNEPort* port) {
    port -> setParentItem (this);

    int horzMargin = 20;
    int vertMargin = 25;
    QFontMetrics fm (scene() -> font());
    int yInput = port -> radius();
    int yOutput = yInput;
    int w = fm.width (port -> portName());
    int h = fm.height();

            foreach (QGraphicsItem* port_, childItems()) {
            if (port_ -> type() == QNEPort::Type) {
                QNEPort* port = (QNEPort*) port_;
                if (port -> portType() == QNEPort::OutputPort) {
                    port -> setPos (port -> radius (), yOutput + vertMargin);
                    yOutput += h;
                } else {
                    port -> setPos ( - port -> radius (), yInput + vertMargin);
                    yInput += h;
                }
            }
        }

    port -> setBlock (this);
    port -> initialise();
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


void QNEBlock::editModuleName() {
    if (! _nameEdit) {
        createNameEditor ();
        scene() -> addItem (_nameProxy);
        _nameEdit -> setFixedHeight (16);
    }
    _nameProxy -> setPos (pos ().x () + 2, pos ().y () + 2);
    QString name = _module -> name();
    _nameEdit -> setText (name);
    _nameEdit -> selectAll ();
    _nameEdit -> setFocus();
    _nameEdit -> show();
}


void QNEBlock::createNameEditor() {
    _nameEdit = new QLineEdit();
    _nameProxy -> setWidget (_nameEdit);
    connect (_nameEdit, &QLineEdit::editingFinished, this, &QNEBlock::setName);
}

void QNEBlock::setName() {
    _nameEdit -> hide();
    _module -> setName (_nameEdit -> text());
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
    if (_nameProxy) {
        _nameProxy->setPos (pos ().x () + 2, pos ().y () + 2);
    }
}

void QNEBlock::mousePressEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent (event);

}

void QNEBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent (event);
}

void QNEBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QGraphicsPathItem::paint (painter, option);
    QPen pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_handle_text_color_selected : CalenhadServices::preferences() -> calenhad_handle_text_color_normal);
    QBrush brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_handle_brush_color_selected : CalenhadServices::preferences() -> calenhad_handle_brush_color_normal);
    painter -> setBrush (brush);
    QRect rect (38, 0, 38, 0);
    rect.setWidth (rect.width() - 4);
    rect.moveTo (5, 3);
    painter -> drawText (rect, Qt::AlignLeft, _module -> name());
}

QRectF QNEBlock::boundingRect() const {
    QRectF r = QGraphicsPathItem::boundingRect();
        foreach (QGraphicsItem* port_, childItems ()) {
            if (port_ -> type() == QNEPort::Type) {
                r = r.united (port_ -> boundingRect());
            }
        }
    return r;
}



QModule* QNEBlock::module () {
    return _module;
}

void QNEBlock::mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event) {
    editModuleName();
}