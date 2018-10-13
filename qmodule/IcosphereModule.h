//
// Created by martin on 12/10/18.
//

#ifndef MESSAGES_ICOSPHEREMODULE_H
#define MESSAGES_ICOSPHEREMODULE_H

#include "Module.h"
#include "../icosphere/icosphere.h"


namespace calenhad {
    namespace qmodule {

        class IcosphereModule : public Module {
        public:
            IcosphereModule (Module* parent = 0);

            virtual ~IcosphereModule ();

            void initialise () override;

            bool isComplete  () override;

            QString glsl () override;


        protected:
            icosphere::Icosphere* _icosphere;

        };
    }
}


#endif //MESSAGES_ICOSPHEREMODULE_H
