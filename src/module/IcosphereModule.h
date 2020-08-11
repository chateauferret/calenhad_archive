//
// Created by martin on 12/10/18.
//

#ifndef MESSAGES_ICOSPHEREMODULE_H
#define MESSAGES_ICOSPHEREMODULE_H

#include "Module.h"
#include "../icosphere/icosphere.h"
#include "StructuredGrid.h"


namespace calenhad {
    namespace module {

        class IcosphereModule : public calenhad::module::RasterModule {
        public:
            IcosphereModule ();

            ~IcosphereModule () override;

            void initialise () override;

            bool isComplete  () override;

            grid::Icosphere* icosphere ();

            QImage* raster() override;

        protected:
            grid::Icosphere* _icosphere{};
            void updateCache() override;

        };
    }
}


#endif //MESSAGES_ICOSPHEREMODULE_H
