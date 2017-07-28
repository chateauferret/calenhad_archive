//
// Created by martin on 26/11/16.
//

#ifndef CALENHAD_QCOMBINERMODULE_H
#define CALENHAD_QCOMBINERMODULE_H


#include <QString>
#include "QRangeModule.h"
#include <libnoise/module/clamp.h>

namespace calenhad {
    namespace qmodule {
        class QClampModule : public QRangeModule {
        Q_OBJECT
        public:
            static QClampModule* newInstance ();

            void initialise () override;

            noise::module::Clamp* module () override;

            QString nodeType () override;

            double upperBound () override;

            double lowerBound () override;

            QClampModule* clone () override;

            virtual void inflate (const QDomElement& element) override;

            virtual void serialize (QDomDocument& doc) override;

        private:
            QClampModule (QWidget* parent = 0);

            void setBounds (double lowerBound, double upperBound) override;
        };
    }
}


#endif //CALENHAD_QCOMBINERMODULE_H
