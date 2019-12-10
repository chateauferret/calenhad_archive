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
#include "QNotificationHost.h"


namespace calenhad {
    namespace notification {

        class QNotification : public QFrame {
        Q_OBJECT
        public:

            QNotification (const QString& title, const QString& message, QWidget* host, const NotificationStyle& style = NotificationStyle::InfoNotification, const int& duration = 10000);

            ~QNotification ();

            virtual void showEvent (QShowEvent* e);

            virtual void mousePressEvent (QMouseEvent* e) override;

            void setDuration (const int& duration);
            int duration ();

        public slots:

            void dismiss ();

        signals:

            void dismissed();

            void complete ();

        protected:

            QNotification ();
            QLabel* _icon;
            QLabel* _message;
            QLabel* _title;
            int _duration;


        };
    }
}



#endif //CALENHAD_QNEMESSAGEBOX_H
