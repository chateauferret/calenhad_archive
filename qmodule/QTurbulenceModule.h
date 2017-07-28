//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QTURBULENCEMODULE_H
#define CALENHAD_QTURBULENCEMODULE_H

#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

namespace calenhad {
    namespace qmodule {
        class QTurbulenceModule : public QModule {
        Q_OBJECT

        public:
            static QTurbulenceModule* newInstance ();

            virtual ~QTurbulenceModule ();

            void initialise () override;

            Q_PROPERTY (double frequency
                                READ
                                frequency
                                WRITE
                                setFrequency);
            Q_PROPERTY (double power
                                READ
                                power
                                WRITE
                                setPower);
            Q_PROPERTY (double roughness
                                READ
                                roughness
                                WRITE
                                setRoughness);

            double frequency ();

            double power ();

            double roughness ();

            noise::module::Turbulence* module () override;

            QString nodeType () override;

            QTurbulenceModule* clone () override;

        public slots:

            void setFrequency (double value);

            void setPower (double value);

            void setRoughness (double value);

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;


        protected:
            QTurbulenceModule (QWidget* parent = 0);

            QDoubleSpinBox* frequencySpin, * powerSpin, * roughnessSpin;
        };
    }
}


#endif //CALENHAD_QTURBULENCEMODULE_H
