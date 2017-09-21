//
// Created by martin on 24/10/16.
//

#include <QStyle>
#include <QtCore/QTimer>
#include <QtWidgets/QAbstractSpinBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <memory>
#include <iostream>
#include "QNotificationStack.h"
#include "QProgressNotification.h"
using namespace calenhad::notification;

QNotificationStack::QNotificationStack (int width, QWidget* parent) : QWidget (parent), _parent (parent), _width (width) {
    setLayout (new QVBoxLayout());
    layout() -> setAlignment (Qt::AlignBottom | Qt::AlignRight);
    setWindowFlags (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setFixedWidth (width);
    setFixedHeight (parent -> height());
    setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Ignored));
    move (parent -> width() - width - 5, 5);
    show();
}

QNotificationStack::~QNotificationStack() {

}

QNotification* QNotificationStack::message (const QString& style, const QString& message, const int& duration) {
    QNotification* notification = new QNotification (message, _host, style);
    notification -> setDuration (duration);
    notification -> setFixedWidth (width());
    QTimer* timer = new QTimer (notification);
    timer->setInterval (duration);
    timer->setSingleShot (true);
    connect (timer, SIGNAL (timeout()), notification, SLOT (dismiss()), Qt::QueuedConnection);
    connect (notification, SIGNAL (displayed()), timer, SLOT (start()), Qt::QueuedConnection);
    connect (notification, SIGNAL (dismissed (QNotification*)), this, SLOT (clearMessage (QNotification*)), Qt::QueuedConnection);
    layout() -> addWidget (notification);
    std::cout << message.toStdString() << "\n";
    return notification;
}

QProgressNotification* QNotificationStack::progress (const QString& style, const QString& message, const int& duration, const int& toDo, const int& delay) {
    QProgressNotification* notification = new QProgressNotification (message, nullptr);
    notification -> setToDo (toDo);
    notification -> setDuration (duration);
    notification -> setFixedWidth (width());
    QTimer* timer = new QTimer (notification);
    timer->setInterval (duration);
    timer->setSingleShot (true);
    connect (timer, SIGNAL (timeout()), notification, SLOT (dismiss()), Qt::QueuedConnection);
    connect (notification, SIGNAL (complete()), timer, SLOT (start()), Qt::QueuedConnection);
    connect (notification, SIGNAL (dismissed (QNotification*)), this, SLOT (clearMessage (QNotification*)));
    QTimer* showTimer = new QTimer (notification);
    showTimer -> setInterval (delay);
    showTimer -> setSingleShot (true);
    connect (showTimer, &QTimer::timeout, this, [=] () { layout() -> addWidget (notification);  });
    showTimer -> start();
    return notification;
}

void QNotificationStack::clearMessage (QNotification* message) {
    // find the message with this id and dispose it
    layout() -> removeWidget (message);
}
