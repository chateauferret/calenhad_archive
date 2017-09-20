//
// Created by martin on 20/09/17.
//

#ifndef CALENHAD_PORTNAMEVALIDATOR_H
#define CALENHAD_PORTNAMEVALIDATOR_H
#include "NodeNameValidator.h"

namespace calenhad {
    namespace nodeedit {
        class QNEPort;

        class PortNameValidator : public NodeNameValidator {
        public:
            PortNameValidator (calenhad::nodeedit::QNEPort* port);
            ~PortNameValidator();
            virtual QValidator::State validate (QString &input, int &pos) const override;

        protected:
            QNEPort* _port;
        };
    }
}

#endif //CALENHAD_PORTNAMEVALIDATOR_H
