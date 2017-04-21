//
// Created by martin on 24/10/16.
//

#include "messagefactory.h"

MessageFactory::MessageFactory()  {
}

MessageFactory::~MessageFactory() {

}

int MessageFactory::message (const QString& title, const QString& message) {
    QNEMessageBox* msgBox = new QNEMessageBox (message, _host);
    messages.append (msgBox);
    msgBox -> setIndex (messages.indexOf (msgBox));
    connect (msgBox, SIGNAL (messageDismissed (int)), this, SLOT (clearMessage (const int&)));
    return msgBox -> id();
}

void MessageFactory::setHost (QWidget* host) {
    _host = host;
}

void MessageFactory::clearMessage (const int& id) {
    // find the message with this id and dispose it
    for (QNEMessageBox* message : messages) {
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
