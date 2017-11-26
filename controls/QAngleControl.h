//
// Created by martin on 19/11/17.
//

#ifndef CALENHAD_QANGLECONTROL_H
#define CALENHAD_QANGLECONTROL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QStackedLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

namespace calenhad {
    namespace controls {
        enum AngleType { Latitude, Longitude, Bearing };
        class QAngleControl : public QWidget {
            Q_OBJECT

        public:
            QAngleControl (const QString& caption, const AngleType& type, QWidget* parent = 0);
            virtual ~QAngleControl();
            double value();
            void setValue (const double& value);

        public slots:
            void toggleFormat();


        signals:
            void valueChanged (const double& value);

        protected:
            QLineEdit* _decimalDegreesText, * _tradDegreesText;
            QStackedLayout* _stack;
            QHBoxLayout* _layout;
            QPushButton* _formatButton;
            QHBoxLayout* _tradLayout, * _decimalLayout;
            double _value;
            void trimBox (QWidget* w);
            AngleType _type;

        protected slots:
            void decimalUpdated();
            void tradUpdated();

        };
    }
}



#endif //CALENHAD_QANGLECONTROL_H
