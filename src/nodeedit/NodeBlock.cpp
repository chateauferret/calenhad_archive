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

#include "NodeBlock.h"
#include <QPainter>
#include "Calenhad.h"
#include "module/Module.h"
#include "EditableLabel.h"
#include "Connection.h"
#include "../pipeline/ModuleFactory.h"
#include "../nodeedit/CalenhadView.h"
#include "Port.h"
#include <QGraphicsSceneMouseEvent>
#include "../pipeline/CalenhadModel.h"
#include "NodeNameValidator.h"
#include "../exprtk/Calculator.h"
#include <QApplication>

using namespace calenhad::controls;
using namespace calenhad::nodeedit;
using namespace calenhad::module;



NodeBlock::NodeBlock (Node* node, QGraphicsItem* parent) : QGraphicsPathItem (parent), _node (node), _nameLabel (nullptr), _expression (QString::null),
                                                           _size (QSizeF (0, 0)),
                                                           _endorsementOrright (QPixmap (":/appicons/status/orright.png")),
                                                           _endorsementGoosed (QPixmap (":/appicons/status/goosed.png")) {

    _margin = CalenhadServices::preferences() -> calenhad_handle_module_margin;
    _spacing = CalenhadServices::preferences() -> calenhad_port_spacing + 2 * CalenhadServices::preferences() -> calenhad_port_radius;
    _nameValidator = new NodeNameValidator (_node);
    setZValue (0);

    // type label
    _typeLabel = new QGraphicsTextItem(this);
    _typeLabel->setPlainText(_node->nodeType());
}

// Use this constructor to build a nodeblock without a model so that we can create an icon representing the node type
NodeBlock::NodeBlock (const QString& type) : QGraphicsPathItem (nullptr), _node (nullptr), _nameLabel (nullptr), _expression (QString::null),
                                                           _size (QSizeF (0, 0)),
                                                           _endorsementOrright (QPixmap (":/appicons/status/orright.png")),
                                                           _endorsementGoosed (QPixmap (":/appicons/status/goosed.png")) {

    _margin = CalenhadServices::preferences() -> calenhad_handle_module_margin;
    _spacing = CalenhadServices::preferences() -> calenhad_port_spacing + 2 * CalenhadServices::preferences() -> calenhad_port_radius;
    _nameValidator = nullptr;

    // type label
    _typeLabel = new QGraphicsTextItem(this);
    _typeLabel->setPlainText (type);
    setZValue (0);
}

NodeBlock::~NodeBlock() {

}


void NodeBlock::initialise() {

    if (_size.height() == 0) { _size.setHeight (CalenhadServices::preferences() -> calenhad_handle_module_height); }
    if (_size.width() == 0) { _size.setWidth (CalenhadServices::preferences() -> calenhad_handle_module_width); }
    if (! (_node -> name() == QString())) {

        setCursor (Qt::OpenHandCursor);

        setFlag (QGraphicsItem::ItemIsMovable);
        setFlag (QGraphicsItem::ItemIsSelectable);
        setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
        setFlag (QGraphicsItem::ItemSendsGeometryChanges);

        // name label
        _nameLabel = new EditableLabel(this);
        _oldName = _node->name();
        _nameLabel->setText(_node->name());
        _nameLabel->setTextColor(CalenhadServices::preferences()->calenhad_module_text_color_normal);
        _nameLabel->setValidator(_nameValidator);
        connect(_nameLabel, &EditableLabel::textEdited, this, [=]() {
            QString name = _nameLabel->toPlainText();
            _node->propertyChangeRequested("name", name);
        });

        if (_expression != QString::null) {
            _textLabel = new EditableLabel(this);
            _nameLabel->setText(_expression);
            _nameLabel->setTextColor(CalenhadServices::preferences()->calenhad_module_text_color_normal);
            _nameLabel->setValidator(_nameValidator);
            connect(_nameLabel, &EditableLabel::textEdited, this, [=]() {
                QString name = _nameLabel->toPlainText();
                _node->propertyChangeRequested("name", name);
            });
        }

        connect(_node, &Node::nameChanged, this, [=]() { _nameLabel->setText(_node->name()); });
    }

    setPath (makePath ());
}

QVariant NodeBlock::itemChange (GraphicsItemChange change, const QVariant &value) {
    if (node()) {
        if (CalenhadServices::preferences() -> calenhad_desktop_grid_snap && change == ItemPositionChange) {
            QPointF newPos = value.toPointF();
            if (QApplication::mouseButtons() == Qt::LeftButton) {
                node()->model()->snapToGrid(newPos);
            }
            return newPos;
        } else {
            return QGraphicsItem::itemChange(change, value);
        }
    } else return 0;
}

