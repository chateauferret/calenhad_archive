//
// Created by martin on 24/10/16.
//

#ifndef CALENHAD_MESSAGEFACTORY_H
#define CALENHAD_MESSAGEFACTORY_H
#include <QWidget>
#include <QVector>
#include "nodeedit/qnemessagebox.h"
#include <QTextStream>

class CalenhadMessageStream;

class MessageFactory : public QObject {
    Q_OBJECT
public:
    MessageFactory ();
    ~MessageFactory();
    void message (const QString& title, const QString& message);
    void setHost (QWidget* host);
    public slots:
    void clearMessage (QNEMessageBox* message);

private:
    QWidget* _host = 0;
    QVector<QNEMessageBox*> messages;
};




#endif //CALENHAD_MESSAGEFACTORY_H
