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
#include "../preferences/PreferencesService.h"
#include <src/CalenhadServices.h>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QtWidgets>
#include "QIconPalette.h"
#include "FlowLayout.h"
#include "../pipeline/ModuleFactory.h"

using namespace calenhad;
using namespace calenhad::controls;

QIconPalette::QIconPalette (QWidget *parent) : QWidget (parent), _iconSize (CalenhadServices::preferences() -> calenhad_toolpalette_icon_size) {

    _topScrollButton = new QPushButton ();
    _topScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_up.png"));
    _topScrollButton -> setFixedHeight (CalenhadServices::preferences() -> calenhad_toolpalette_arrow_size);

    _bottomScrollButton = new QPushButton ();
    _bottomScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_down.png"));
    _bottomScrollButton -> setFixedHeight (CalenhadServices::preferences() -> calenhad_toolpalette_arrow_size);

    _leftScrollButton = new QPushButton ();
    _leftScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_left.png"));
    _leftScrollButton -> setFixedWidth (CalenhadServices::preferences() -> calenhad_toolpalette_arrow_size);

    _rightScrollButton = new QPushButton ();
    _rightScrollButton -> setIcon (QIcon (":/appicons/controls/arrow_right.png"));
    _leftScrollButton -> setFixedWidth (CalenhadServices::preferences() -> calenhad_toolpalette_arrow_size);

    setAcceptDrops (true);
    setLayout (new QStackedLayout());

    // add all the widgets in the right order. We will show and hide them appropriately when the context changes.

    _iconsPanel = new QWidget();
    _iconsPanel -> setLayout (new FlowLayout());

    _horizontalScrollPanel = new QScrollArea();
    _horizontalIconPanel = new QWidget();
    _horizontalIconPanel -> setLayout (new QHBoxLayout());
    _horizontalIconPanel -> layout() -> setSizeConstraint(QLayout::SetMinimumSize);
    _horizontalIconPanel -> layout() -> setMargin (0);
    _horizontalScrollPanel -> setWidget (_horizontalIconPanel);
    _horizontalScrollPanel -> setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    _horizontalScrollPanel -> setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    _horizontalScrollPanel -> setFrameStyle (QFrame::NoFrame);

    _verticalScrollPanel = new QScrollArea();
    _verticalIconPanel = new QWidget();
    _verticalIconPanel -> setLayout (new QVBoxLayout());
    _verticalIconPanel -> layout() -> setSizeConstraint(QLayout::SetMinimumSize);
    _verticalScrollPanel -> setWidget (_verticalIconPanel);
    _verticalScrollPanel -> setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    _verticalScrollPanel -> setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    _verticalScrollPanel -> setFrameStyle (QFrame::NoFrame);

    _horizontalIconPanel -> layout() -> setMargin (0);
    _verticalIconPanel -> layout() -> setMargin (0);

    _verticalPanel = new QWidget();
    _verticalPanel -> setLayout (new QVBoxLayout());
    _verticalPanel -> setMinimumWidth (_iconSize);
    _verticalPanel -> layout() -> addWidget (_topScrollButton);
    _verticalPanel -> layout() -> addWidget (_verticalScrollPanel);
    _verticalPanel -> layout() -> addWidget (_bottomScrollButton);

    _horizontalPanel = new QWidget();
    _horizontalPanel -> setLayout (new QHBoxLayout());
    _horizontalPanel -> setMinimumHeight (_iconSize);
    _horizontalPanel -> layout() -> addWidget (_leftScrollButton);
    _horizontalPanel -> layout() -> addWidget (_horizontalScrollPanel);
    _horizontalPanel -> layout() -> addWidget (_rightScrollButton);

    _horizontalPanel -> layout() -> setMargin (0);
    _verticalPanel -> layout() -> setMargin (0);

    layout() -> addWidget (_horizontalPanel);
    layout() -> addWidget (_verticalPanel);
    layout() -> addWidget (_iconsPanel);

    QList<QWidget*> iconPanels;
    iconPanels.append (_horizontalIconPanel);
    iconPanels.append (_verticalIconPanel);
    iconPanels.append (_iconsPanel);

    _types = CalenhadServices::modules() -> types();
    for (QString type : _types) {
        for (QWidget* panel : iconPanels) {
            QLabel* label = new QLabel();
            QPixmap p = CalenhadServices::modules() -> getIcon (type);

            label -> setAlignment (Qt::AlignCenter);
            if (! p.isNull()) {
                label -> setPixmap (p);
                //label -> setFixedSize (_iconSize, _iconSize);
            } else {
                label -> setText (type);
            }

            label -> setObjectName (type);
            panel -> layout() -> addWidget (label);
            label -> setAttribute (Qt::WA_DeleteOnClose);
        }
    }

    connect (_rightScrollButton, &QPushButton::pressed, this, [=] () { updateScrollButtons(); });
    connect (_leftScrollButton, &QPushButton::pressed, this, [=] () { updateScrollButtons(); });
    connect (_topScrollButton, &QPushButton::pressed, this, [=] () { updateScrollButtons(); });
    connect (_bottomScrollButton, &QPushButton::pressed, this, [=] () { updateScrollButtons(); });
}

