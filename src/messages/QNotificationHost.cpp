//
// Created by martin on 24/10/16.
//

#include <QStyle>
#include <QtCore/QTimer>
#include <QMouseEvent>
#include <QtWidgets/QAbstractSpinBox>
#include <QtWidgets/QLayout>
#include <src/CalenhadServices.h>
#include "../preferences/PreferencesService.h"
#include "QNotificationHost.h"
#include "QProgressNotification.h"

using namespace calenhad::notification;

QNotificationHost::QNotificationHost (QWidget* parent) : QMainWindow (parent),
    _width (CalenhadServices::preferences() -> calenhad_notifications_width),
    _margin (CalenhadServices::preferences() -> calenhad_notifications_margin) {
}

QNotificationHost::~QNotificationHost() {

}

QNotification* QNotificationHost::message (const QString& title, const QString& message, const NotificationStyle& style, const int& duration) {
    QNotification* notification = new QNotification (title, message, this, style);
    notification -> setDuration (duration);
    notification -> setFixedWidth (_width);
    display (notification);
    return notification;
}

void QNotificationHost::display (QNotification* notification) {
    QTimer* timer = new QTimer (notification);
    timer -> setInterval (notification -> duration());
    timer -> setSingleShot (true);
    connect (timer, SIGNAL (timeout()), notification, SLOT (dismiss()), Qt::QueuedConnection);
    connect (notification, SIGNAL (dismissed()), this, SLOT (clearMessage()), Qt::QueuedConnection);
    connect (notification, SIGNAL (complete()), timer, SLOT (start()), Qt::QueuedConnection);
    _messages.append (notification);
    notification -> show();
    reorder();
}

QProgressNotification* QNotificationHost::progress (const QString& title, const QString& message, const NotificationStyle& style, const int& duration, const int& toDo, const int& delay) {
    QProgressNotification* notification = new QProgressNotification (title, message, nullptr);
    notification -> setToDo (toDo);
    notification -> setDuration (duration);
    notification -> setFixedWidth (_width);
    display (notification);
    return notification;
}

void QNotificationHost::clearMessage() {
    // find the message with this id and dispose it
    QNotification* message = dynamic_cast<QNotification*> (sender());
    if (message) {
        _messages.removeOne (message);
        message -> close ();
        reorder ();
    }
}

void QNotificationHost::clearAll() {
    for (QNotification* message : _messages) {
        _messages.removeOne (message);
        message -> close ();
    }
}

void QNotificationHost::reorder() {
    int x = width() - _width - _margin;
    int y = height();
    for (QNotification* n : _messages) {
        y -= n -> height();
        y -= _margin;
        n -> move (mapToGlobal (QPoint (x, y)));
    }
}