
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

using namespace icosphere;

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

            int vertexCount();
            void setDepth (const int& depth);
            void setBounds (const icosphere::Bounds& bounds);
            void setModule (noise::module::Module* module);
            void setKey (const QString& key);
            void build ();


        public slots:
            void buildAsync ();
            void cancel();
            void fill();

        signals:
            void complete (std::shared_ptr<Icosphere>);
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