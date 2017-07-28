//
// Created by martin on 15/04/17.
//

#ifndef CALENHAD_MESSAGESERVICE_H
#define CALENHAD_MESSAGESERVICE_H
#include <QObject>

namespace calenhad {
    namespace notification {

        class QNotification;

        class QProgressNotification;

        class QNotificationService {
        public:
            virtual QNotification* message (const QString& title, const QString& message, const int& duration = 10000) = 0;

            virtual QProgressNotification*
            progress (const QString& title, const QString& message, const int& duration = 10000, const int& toDo = 100, const int& delay = 500) = 0;
        };
    }
}


#endif //CALENHAD_MESSAGESERVICE_H
