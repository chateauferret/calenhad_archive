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

class QNEMessageBox : public QFrame {
    Q_OBJECT
public:
    QNEMessageBox (const QString& message, QWidget* parent);
    ~QNEMessageBox();
    void showEvent (QShowEvent* e);
    void mousePressEvent (QMouseEvent* e);
    void setIndex (const int& index);

public slots:
    void dismiss();

signals:
    void messageDismissed (QNEMessageBox* source);

private:
    QTimer* _timer;
    QLabel* _message;
};


#endif //CALENHAD_QNEMESSAGEBOX_H
