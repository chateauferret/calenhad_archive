/*
   Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
        ** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
        ** Software or, alternatively, in accordance with the terms contained in
        ** a written agreement between you and The Qt Company. For licensing terms
        ** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
        ** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
        **     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
        **     notice, this list of conditions and the following disclaimer in
        **     the documentation and/or other materials provided with the
        **     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
        **     contributors may be used to endorse or promote products derived
        **     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
S INTERRUPTION) HOWEVER CAUSED AND ON ANY
        ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QScroller>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QtGui/QDrag>
#include <QtGui/QPainter>
#include <QtWidgets/QGridLayout>
#include <iostream>
#include <CalenhadServices.h>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QtWidgets>
#include "NodePalette.h"
#include "FlowLayout.h"


NodePalette::NodePalette (QWidget *parent) : QWidget (parent) {

    _topScrollButton = new QPushButton ();
    _topScrollButton -> setFixedSize (36, 36);
    _topScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_up.png"));

    _bottomScrollButton = new QPushButton ();
    _bottomScrollButton -> setFixedSize (36, 36);
    _bottomScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_down.png"));

    _leftScrollButton = new QPushButton ();
    _leftScrollButton -> setFixedSize (36, 36);
    _leftScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_left.png"));

    _rightScrollButton = new QPushButton ();
    _rightScrollButton -> setFixedSize (36, 36);
    _rightScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_right.png"));

    setAcceptDrops (true);
    setLayout (new QStackedLayout());

    // add all the widgets in the right order. We will show and hide them appropriately when the context changes.

    _iconsPanel = new QWidget();
    _iconsPanel -> setLayout (new FlowLayout());

    _horizontalScrollPanel = new QScrollArea();
    _horizontalPanel = new QWidget();
    _horizontalPanel -> setLayout (new QHBoxLayout());
    _horizontalPanel -> layout() -> setSizeConstraint(QLayout::SetMinimumSize);
    _horizontalScrollPanel -> setWidget (_horizontalPanel);
    _horizontalScrollPanel -> setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    _horizontalScrollPanel -> setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

    _verticalScrollPanel = new QScrollArea();
    _verticalPanel = new QWidget();
    _verticalPanel -> setLayout (new QVBoxLayout());
    _verticalPanel -> layout() -> setSizeConstraint(QLayout::SetMinimumSize);
    _verticalScrollPanel -> setWidget (_verticalPanel);
    _verticalScrollPanel -> setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    _verticalScrollPanel -> setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

    _horizontalPanel -> layout() -> setMargin (0);
    _verticalPanel -> layout() -> setMargin (0);

    layout() -> addWidget (_horizontalScrollPanel);
    layout() -> addWidget (_verticalScrollPanel);
    layout() -> addWidget (_iconsPanel);

    QList<QWidget*> iconPanels;
    iconPanels.append (_horizontalPanel);
    iconPanels.append (_verticalPanel);
    iconPanels.append (_iconsPanel);

    _types = CalenhadServices::modules() -> types ();
    for (QString type : _types) {
        for (QWidget* panel : iconPanels) {
            QLabel* icon = new QLabel (this);
            QPixmap* pixmap = CalenhadServices::modules ()->getIcon (type);
            icon->setToolTip (type);
            QPixmap p = (*pixmap).scaled (32, 32);
            icon->setAlignment (Qt::AlignTop | Qt::AlignLeft);
            icon->setPixmap (p);
            icon->setFixedSize (36, 36);
            QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect ();
            effect->setOffset (2);
            effect->setColor (QColor ("#7F7F7F7F"));
            icon->setGraphicsEffect (effect);
            icon->setObjectName (type);
            panel->layout ()->addWidget (icon);
            panel -> layout() -> setSpacing (4);
            icon -> setAttribute (Qt::WA_DeleteOnClose);
        }
    }


    connect (_rightScrollButton, &QPushButton::pressed, this, [=] () { _horizontalScrollPanel -> scroll (-36, 0); updateScrollButtons(); });
    connect (_leftScrollButton, &QPushButton::pressed, this, [=] () { _horizontalScrollPanel -> scroll (36, 0); updateScrollButtons(); });
    connect (_topScrollButton, &QPushButton::pressed, this, [=] () { _verticalScrollPanel -> scroll (0, 36); updateScrollButtons(); });
    connect (_bottomScrollButton, &QPushButton::pressed, this, [=] () { _verticalScrollPanel -> scroll (0, -36); updateScrollButtons(); });
}

void NodePalette::updateScrollButtons() {
    _leftScrollButton -> setEnabled (_horizontalPanel -> findChild<QWidget*> (_types.first()) -> isVisible());
    _rightScrollButton -> setEnabled (_horizontalPanel -> findChild<QWidget*> (_types.last()) -> isVisible());
    _topScrollButton -> setEnabled (_verticalPanel -> findChild<QWidget*> (_types.first()) -> isVisible());
    _bottomScrollButton -> setEnabled (_verticalPanel -> findChild<QWidget*> (_types.last()) -> isVisible());
}

void NodePalette::resizeEvent (QResizeEvent* e) {
    layoutIcons();
}

void NodePalette::layoutIcons() {
    if (_area == Qt::NoDockWidgetArea) {
        setMaximumSize (300, 300);
        setMinimumHeight (36 + 2 * layout() -> margin());
        setMinimumWidth (36 + 2 * layout() -> margin());
        ((QStackedLayout*) layout()) -> setCurrentWidget (_iconsPanel);
    }
    if (_area == Qt::LeftDockWidgetArea || _area == Qt::RightDockWidgetArea) {
        setFixedWidth (36 + 2 * layout() -> margin());
        setMaximumHeight (QWIDGETSIZE_MAX);
        ((QStackedLayout*) layout()) -> setCurrentWidget (_verticalScrollPanel);
    }
    if (_area == Qt::TopDockWidgetArea || _area == Qt::BottomDockWidgetArea) {
        setFixedHeight (36 + layout() -> margin());
        setMaximumWidth (QWIDGETSIZE_MAX);
        ((QStackedLayout*) layout()) -> setCurrentWidget (_horizontalScrollPanel);
    }
}

void NodePalette::mousePressEvent(QMouseEvent *event) {
    QLabel* child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child) return;

    const QPixmap* pixmap = child -> pixmap();
    QString type = child -> objectName();
    std::cout << "Type " << type.toStdString () << "\n";
    if ((! type.isNull() && _types.contains (type))) {
        QByteArray itemData;
        QDataStream dataStream (&itemData, QIODevice::WriteOnly);

        dataStream << type;
        QMimeData* mimeData = new QMimeData;
        mimeData->setData ("application/x-dnditemdata", itemData);

        if (pixmap && !(pixmap->isNull ())) {
            QDrag* drag = new QDrag (this);
            drag->setMimeData (mimeData);
            drag->setPixmap (*pixmap);
            drag->setObjectName (child->objectName ());

            if (drag->exec (Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
                child->close ();
            } else {
                child->show ();
                child->setPixmap (*pixmap);
            }
        }
    }
}

void NodePalette::moved (Qt::DockWidgetArea area) {
    _area = area;
    layoutIcons();
    updateGeometry();
    adjustSize();

}
