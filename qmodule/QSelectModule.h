//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSELECTMODULE_H
#define CALENHAD_QSELECTMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include "QRangeModule.h"
#include <libnoise/module/select.h>

namespace calenhad {
    namespace qmodule {
        class QSelectModule : public QRangeModule {
        Q_OBJECT
        public:
            static QSelectModule* newInstance ();

            virtual ~QSelectModule ();

            void initialise () override;

            Q_PROPERTY (double falloff
                                READ
                                        falloff
                                WRITE
                                setFalloff);

            double falloff ();

            double lowerBound ();

            double upperBound ();

            noise::module::Select* module () override;

            QSelectModule* clone () override;

            virtual QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

        public slots:

            void setFalloff (double value);

        protected:
            QSelectModule (QWidget* parent = 0);

            QDoubleSpinBox* falloffSpin;


            void setBounds (double lowerBound, double upperBound) override;
        };
    }
}


#endif //CALENHAD_QSELECTMODULE_H
