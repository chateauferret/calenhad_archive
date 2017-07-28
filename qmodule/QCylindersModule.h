//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QCYLINDERSMODULE_H
#define CALENHAD_QCYLINDERSMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>
#include <QDoubleSpinBox>

namespace calenhad {
    namespace qmodule {

        class QCylindersModule : public QModule {
        Q_OBJECT
        public:
            static QCylindersModule* newInstance ();

            virtual ~QCylindersModule ();

            void initialise () override;

            Q_PROPERTY (double frequency
                                READ
                                        frequency
                                WRITE
                                setFrequency);

            double frequency ();

            noise::module::Cylinders* module () override;

            QCylindersModule* clone () override;

            QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

        public slots:

            void setFrequency (double value);

        protected:
            QDoubleSpinBox* frequencySpin;

            QCylindersModule (QWidget* parent = 0);


        };

    }
}
#endif //CALENHAD_QCYLINDERSMODULE_H
