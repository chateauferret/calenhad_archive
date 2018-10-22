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

#include "Connection.h"
#include "qmodule/Module.h"
#include "../CalenhadServices.h"
#include "Port.h"
#include "../preferences/PreferencesService.h"
#include "Toolbox.h"
#include <QMenu>
#include <math.h>

using namespace calenhad::notification;
using namespace calenhad::nodeedit;

Connection::Connection (QGraphicsItem* parent) : QGraphicsPathItem (parent) {
    setFlag (QGraphicsItem::ItemIsSelectable);
    setFlag (QGraphicsItem::ItemClipsToShape);
    setFlag (QGraphicsItem::ItemIsFocusable, false);
    setZValue (-1);
    m_port1 = 0;
    m_port2 = 0;
}

Connection::~Connection() {
    if (m_port1) {
        Port* p1 = m_port1;
        p1 -> removeConnection (this);
        m_port1 = nullptr;
    }
    if (m_port2) {
        Port* p2 = m_port2;
        p2 -> removeConnection (this);
        m_port2 = nullptr;
    }
}

void Connection::setPos1 (const QPointF& p) {
    pos1 = p;
}

void Connection::setPos2 (const QPointF& p) {
    pos2 = p;
}

void Connection::setPort1 (Port* p) {
    m_port1 = p;
    m_port1 -> addConnection (this);
}

void Connection::setPort2 (Port* p) {
    if (p) {
        m_port2 = p;
        m_port2->addConnection (this);
    } else {
        m_port2 -> removeConnection (this);
    }
}

void Connection::updatePosFromPorts () {
    pos1 = m_port1 -> scenePos();
    if (m_port2) { pos2 = m_port2 -> scenePos(); }
}

void Connection::updatePath() {

    QPainterPath path1, path2;
    QPen pen;
    QBrush brush;

    qreal dx = pos2.x () - pos1.x () - 50;
    qreal dy = pos2.y () - pos1.y ();

    QVector<QPointF> points;
    points << pos1;

    points << QPointF (pos1.x() + 25.0, pos1.y());

    if (dx < 25.0) {
        points << QPointF (pos1.x() + 25.0, pos1.y() + 25.0 * (dy < 0 ? -1 : 1));;
    }

    QPointF p1 = QPointF (pos1.x () + 25.0 + dx * 0.25, pos1.y ());
    QPointF c = QPointF (pos1.x () + 25.0 + dx * 0.5, pos1.y () + dy * 0.5);
    QPointF p2 = QPointF (pos1.x () + 25.0 + dx * 0.75, pos2.y ());

    if (dx < 25.0) {
        points << QPointF (pos2.x() - 25.0, pos2.y() - 25.0 * (dy < 0 ? -1 : 1));
    }

    points << QPointF (pos2.x() - 25.0, pos2.y());
    points << pos2;

    for (int i = 1; i < points.size(); i++) {
    //    p.lineTo (points [i]);
    }

    if (m_port2) {
        if (isSelected ()) {
            pen = QPen (CalenhadServices::preferences() -> calenhad_connector_selected_color, CalenhadServices::preferences() -> calenhad_connector_selected_weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        } else {
            pen = QPen (CalenhadServices::preferences() -> calenhad_connector_normal_color, CalenhadServices::preferences() -> calenhad_connector_normal_weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        }
    } else {
        pen = QPen (CalenhadServices::preferences() -> calenhad_connector_drawing_color, CalenhadServices::preferences() -> calenhad_connector_drawing_weight,  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    setPen (pen);
    path1.moveTo (pos1);
    path1.quadTo (p1, c);
    path1.quadTo (p2, pos2);
    path2.moveTo (pos2);
    path2.quadTo (p2, c);
    path2.quadTo (p1, pos1);
    QPainterPath p;
    p.addPath (path1);
    p.addPath (path2);
    setPath (p);
}

Port* Connection::port1 () const {
    return m_port1;
}

Port* Connection::port2 () const {
    return m_port2;
}

void Connection::serialise (QDomElement& element) {
    if (m_port1 && m_port2) {
        QDomDocument doc = element.ownerDocument();
        QDomElement connectionElement = doc.createElement ("connection");
        element.appendChild (connectionElement);
        QDomElement connectionFromElement = doc.createElement ("source");
        connectionElement.appendChild (connectionFromElement);
        QDomElement connectionToElement = doc.createElement ("target");
        connectionElement.appendChild (connectionToElement);
        connectionFromElement.setAttribute ("module", m_port1 -> owner() -> name ());
        connectionToElement.setAttribute ("module", m_port2 -> owner() -> name ());
        connectionFromElement.setAttribute ("output", m_port1 -> index());
        connectionToElement.setAttribute ("input", m_port2 -> index ());
    }
}

void Connection::inflate (const QDomDocument& doc) {
    // to do - handle name and geometry here. Connecting the ports is dealt with in CalenhadModel::inflate.
}

Port* Connection::otherEnd (Port* port) {
    if (port) {
        if (port == m_port1) { return m_port2; }
        if (port == m_port2) { return m_port1; }
    }
    return nullptr;
}

void Connection::mousePressEvent (QGraphicsSceneMouseEvent* e) {
    QGraphicsItem::mousePressEvent (e);
}
