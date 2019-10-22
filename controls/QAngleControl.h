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
#include <QValidator>
#include <module/ParamValidator.h>
#include "../geoutils.h"

namespace calenhad {
    namespace controls {
        class QAngleControl;

        class QAngleControl : public QWidget {
            Q_OBJECT

        public:
            QAngleControl (const QString& caption, const geoutils::AngleType& type, QWidget* parent = 0);
            ~QAngleControl() override;
            double value();
            void setValue (const double& value);
            geoutils::AngleType angleType();
            void refresh ();
            bool isValid();
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
            double _value{};
            void trimBox (QWidget* w);
            geoutils::AngleType _type;
            void setValidator (calenhad::module::AcceptAngle* validator);
            module::AcceptAngle* _validator;
            QLabel* _statusLabel;
            QPixmap _statusOrright;
            QPixmap _statusGoosed;
            bool _isValid;

        protected slots:
            void decimalUpdated();
            void tradUpdated();


            void validate ();
        };
    }
}



#endif //CALENHAD_QANGLECONTROL_H
