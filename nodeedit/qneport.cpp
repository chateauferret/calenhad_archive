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

#include "qneport.h"
#include "qneblock.h"
#include <QGraphicsScene>
#include "EditableLabel.h"
#include "qneconnection.h"
#include "../CalenhadServices.h"
#include "../qmodule/QModule.h"
#include "../actions/ChangeModuleCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/CalenhadController.h"

QNEPort::QNEPort (int type, int index, const QString& name, QNEBlock* parent) :
        QGraphicsPathItem (parent),
        _radius (CalenhadServices::preferences() -> calenhad_port_radius),
        _margin (CalenhadServices::preferences() -> calenhad_port_margin),
        _index (index),
        _portType (type),
        _portName (name) {
    QPainterPath p;
    QPolygonF polygon;
    _label = new EditableLabel (this);
    _label->setDefaultTextColor (CalenhadServices::preferences ()->calenhad_port_text_color);
    connect (_label, SIGNAL (textEdited (const QString&)), this, SLOT (nameChangeRequested (const QString&)));

    if (type == OutputPort) {
        polygon << QPointF (-_radius, -_radius) << QPointF (_radius, 0) << QPointF (-_radius, _radius) << QPointF (-_radius, -_radius);
        setPen (QPen (CalenhadServices::preferences() -> calenhad_port_out_border_color, CalenhadServices::preferences() -> calenhad_port_border_weight));
        setBrush (CalenhadServices::preferences() -> calenhad_port_out_fill_color);
        setCursor (Qt::ArrowCursor);
    }
    if (type == InputPort) {
        polygon << QPointF (-_radius, -_radius) << QPointF (_radius, 0) << QPointF (-_radius, _radius) << QPointF (-_radius, -_radius);
        setHighlight (PortHighlight::NONE);
    }
    if (type == ControlPort) {
        polygon = QRectF (-_radius, -_radius, _radius * 2, _radius * 2);
        setHighlight (PortHighlight::NONE);
    }
    p.addPolygon (polygon);
    setPath (p);
    setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
}

QNEPort::~QNEPort () {
    foreach (QNEConnection* conn, m_connections) delete conn;
}

void QNEPort::setBlock (QNEBlock* b) {
    _block = b;
}

void QNEPort::nameChangeRequested (const QString& value) {
    if (portName() != value) {
        ChangeModuleCommand* c = new ChangeModuleCommand (owner (), "name", portName (), value, _index, _portType);
        CalenhadModel* model = owner () -> model();
        if (model) {
            model -> controller() -> doCommand (c);
        }
    }
}

void QNEPort::setName (const QString& n) {
    _portName = n;

    // if we are setting the name from outside, we need to update the display too
    if (_label -> toPlainText() != n) {
        _label -> setText (n);
    }
}

int QNEPort::radius () {
    return _radius;
}

QVector<QNEConnection*>& QNEPort::connections () {
    return m_connections;
}

QNEBlock* QNEPort::block () const {
    return _block;
}

bool QNEPort::isConnected (QNEPort* other) {
            foreach (QNEConnection* conn, m_connections) {
            if (conn -> port1 () == other || conn -> port2 () == other) {
                return true;
            }
        }
    return false;
}

QVariant QNEPort::itemChange (GraphicsItemChange change, const QVariant& value) {
    if (change == ItemScenePositionHasChanged) {
                foreach (QNEConnection* conn, m_connections) {
                conn -> updatePosFromPorts ();
                conn -> updatePath ();
            }
    }
    return value;
}

void QNEPort::initialise () {
        _label -> setPlainText (_portName);
        _label -> setDefaultTextColor (CalenhadServices::preferences() -> calenhad_port_text_color);
        if (_portType == OutputPort) {
            _label -> setPos (_radius, -2 * (_radius + 1));
        } else {
            _label -> setPos (-(_label -> boundingRect ().width () + 4), -2 * (_radius + 1));
        }
}

void QNEPort::setHighlight (const PortHighlight& highlight) {
    if (highlight == PortHighlight::NONE) {
        QBrush brush = QBrush (CalenhadServices::preferences ()->calenhad_port_in_fill_color);
        QPen pen = QPen (CalenhadServices::preferences ()->calenhad_port_in_border_color, CalenhadServices::preferences ()->calenhad_port_border_weight);
        setBrush (brush);
        setPen (pen);
    }

    if (highlight == PortHighlight::CAN_CONNECT) {
        QBrush brush = QBrush (CalenhadServices::preferences ()->calenhad_port_in_fill_color_drop);
        QPen pen = QPen (CalenhadServices::preferences ()->calenhad_port_in_border_color_drop, CalenhadServices::preferences ()->calenhad_port_border_weight);
        setBrush (brush);
        setPen (pen);
    }

    if (highlight == PortHighlight::CONNECTED) {
        QBrush brush = QBrush (CalenhadServices::preferences ()->calenhad_port_in_fill_color_connected);
        QPen pen = QPen (CalenhadServices::preferences ()->calenhad_port_in_border_color_connected, CalenhadServices::preferences ()->calenhad_port_border_weight);
        setBrush (brush);
        setPen (pen);
    }

}

bool QNEPort::hasConnection() {
    return (! (m_connections.isEmpty()));
}

QRectF QNEPort::boundingRect() const {
    QRectF r = QRectF (- (_radius + 5), -(_radius + 5),  2 * (_radius + 5), 2 * (_radius + 5));
    return  r;
}

int QNEPort::index() {
    return _index;
}

QNode* QNEPort::owner ()  {
    return _block -> module();
}

void QNEPort::invalidateRenders() {
    _block -> module() -> invalidate();
}

QString& QNEPort::portName() {
    return _portName;
}
