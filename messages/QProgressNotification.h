//
// Created by martin on 19/06/17.
//

#ifndef CALENHAD_QPROGRESSNOTIFICATION_H
#define CALENHAD_QPROGRESSNOTIFICATION_H


#include "QNotification.h"
#include <QProgressBar>

class QProgressNotification : public QNotification {
    Q_OBJECT
public:
    QProgressNotification (const QString& message, QWidget* parent);
    virtual ~QProgressNotification();
    void showEvent (QShowEvent* e) override;
    void mousePressEvent (QMouseEvent* e) override;
    void setToDo (const int& toDo);

public slots:
    void setMessage (const QString& message);
    void setProgress (const int& progress);
    void kill();

signals:
    void complete();
    void cancel();

protected:
    QProgressBar* _progressBar;
    double _progress, _toDo;


};


#endif //CALENHAD_QPROGRESSNOTIFICATION_H
