//
// Created by martin on 24/10/16.
//

#ifndef CALENHAD_MESSAGEFACTORY_H
#define CALENHAD_MESSAGEFACTORY_H
#include <QWidget>
#include <QVector>
#include "nodeedit/qnemessagebox.h"
#include "MessageService.h"
#include <QTextStream>

class CalenhadMessageStream;

class MessageFactory : public MessageService {
    Q_OBJECT
public:
    MessageFactory ();
    ~MessageFactory();
    int message (const QString& title, const QString& message) override;
    void setHost (QWidget* host) override;
    public slots:
    void clearMessage (const int& id);

private:
    QWidget* _host = 0;
    QVector<QNEMessageBox*> messages;
};




#endif //CALENHAD_MESSAGEFACTORY_H