QPainterPath NodeBlock::makePath() {
    if (! (_node -> name() == QString())) {
        _nameLabel->setPos(_size.width() / 2 - (_nameLabel->boundingRect().width() / 2), _size.height() + _margin);
    }

    _typeLabel -> setPos (_size.width() / 2 - (_typeLabel -> boundingRect().width() / 2 ),  - _margin);
    // shape of the block's body
    QPainterPath p;
    QPolygonF polygon;
    polygon = QRectF (0, 0, _size.width() + 2 * _margin, _size.height() + 2 * _margin);
    setPen (QPen (CalenhadServices::preferences() -> calenhad_module_text_color_normal, CalenhadServices::preferences() -> calenhad_port_border_weight));
    setBrush (CalenhadServices::preferences() -> calenhad_module_brush_color_normal);
    p.addPolygon (polygon);
    return p;
}

void NodeBlock::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option)
//    _nameLabel -> setVisible (_node -> nameVisible());
    _brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_module_brush_color_selected : CalenhadServices::preferences() -> calenhad_module_brush_color_normal);
    painter -> setBrush (_brush);
    _pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_module_text_color_selected : CalenhadServices::preferences() -> calenhad_module_text_color_normal);
    painter -> setPen (_pen);
    QPainterPath p = makePath();
    painter -> drawPath (p);
    if (node()) {
        QPixmap endorsement = _node->isComplete() ? _endorsementOrright : _endorsementGoosed;
        // text, if any
        if (!(_expression.isNull() || _expression.isEmpty())) {
            QFont f = painter->font();
            f.setPointSize(6);
            painter->setFont(f);
            QString text = QString::number(CalenhadServices::calculator()->compute (_expression));
            painter->drawText(_margin, (int) _size.height() + _margin - 2, text);
            painter->drawPixmap((int) p.boundingRect().width() - endorsement.width() - _margin,
                                (int) (_margin + p.boundingRect().height() - endorsement.height()) / 2 - 1,
                                endorsement);
        } else {
            painter->drawPixmap((int) _size.width() + _margin - endorsement.width(),
                                    (int) _size.height() + _margin - endorsement.height(), endorsement);
        }
    }
}

Port* NodeBlock::addPort (Port* port) {
    port -> setParentItem (this);
    _inputs = 0;


    int yInput = (_size.height() - _spacing) / 2;
    foreach (QGraphicsItem* item, childItems()) {
        if (item->type () == Port::Type) {
            Port* port = (Port*) item;
            if (port -> portType () == Port::OutputPort) {
                qreal v = (_size.height() / 2) + _margin;
                qreal r = _size.width() + 2 * _margin + port -> radius();
                port -> setPos (r, v);
            } else {
                _inputs++;
            }
        }
    }

    yInput -= _spacing * _inputs / 2 - _margin;
    foreach (QGraphicsItem* item, childItems()) {
        if (item->type () == Port::Type) {
            Port* port = (Port*) item;
            if (port -> portType () != Port::OutputPort) {
                qreal r = -port -> radius();
               yInput += _spacing;
                port -> setPos (r, yInput);
            }
        }
    }

    port -> setBlock (this);
    port -> initialise();
    if (node()) {
        connect (port, &Port::connected, _node, &Node::invalidate);
        connect (port, &Port::disconnected, _node, &Node::invalidate);
    }
    return port;
}

QVector<Port*> NodeBlock::inputs() {
    if (dynamic_cast<Module*> (_node)) {
        Module* m = (Module*) _node;
        QVector<Port*> ports = m -> ports();
        Port* out = output();
        ports.remove (ports.indexOf (out));
        return ports;
    } else {
        return QVector<Port*>();
    }
}

Port* NodeBlock::output() {
    if (dynamic_cast<Module*> (_node)) {
        Module* m = (Module*) _node;
        return m -> output ();
    } else {
        return nullptr;
    }
}

QVector<Port*> NodeBlock::controls() {
    QVector<Port*> res;
    foreach (QGraphicsItem* p, parentItem() -> childItems()) {
        if (p -> type() == Port::Type) {
            Port* port = (Port*) p;
            if (port -> portType () != Port::ControlPort) {
                res.append (port);
            }
        }
    }
    return res;
}

void NodeBlock::mouseMoveEvent (QGraphicsSceneMouseEvent* event) {

    QGraphicsItem::mouseMoveEvent (event);

}

void NodeBlock::mousePressEvent (QGraphicsSceneMouseEvent* event) {
    setCursor (Qt::ClosedHandCursor);
    _oldZ = zValue();
    // don't raise the event to the superclass because this would cancel the selection
}

void NodeBlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent (event);
}

QRectF NodeBlock::boundingRect() const {
    QRectF r = QGraphicsPathItem::boundingRect();
        foreach (QGraphicsItem* item, childItems ()) {
            if (item -> type() == Port::Type) {
                r = r.united (item -> boundingRect());
            }
        }
    if (_nameLabel) {
        r = r.united(_nameLabel -> boundingRect());
    }
    return r;
}

Node* NodeBlock::node () {
    return _node;
}

void NodeBlock::mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event) {
    _node -> showModuleDetail (true);
}

void NodeBlock::nodeChanged () {
    _nameLabel -> setPlainText (_node -> name());
    _nameLabel -> setPos (_size.width() - (_nameLabel -> boundingRect().width() / 2 ), _size.height() + 3 * _margin);
    _node -> invalidate();
}

void NodeBlock::setText (const QString& text) {
    _expression = text;
}
