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
#include <QMimeData>
#include <QDragEnterEvent>
#include <QtGui/QDrag>
#include <QtGui/QPainter>
#include <QtWidgets/QGridLayout>
#include "NodePalette.h"

NodePalette::NodePalette (QWidget *parent) : QWidget (parent) {
    setMinimumSize (200, 200);
    setAcceptDrops (true);
    setLayout (new QGridLayout ());
    QLabel* groupIcon = new QLabel (this);
    groupIcon->setPixmap (QPixmap ("/home/martin/ClionProjects/calenhad/resources/icons/nodegroup.png").scaled (32, 32));

    groupIcon -> setObjectName ("NodeGroup");
    ((QGridLayout*) layout ()) -> addWidget (groupIcon);
    groupIcon->setAttribute (Qt::WA_DeleteOnClose);
}

void NodePalette::mousePressEvent(QMouseEvent *event) {
    QLabel* child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child) return;

    const QPixmap* pixmap = child -> pixmap();
    QString type = child -> objectName();

    QByteArray itemData;
    QDataStream dataStream (&itemData, QIODevice::WriteOnly);
    //dataStream << pixmap << type;
    dataStream << type;
    QMimeData *mimeData = new QMimeData;
    mimeData -> setData ("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag (this);
    drag -> setMimeData(mimeData);
    drag -> setPixmap (*pixmap);
    drag -> setObjectName (child -> objectName());

    QPixmap tempPixmap = QPixmap (*pixmap);
    QPainter painter;
    painter.begin (&tempPixmap);
    painter.fillRect (pixmap -> rect(), QColor(127, 127, 127, 127));
    painter.end();

    child -> setPixmap (tempPixmap);

    if (drag -> exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child -> close();
    } else {
        child -> show();
        child -> setPixmap (*pixmap);
    }
}