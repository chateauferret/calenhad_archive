/*
 * SizeGripItem - A size grip QGraphicsItem for interactive resizing.
 * https://github.com/cesarbs/sizegripitem
 * Copyright (c) 2011 Cesar L. B. Silveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QBrush>
#include <iostream>
#include <QtCore/QTimer>
#include <QGraphicsSceneHoverEvent>
#include <cmath>
#include "SizeGripItem.h"

using namespace calenhad::controls;

SizeGripItem::HandleItem::HandleItem (int positionFlags, SizeGripItem* parent)
        : QGraphicsRectItem (-4, -4, 8, 8, parent),
          positionFlags_ (positionFlags),
          parent_ (parent) {
    setBrush (QBrush (Qt::lightGray));
    setFlag (ItemIsMovable);
    setFlag (ItemSendsGeometryChanges);
}

int SizeGripItem::HandleItem::positionFlags () const {
    return positionFlags_;
}

QVariant SizeGripItem::HandleItem::itemChange (GraphicsItemChange change, const QVariant& value) {
    QVariant retVal = value;

    if (change == ItemPositionChange) {
        retVal = restrictPosition (value.toPointF ());
    } else if (change == ItemPositionHasChanged) {
        QPointF pos = value.toPointF ();

        switch (positionFlags_) {
            case TopLeft:
                parent_->setTopLeft (pos);
                break;
            case Top:
                parent_->setTop (pos.y ());
                break;
            case TopRight:
                parent_->setTopRight (pos);
                break;
            case Right:
                parent_->setRight (pos.x ());
                break;
            case BottomRight:
                parent_->setBottomRight (pos);
                break;
            case Bottom:
                parent_->setBottom (pos.y ());
                break;
            case BottomLeft:
                parent_->setBottomLeft (pos);
                break;
            case Left:
                parent_->setLeft (pos.x ());
                break;
        }
    }

    return retVal;
}

QPointF SizeGripItem::HandleItem::restrictPosition (const QPointF& newPos) {
    QPointF retVal = pos ();

    if (positionFlags_ & Top || positionFlags_ & Bottom) {
        retVal.setY (newPos.y ());
    }

    if (positionFlags_ & Left || positionFlags_ & Right) {
        retVal.setX (newPos.x ());
    }

    if (positionFlags_ & Top && retVal.y () > parent_->_rect.bottom ()) {
        retVal.setY (parent_->_rect.bottom ());
    } else if (positionFlags_ & Bottom && retVal.y () < parent_->_rect.top ()) {
        retVal.setY (parent_->_rect.top ());
    }

    if (positionFlags_ & Left && retVal.x () > parent_->_rect.right ()) {
        retVal.setX (parent_->_rect.right ());
    } else if (positionFlags_ & Right && retVal.x () < parent_->_rect.left ()) {
        retVal.setX (parent_->_rect.left ());
    }

    return retVal;
}

SizeGripItem::SizeGripItem (Resizer* resizer, QGraphicsItem* parent)
        : QGraphicsItem (parent),
          resizer_ (resizer) {
    setAcceptHoverEvents (true);
    if (parentItem ()) {
        _rect = parentItem ()->boundingRect ();
    }

    handleItems_.append (new HandleItem (TopLeft, this));
    handleItems_.append (new HandleItem (Top, this));
    handleItems_.append (new HandleItem (TopRight, this));
    handleItems_.append (new HandleItem (Right, this));
    handleItems_.append (new HandleItem (BottomRight, this));
    handleItems_.append (new HandleItem (Bottom, this));
    handleItems_.append (new HandleItem (BottomLeft, this));
    handleItems_.append (new HandleItem (Left, this));
    updateHandleItemPositions ();

    timer.setInterval (1000);
    timer.setSingleShot (true);
    connect (&timer, &QTimer::timeout, this, [=] () { setOpacity (0.01); });
    setOpacity (0.01);
}

SizeGripItem::~SizeGripItem () {
    if (resizer_) {
        delete resizer_;
    }
}

QRectF SizeGripItem::boundingRect () const {
    return _rect;
}

void SizeGripItem::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
}

#define IMPL_SET_FN(TYPE, POS)                  \
    void SizeGripItem::set ## POS (TYPE v)      \
    {                                           \
        _rect.set ## POS (v);                   \
        doResize();                             \
    }

IMPL_SET_FN(qreal, Top)
IMPL_SET_FN(qreal, Right)
IMPL_SET_FN(qreal, Bottom)
IMPL_SET_FN(qreal, Left)
IMPL_SET_FN(const QPointF&, TopLeft)
IMPL_SET_FN(const QPointF&, TopRight)
IMPL_SET_FN(const QPointF&, BottomRight)
IMPL_SET_FN(const QPointF&, BottomLeft)

void SizeGripItem::doResize () {
    if (resizer_) {
        (*resizer_) (parentItem (), _rect);
        updateHandleItemPositions ();
    }
}

void SizeGripItem::hoverEnterEvent (QGraphicsSceneHoverEvent* event) {
    if (! isSelected()) {
        setOpacity (1.0);
        timer.start ();
    }
}

void SizeGripItem::hoverLeaveEvent (QGraphicsSceneHoverEvent* event) {
    timer.start();
}

void SizeGripItem::hoverMoveEvent (QGraphicsSceneHoverEvent* event) {
    if ((! isSelected()) && std::abs (event -> pos().x() - _rect.left()) < _margin
            || std::abs (event -> pos().x() - _rect.right()) < _margin
            || std::abs (event -> pos().y() - _rect.top()) < _margin
            || std::abs (event -> pos().y() - _rect.bottom()) < _margin) {
        setOpacity (1.0);
    } else {
        timer.start();
    }
    event -> ignore ();
}

void SizeGripItem::updateHandleItemPositions () {
            foreach (HandleItem* item, handleItems_) {
            item->setFlag (ItemSendsGeometryChanges, false);

            switch (item->positionFlags ()) {
                case TopLeft:
                    item->setPos (_rect.topLeft ());
                    break;
                case Top:
                    item->setPos (_rect.left () + _rect.width () / 2 - 1, _rect.top ());
                    break;
                case TopRight:
                    item->setPos (_rect.topRight ());
                    break;
                case Right:
                    item->setPos (_rect.right (), _rect.top () + _rect.height () / 2 - 1);
                    break;
                case BottomRight:
                    item->setPos (_rect.bottomRight ());
                    break;
                case Bottom:
                    item->setPos (_rect.left () + _rect.width () / 2 - 1, _rect.bottom ());
                    break;
                case BottomLeft:
                    item->setPos (_rect.bottomLeft ());
                    break;
                case Left:
                    item->setPos (_rect.left (), _rect.top () + _rect.height () / 2 - 1);
                    break;
            }

            item->setFlag (ItemSendsGeometryChanges, true);
        }
}