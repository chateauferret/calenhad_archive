//
// Created by martin on 06/02/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERICOSPHERE_H
#define CALENHAD_NOISEMAPBUILDERICOSPHERE_H

#include <string>
#include "../icosphere/icosphere.h"

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

        class IcosphereBuilder : public QObject {
            Q_OBJECT
        public:
            IcosphereBuilder ();
            virtual ~IcosphereBuilder();
            void setDepth (const int& depth);
            void setBounds (const icosphere::Bounds& bounds);
            void setModule (noise::module::Module* module);
            virtual std::shared_ptr<icosphere::Icosphere> icosphere();
            void setIcosphere (std::shared_ptr<icosphere::Icosphere> icosphere);

        public slots:
            void build();
            void fill();
            void cancel();

         signals:
            void complete (std::shared_ptr<icosphere::Icosphere>);

        protected:
            std::shared_ptr<icosphere::Icosphere> _icosphere;
            char _depth;
            icosphere::Bounds _bounds;
            geoutils::Cartesian c;
            noise::module::Module* _module;
            bool _cancelled = false;
            QMutex _mutex;
            bool isCancelled ();
        };

    }
}

#endif //CALENHAD_NOISEMAPBUILDERICOSPHERE_H
