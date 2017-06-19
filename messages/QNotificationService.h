//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_MESSAGESERVICE_H
#define CALENHAD_MESSAGESERVICE_H
#include <QObject>

class QMessageService : public QObject {
    Q_OBJECT
public:
    virtual int message (const QString& title, const QString& message, const int& duration = 10000) = 0;
    virtual void setHost (QWidget* host) = 0;
};


#endif //CALENHAD_MESSAGESERVICE_H
