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
#include "module/Module.h"
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
    p1 = p;
}

void Connection::setPos2 (const QPointF& p) {
    p2 = p;
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
    p1 = m_port1 -> scenePos();
    if (m_port2) { p2 = m_port2 -> scenePos(); }
}

void Connection::updatePath() {

    QPainterPath path1, path2;
    QPen pen;
    QBrush brush;

    qreal dx = p2.x () - p1.x ();
    qreal dy = p2.y () - p1.y ();

    QVector<QPointF> points;

    QPointF q1, q2, r1, r2, c;


    qreal w = CalenhadServices::preferences() -> calenhad_handle_module_height;
    c = QPointF (p1.x () + dx * 0.5, p1.y () + dy * 0.5);
    int d = dy < 0 ? -1 : 1;

    // Two paths identical but in opposite directions, so that the one half fills in the holes the rendering leaves in the other FFS.


    if (dx < 0) {
        q1 = QPointF (p1.x() + w * 2, p1.y() + w * d);
        q2 = QPointF (p2.x() - w * 2, p2.y() - w * d);
        r2 = QPointF (p2.x(), p2.y() - w * d);
        r1 = QPointF (p1.x(), p1.y() + w * d);
        path1.moveTo (p1);
        path1.quadTo (q1, c);
        //path1.quadTo (c, r2);
        path1.quadTo (q2, p2);
        path2.moveTo (p2);
        path2.quadTo (q2, c);
       // path2.quadTo (c, r1);
        path2.quadTo (q1, p1);
    } else {
        q2 = QPointF (p1.x() + w, p2.y());
        q1 = QPointF (p1.x() + w, p1.y());
        r2 = QPointF (p1.x() + dx * 0.75, p2.y());
        r1 = QPointF (p1.x() + dx * 0.25, p1.y());
        path1.moveTo (p1);
        path1.quadTo (r1, c);
        path1.quadTo (r2, p2);
        path2.moveTo (p2);
        path2.quadTo (r2, c);
        path2.quadTo (r1, p1);

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

void Connection::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter -> setRenderHint (QPainter::Antialiasing);
    QGraphicsPathItem::paint (painter, option, widget);
}