//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QTRANSLATEMODULE_H
#define CALENHAD_QTRANSLATEMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>

namespace calenhad {
    namespace qmodule {

        class QTranslateModule : public QModule {
        Q_OBJECT
        public:
            static QTranslateModule* newInstance ();

            virtual ~QTranslateModule ();

            void initialise () override;

            Q_PROPERTY (double dX
                                READ
                                        dX
                                WRITE
                                setDX);
            Q_PROPERTY (double dY
                                READ
                                        dY
                                WRITE
                                setDY);
            Q_PROPERTY (double dZ
                                READ
                                        dZ
                                WRITE
                                setDZ);

            double dX ();

            double dY ();

            double dZ ();

            noise::module::TranslatePoint* module () override;

            QTranslateModule* clone () override;

            virtual QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

        public slots:

            void setDX (double value);

            void setDY (double value);

            void setDZ (double value);

        protected:
            QTranslateModule (QWidget* parent = 0);

            QDoubleSpinBox* dXSpin, * dYSpin, * dZSpin;
        };
    }
}

#endif //CALENHAD_QTRANSLAtEMODULE_H
