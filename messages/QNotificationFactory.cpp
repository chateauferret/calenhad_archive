//
// Created by martin on 24/10/16.
//

#include "QNotificationFactory.h"

QNotificationFactory::QNotificationFactory()  {
}

QNotificationFactory::~QNotificationFactory() {

}

int QNotificationFactory::message (const QString& style, const QString& message, const int& duration) {
    QNotification* notification = new QNotification (message, _host, style);
    notification -> setDuration (duration);
    messages.append (notification);
    notification -> setIndex (messages.indexOf (notification));
    connect (notification, SIGNAL (dismissed (int)), this, SLOT (clearMessage (const int&)));
    notification -> show();
    return notification -> id();
}

void QNotificationFactory::setHost (QWidget* host) {
    _host = host;
}

void QNotificationFactory::clearMessage (const int& id) {
    // find the message with this id and dispose it
    for (QNotification* message : messages) {
        if (message -> id() == id) {
            messages.remove (messages.indexOf (message));
            delete message;
        }
    }

    // reorder the remaining messages
    for (int i = 0; i < messages.count(); i++) {
        messages.at (i) -> setIndex (i);
    }

}
