//
// Created by martin on 19/06/17.
//

#include <QtWidgets/QProgressBar>
#include <iostream>
#include "QProgressNotification.h"

using namespace calenhad::notification;

QProgressNotification::QProgressNotification (const QString& message, QWidget* host) : QNotification (message, host)  {
    _progressBar = new QProgressBar (this);
    layout() -> addWidget (_progressBar);
    _progressBar -> setOrientation (Qt::Horizontal);
    _progressBar -> setMinimum (0);
    _progressBar -> setFixedHeight (10);
}

QProgressNotification::~QProgressNotification () {

}

void QProgressNotification::showEvent (QShowEvent* e) {
    std::cout << "Shown\n";
    emit displayed();
}

void QProgressNotification::setProgress (const int& progress) {
    _progress = progress;

    if (_progress >= _toDo) {
        emit complete();
    }
    _progressBar -> setValue (progress);
    //std::cout << "Progress: " << progress << "\n";
}

void QProgressNotification::setComplete() {
    _progress = _toDo;
    emit complete();
    _progressBar -> setValue (_progress);
}

void QProgressNotification::kill() {
    std::cout << "Killed\n";
    dismiss();
}

void QProgressNotification::setToDo (const int& toDo) {
    _toDo = toDo;
    _progressBar -> setMaximum (toDo);
}

void QProgressNotification::mousePressEvent (QMouseEvent* e) {
    dismiss();

}

void QProgressNotification::setMessage (const QString& message) {
    std::cout << "Message: " << message.toStdString () << "\n";
    _message -> setText (message);
}
