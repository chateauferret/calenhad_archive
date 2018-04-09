//
// Created by martin on 26/10/16.
//

#include <iostream>
#include <QPixmap>
#include "QNotification.h"
using namespace calenhad::notification;

QNotification::QNotification (const QString& title, const QString& message, QWidget* host, const NotificationStyle& style, const int& duration) : QFrame (host), _duration (duration) {
    setObjectName ("info");
    setFrameStyle (QFrame::Panel | QFrame::Raised);
    setLineWidth (2);
    if (style == NotificationStyle::ErrorNotification) { setObjectName ("error"); }
    if (style == NotificationStyle::WarningNotification) { setObjectName ("warning"); }
    if (style == NotificationStyle::InfoNotification) { setObjectName ("info"); }
    QPixmap icon (":/appicons/messages/" + objectName() + ".png");
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowModality (Qt::NonModal);
    QGridLayout* layout = new QGridLayout (this);
    _message = new QLabel (this);
    _message -> setText (message);
    _message -> setWordWrap (true);
    _message -> setObjectName ("message");
    _title = new QLabel (this);
    _title -> setText (title);
    _title -> setWordWrap (true);
    _title -> setObjectName ("title");
    _icon = new QLabel (message);
    _icon -> setPixmap (icon);
    _icon -> setFixedSize (24, 24);
    _icon -> setObjectName ("icon");
    layout -> addWidget (_title, 0, 0, 1, 3);
    layout -> addWidget (_icon, 1, 0, 3, 1);
    layout -> addWidget (_message, 2, 1, 1, 2);
    QTimer* timer = new QTimer (this);
    timer->setInterval (_duration);
    timer->setSingleShot (true);
    connect (timer, SIGNAL (timeout()), this, SLOT (dismiss()), Qt::QueuedConnection);
    connect (this, SIGNAL (complete()), timer, SLOT (start()), Qt::QueuedConnection);
}

QNotification::~QNotification() {

}

void QNotification::showEvent (QShowEvent* e) {
    emit complete();
}

void QNotification::mousePressEvent (QMouseEvent* e) {
    dismiss();
}

void QNotification::dismiss() {
    emit dismissed();
}

void QNotification::setDuration (const int& duration) {
    _duration = duration;
}

int QNotification::duration() {
    return _duration;
}