//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QEXPONENTMODULE_H
#define CALENHAD_QEXPONENTMODULE_H


#include <QtWidgets/QWidget>
#include "QModule.h"
#include <libnoise/module/module.h>


namespace calenhad {
    namespace qmodule {

        class QExponentModule : public QModule {
        Q_OBJECT
        public:
            static QExponentModule* newInstance ();

            virtual ~QExponentModule ();

            void initialise () override;

            Q_PROPERTY (double exponent
                                READ
                                        exponent
                                WRITE
                                setExponent);

            double exponent ();

            noise::module::Exponent* module () override;

            QExponentModule* clone () override;

            QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

        public slots:

            void setExponent (double value);

        protected:
            QExponentModule (QWidget* parent = 0);

            QDoubleSpinBox* exponentSpin;
        };
    }
}

#endif //CALENHAD_QEXPONENTMODULE_H
