//
// Created by martin on 06/02/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERICOSPHERE_H
#define CALENHAD_NOISEMAPBUILDERICOSPHERE_H

#include <string>
#include "../icosphere/icosphere.h"

namespace icosphere {
    class Icosphere;
}

namespace noise {
    namespace utils {
        class Legend;
    }
}


namespace noise {
    namespace module {
        class Module;
    }
    namespace utils {

        class IcosphereBuilder {
        public:
            IcosphereBuilder (const char& depth, const icosphere::Bounds& bounds = icosphere::Bounds());
            virtual ~IcosphereBuilder();
            virtual void build (std::string dataset, noise::utils::Legend* legend, noise::module::Module* module);
            virtual icosphere::Icosphere* icosphere();

        protected:
            icosphere::Icosphere* _icosphere;
            char _depth;
            icosphere::Bounds _bounds;
            geoutils::Cartesian c;
        };

    }
}

#endif //CALENHAD_NOISEMAPBUILDERICOSPHERE_H
