//
// Created by martin on 24/10/16.
//

#ifndef CALENHAD_MESSAGEFACTORY_H
#define CALENHAD_MESSAGEFACTORY_H
#include <QWidget>
#include <QVector>
#include "QNotificationService.h"
#include <QTextStream>
#include <memory>

namespace calenhad {
    namespace notification {


        class CalenhadMessageStream;

        class QNotification;

        class QNotificationStack : public QWidget, public QNotificationService {
        Q_OBJECT
        public:
            QNotificationStack (int width = 100, QWidget* parent = 0);

            ~QNotificationStack ();

            QNotification* message (const QString& title, const QString& message, const int& duration = 10000) override;

            QProgressNotification* progress (const QString& title, const QString& message, const int& duration = 10000, const int& toDo = 100, const int& delay = 500) override;

        public slots:

            void clearMessage (QNotification* message);

        private:
            QWidget* _host = 0;


            int _width;
            QWidget* _parent;

        };
    }
}




#endif //CALENHAD_MESSAGEFACTORY_H
