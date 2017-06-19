//
// Created by martin on 26/10/16.
//

#include <iostream>
#include "QNotification.h"

int QNotification::nextId = 0;

QNotification::QNotification (const QString& message, QWidget* parent, const QString& style) : QFrame (parent), _timer (new QTimer()) {
    setObjectName (style.toLower());
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality (Qt::NonModal);
    QLayout* layout = new QVBoxLayout (this);
    _message = new QLabel (objectName() +"\n" + message);
    layout -> addWidget (_message);
    _id = nextId++;
}

QNotification::~QNotification () {
    if (_timer) { delete _timer; }
}

void QNotification::showEvent (QShowEvent* e) {
    if (_duration > 0) {
        _timer->setInterval (_duration);
        _timer->setSingleShot (true);
        connect (_timer, SIGNAL (timeout ()), this, SLOT (dismiss ()));
        _timer->start ();
    }
}

void QNotification::mousePressEvent (QMouseEvent* e) {
    _timer -> stop();
    disconnect (_timer, SIGNAL (timeout()), this, SLOT (dismiss()));
    dismiss();
}

void QNotification::dismiss() {
    close();
    emit dismissed (_id);
}

void QNotification::setIndex (const int& index) {
    QWidget* w = (QWidget*) parent();
    int x = w -> geometry().x() + w -> width() - width() - 5;
    int y = w -> geometry().y() + w -> height() - (height() * (index + 1)) - 5;
    QPoint pos = QPoint (x, y);
    move (pos);
}

int QNotification::id () {
    return _id;
}

void QNotification::setDuration (const int& duration) {
    _duration = duration;
}
