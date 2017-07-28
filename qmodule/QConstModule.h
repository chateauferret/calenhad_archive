//
// Created by martin on 27/11/16.
//

#ifndef CALENHAD_QCONSTANTMODULE_H
#define CALENHAD_QCONSTANTMODULE_H
#include "QModule.h"
#include <libnoise/module/const.h>

namespace calenhad {
    namespace expressions {
        class ExpressionWidget;
    }
    namespace qmodule {
        class QConstModule : public QModule {
        Q_OBJECT
        public:
            void initialise ();

            double constValue ();

            noise::module::Const* module () override;

            Q_PROPERTY (QString constantValue
                                READ
                                        constValue
                                WRITE
                                setConstValue ());

            static QConstModule* newInstance ();

            QConstModule* clone () override;

            QString nodeType () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;


        public slots:
            void valueReady (const double& value) override;
            void setConstValue (QString exp);

        private:
            calenhad::expressions::ExpressionWidget* _constValueWidget;

            QConstModule (QWidget* parent = 0);

        };
    }
}




#endif //CALENHAD_QCONSTANTMODULE_H
