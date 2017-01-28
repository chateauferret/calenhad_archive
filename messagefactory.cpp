//
// Created by martin on 24/10/16.
//

#include "messagefactory.h"

MessageFactory::MessageFactory() {
}

MessageFactory::~MessageFactory() {

}

void MessageFactory::message (const QString& title, const QString& message) {
    QNEMessageBox* msgBox = new QNEMessageBox (message, _host);
    messages.append (msgBox);
    msgBox -> setIndex (messages.indexOf (msgBox));
    connect (msgBox, SIGNAL (messageDismissed (QNEMessageBox*)), this, SLOT (clearMessage (QNEMessageBox*)));
}

void MessageFactory::setHost (QWidget* host) {
    _host = host;
}

void MessageFactory::clearMessage (QNEMessageBox* msgBox) {
    messages.remove (messages.indexOf (msgBox));
    delete msgBox;
    for (int i = 0; i < messages.count(); i++) {
        messages.at (i) -> setIndex (i);
    }

}
