//
// Created by martin on 28/06/17.
//

#ifndef CALENHAD_ICOSPHEREMODULE_H
#define CALENHAD_ICOSPHEREMODULE_H

#include <memory>
#include <QtCore/QString>
#include "libnoise/module/modulebase.h"
#include "Bounds.h"

namespace icosphere {
    class Icosphere;
}
namespace calenhad {
    namespace notification {
        class QProgressNotification;
    }
}

using namespace icosphere;

namespace noise {
    namespace module {

        class IcosphereModule : public QObject, public noise::module::Module {
        Q_OBJECT
        public:
            IcosphereModule ();

            virtual ~IcosphereModule ();

            int vertexCount ();

            int GetSourceModuleCount () const override;

            double GetValue (double x, double y, double z) const override;

            void setKey (const QString& key);

            void buildIcosphere (const Bounds& bounds, const int& depth);

        signals:
            void available();

        protected:
            std::shared_ptr<icosphere::Icosphere> _icosphere;
            QString _key;
            calenhad::notification::QProgressNotification* _notification;
            icosphere::Bounds _bounds;
            int _depth;

        protected slots:

            void built (std::shared_ptr<Icosphere> icosphere);
        };
    }
}


#endif //CALENHAD_ICOSPHEREMODULE_H
