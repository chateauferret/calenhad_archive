//
// Created by martin on 26/10/16.
//

#include "qnemessagebox.h"

int QNEMessageBox::nextId = 0;

QNEMessageBox::QNEMessageBox (const QString& message, QWidget* parent) : QFrame (parent), _timer (new QTimer()) {
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setWindowModality (Qt::NonModal);
    QLayout* layout = new QVBoxLayout (this);
    _message = new QLabel (message);
    layout -> addWidget (_message);
    show();
    _id = nextId++;
}

QNEMessageBox::~QNEMessageBox () {
    if (_timer) { delete _timer; }
}

void QNEMessageBox::showEvent (QShowEvent* e) {
    _timer -> setInterval (10000);
    _timer -> setSingleShot (true);
    connect (_timer, SIGNAL (timeout()), this, SLOT (dismiss()));
    _timer -> start();
}

void QNEMessageBox::mousePressEvent (QMouseEvent* e) {
    _timer -> stop();
    disconnect (_timer, SIGNAL (timeout()), this, SLOT (dismiss()));
    dismiss();
}

void QNEMessageBox::dismiss() {
    close();
    emit messageDismissed (_id);
}

void QNEMessageBox::setIndex (const int& index) {
    QWidget* w = (QWidget*) parent();
    int x = w -> geometry().x() + w -> width() - width() - 5;
    int y = w -> geometry().y() + w -> height() - (height() * (index + 1)) - 5;
    QPoint pos = QPoint (x, y);
    move (pos);
}

int QNEMessageBox::id () {
    return _id;
}
