//
// Created by martin on 26/10/16.
//

#include <iostream>
#include "QNotification.h"

QNotification::QNotification (const QString& message, QWidget* host, const QString& style, const int& duration) : QFrame (host), _duration (duration) {
    setObjectName (style.toLower());
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowModality (Qt::NonModal);
    QLayout* layout = new QVBoxLayout (this);
    _message = new QLabel (objectName() +"\n" + message);
    layout -> addWidget (_message);

    QTimer* timer = new QTimer (this);
    timer->setInterval (_duration);
    timer->setSingleShot (true);
    connect (timer, SIGNAL (timeout()), this, SLOT (dismiss()), Qt::QueuedConnection);
    connect (this, SIGNAL (displayed()), timer, SLOT (start()), Qt::QueuedConnection);
}

QNotification::~QNotification() {

}

void QNotification::showEvent (QShowEvent* e) {
    emit displayed();
}

void QNotification::mousePressEvent (QMouseEvent* e) {
    dismiss();
}

void QNotification::dismiss() {
    emit dismissed (this);
}

void QNotification::setDuration (const int& duration) {
    _duration = duration;
}