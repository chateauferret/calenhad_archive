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
    QNotification (const QString& message, QWidget* host, const QString& style="info", const int& duration = 10000);
    ~QNotification();
    virtual void showEvent (QShowEvent* e);
    virtual void mousePressEvent (QMouseEvent* e) override;
    void setDuration (const int& duration);

public slots:
    void dismiss();

signals:
    void dismissed (QNotification*);
    void displayed();

protected:
    QNotification ();
    QLabel* _message;
    int _duration;
};


#endif //CALENHAD_QNEMESSAGEBOX_H
