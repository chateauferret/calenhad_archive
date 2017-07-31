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
            QConstModule (QWidget* parent = 0);
            void initialise() {};
            QConstModule* clone() {};
        public slots:

        private:


        };
    }
}




#endif //CALENHAD_QCONSTANTMODULE_H
