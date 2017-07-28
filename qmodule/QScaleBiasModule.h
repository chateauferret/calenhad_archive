//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSCALEBIASMODULE_H
#define CALENHAD_QSCALEBIASMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/scalebias.h>

namespace calenhad {
    namespace qmodule {

        class QScaleBiasModule : public QModule {
        Q_OBJECT
        public:
            static QScaleBiasModule* newInstance ();

            virtual ~QScaleBiasModule ();

            void initialise () override;

            Q_PROPERTY (double scale
                                READ
                                        scale
                                WRITE
                                setScale);
            Q_PROPERTY (double bias
                                READ
                                        bias
                                WRITE
                                setBias);

            double scale ();

            double bias ();

            noise::module::ScaleBias* module () override;

            QScaleBiasModule* clone ();

            virtual QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;


        public slots:

            void setScale (double value);

            void setBias (double value);

        protected:
            QDoubleSpinBox* scaleSpin, * biasSpin;

            QScaleBiasModule (QWidget* parent = 0);

        };
    }
}


#endif //CALENHAD_QSCALEBIASMODULE_H
