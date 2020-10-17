//
// Created by ferret on 16/10/2020.
//

#ifndef CALENHAD_TECTONICS_H
#define CALENHAD_TECTONICS_H

#include "IcosphereModule.h"

namespace calenhad {
    namespace module {

        class Tectonics : public IcosphereModule {
        public:
            explicit Tectonics (const QString& type = CalenhadServices::preferences() -> calenhad_module_tectonics);
            ~Tectonics() override;
        };


    }
}


#endif //CALENHAD_TECTONICS_H
