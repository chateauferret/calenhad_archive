//
// Created by martin on 24/10/16.
//

#ifndef CALENHAD_MESSAGEFACTORY_H
#define CALENHAD_MESSAGEFACTORY_H
#include <QWidget>
#include <QVector>
#include <QTextStream>
#include <memory>
#include <QtWidgets/QMainWindow>

namespace calenhad {
    namespace notification {


        class CalenhadMessageStream;

        class QNotification;
        class QProgressNotification;

        class QNotificationHost : public QMainWindow {
        Q_OBJECT
        public:
            QNotificationHost (QWidget* parent);

            ~QNotificationHost ();


            QNotification* message (const QString& title, const QString& message, const int& duration = 10000);

            QProgressNotification* progress (const QString& title, const QString& message, const int& duration = 10000, const int& toDo = 100, const int& delay = 500);
            void clearAll ();
        public slots:

            void clearMessage();
            void reorder();

        private:

            QList<QNotification*> _messages;
            int _width;


            void display (QNotification* notification);

            int _margin;



        };
    }
}




#endif //CALENHAD_MESSAGEFACTORY_H
