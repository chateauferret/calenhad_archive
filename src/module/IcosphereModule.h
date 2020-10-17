//
// Created by martin on 12/10/18.
//

#ifndef MESSAGES_ICOSPHEREMODULE_H
#define MESSAGES_ICOSPHEREMODULE_H

#include <src/grid/icosphere.h>
#include "Module.h"
#include "../grid/icosphere.h"
#include "StructuredGrid.h"


namespace calenhad {
    namespace module {

        class IcosphereModule : public Module {
        public:

            explicit IcosphereModule (const QString& type);
            ~IcosphereModule() override;

            void initialise();

            bool isComplete() override;

            grid::Icosphere* icosphere ();

            void fetch (calenhad::grid::CubicSphere* buffer) override;

        protected:
            grid::Icosphere* _icosphere{};

        };
    }
}


#endif //MESSAGES_ICOSPHEREMODULE_H
