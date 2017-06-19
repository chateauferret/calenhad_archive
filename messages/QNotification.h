//
// Created by martin on 26/10/16.
//

#ifndef CALENHAD_QNEMESSAGEBOX_H
#define CALENHAD_QNEMESSAGEBOX_H
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QObject>
#include <QtCore/QUuid>

class QNotification : public QFrame {
    Q_OBJECT
public:
    QNotification (const QString& message, QWidget* parent, const QString& style="info");
    ~QNotification();
    void showEvent (QShowEvent* e);
    void mousePressEvent (QMouseEvent* e);
    void setIndex (const int& index);
    void setDuration (const int& duration);
    int id();

public slots:
    void dismiss();

signals:
    void dismissed (int id);

private:
    QTimer* _timer;
    QLabel* _message;
    int _duration;
    int _id;
    static int nextId;
};


#endif //CALENHAD_QNEMESSAGEBOX_H
