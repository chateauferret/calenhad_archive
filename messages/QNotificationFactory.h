//
// Created by martin on 24/10/16.
//

#ifndef CALENHAD_MESSAGEFACTORY_H
#define CALENHAD_MESSAGEFACTORY_H
#include <QWidget>
#include <QVector>
#include "QNotification.h"
#include "QNotificationService.h"
#include <QTextStream>

class CalenhadMessageStream;

class QNotificationFactory : public QMessageService {
    Q_OBJECT
public:
    QNotificationFactory ();
    ~QNotificationFactory();
    int message (const QString& title, const QString& message, const int& duration = 10000) override;
    void setHost (QWidget* host) override;
    public slots:
    void clearMessage (const int& id);

private:
    QWidget* _host = 0;
    QVector<QNotification*> messages;

};




#endif //CALENHAD_MESSAGEFACTORY_H
