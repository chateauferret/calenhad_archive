//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QSCALEMODULE_H
#define CALENHAD_QSCALEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/scalepoint.h>

namespace calenhad {
    namespace qmodule {
        class QScaleModule : public QModule {
        Q_OBJECT
        public:
            static QScaleModule* newInstance ();

            virtual ~QScaleModule ();

            void initialise () override;

            Q_PROPERTY (double scaleX
                                READ
                                        scaleX
                                WRITE
                                setScaleX);
            Q_PROPERTY (double scaleY
                                READ
                                        scaleY
                                WRITE
                                setScaleY);
            Q_PROPERTY (double scaleZ
                                READ
                                        scaleZ
                                WRITE
                                setScaleZ);

            double scaleX ();

            double scaleY ();

            double scaleZ ();

            noise::module::ScalePoint* module () override;

            QScaleModule* clone () override;

            virtual QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;


        public slots:

            void setScaleX (double value);

            void setScaleY (double value);

            void setScaleZ (double value);

        protected:
            QDoubleSpinBox* scaleXSpin, * scaleYSpin, * scaleZSpin;

            QScaleModule (QWidget* parent = 0);
        };
    }
}


#endif //CALENHAD_QSCALEMODULE_H