void QIconPalette::updateScrollButtons() {
    QPushButton* s = (QPushButton*) sender();
    if (s == _rightScrollButton) {
        _horizontalScrollPanel -> horizontalScrollBar() -> setValue (_horizontalScrollPanel->horizontalScrollBar() -> value() + _horizontalScrollPanel -> horizontalScrollBar() -> singleStep());
    }
    if (s == _leftScrollButton) {
        _horizontalScrollPanel -> horizontalScrollBar() -> setValue (_horizontalScrollPanel->horizontalScrollBar() -> value() - _horizontalScrollPanel -> horizontalScrollBar() -> singleStep());
    }
    if (s == _topScrollButton) {
        _verticalScrollPanel -> verticalScrollBar() -> setValue (_verticalScrollPanel->verticalScrollBar() -> value() - _verticalScrollPanel -> verticalScrollBar() -> singleStep());
    }
    if (s == _bottomScrollButton) {
        _verticalScrollPanel -> verticalScrollBar() -> setValue (_verticalScrollPanel->verticalScrollBar() -> value() + _verticalScrollPanel -> verticalScrollBar() -> singleStep());
    }
}

void QIconPalette::resizeEvent (QResizeEvent* e) {
    layoutIcons();
}

void QIconPalette::layoutIcons() {
    if (_area == Qt::NoDockWidgetArea) {
        setMaximumSize (300, 300);
        setMinimumHeight (_iconSize * 2);
        setMinimumWidth (_iconSize * 2);

        ((QStackedLayout*) layout()) -> setCurrentWidget (_iconsPanel);
    }
    if (_area == Qt::LeftDockWidgetArea || _area == Qt::RightDockWidgetArea) {
       setFixedWidth (_iconSize + 2);
        setMaximumHeight (QWIDGETSIZE_MAX);
        ((QStackedLayout*) layout()) -> setCurrentWidget (_verticalPanel);
    }
    if (_area == Qt::TopDockWidgetArea || _area == Qt::BottomDockWidgetArea) {
       setFixedHeight (_iconSize + 2);
        setMaximumWidth (QWIDGETSIZE_MAX);
        ((QStackedLayout*) layout()) -> setCurrentWidget (_horizontalPanel);
    }
}

void QIconPalette::mousePressEvent(QMouseEvent *event) {
    QLabel* child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child) return;

    QPixmap pixmap = child -> grab();
    QString type = child -> objectName();
    if ((! type.isNull() && _types.contains (type))) {
        QByteArray itemData;
        QDataStream dataStream (&itemData, QIODevice::WriteOnly);

        dataStream << type;
        QMimeData* mimeData = new QMimeData;
        mimeData->setData ("application/x-dnditemdata", itemData);

        //if (pixmap && !(pixmap -> isNull ())) {
            QDrag* drag = new QDrag (this);
            drag->setMimeData (mimeData);
            drag->setPixmap (pixmap);
            drag->setObjectName (child->objectName ());

            if (drag->exec (Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
                child->close ();
            } else {
                child->show ();
                child->setPixmap (pixmap);
            }
        //}
    }
}

void QIconPalette::moved (Qt::DockWidgetArea area) {
    _area = area;
    QDockWidget* dock = (QDockWidget*) parent();
    if (_area == Qt::LeftDockWidgetArea || _area == Qt::RightDockWidgetArea) {
        dock -> setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetVerticalTitleBar);
    } else {
        dock -> setFeatures (QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    }
    layoutIcons();
    updateGeometry();
    adjustSize();

}
