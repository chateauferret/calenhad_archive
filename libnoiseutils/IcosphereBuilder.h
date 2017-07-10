
//
// Created by martin on 06/02/17.
//

#ifndef CALENHAD_NOISEMAPBUILDERICOSPHERE_H
#define CALENHAD_NOISEMAPBUILDERICOSPHERE_H

#include <string>
#include "../icosphere/icosphere.h"
#include <QMutex>

namespace icosphere {
    class Icosphere;
}

class QModule;

namespace noise {
    namespace module {
        class Module;
    }
    namespace utils {

        class IcosphereBuilder : public QObject {
        Q_OBJECT
        public:
            IcosphereBuilder (std::shared_ptr<icosphere::Icosphere> icosphere);
            virtual ~IcosphereBuilder();
            
            virtual std::shared_ptr<icosphere::Icosphere> icosphere();
            void fill();
            int vertexCount();
            void setDepth (const int& depth);
            void setBounds (const icosphere::Bounds& bounds);
            void setModule (noise::module::Module* module);
            void setKey (const QString& key);

        public slots:
            void build();
            void cancel();

        signals:
            void complete();
            void abandoned();
            void progress (const int&);
            void status (const QString&);

        protected:
            std::shared_ptr<icosphere::Icosphere> _icosphere;
            int _depth;
            icosphere::Bounds _bounds;
            geoutils::Cartesian c;
            bool _cancelled = false;
            QMutex _mutex;
            bool isAbandoned();
            noise::module::Module* _module;
            QString _key;
        };

    }
}

#endif //CALENHAD_NOISEMAPBUILDERICOSPHERE_H