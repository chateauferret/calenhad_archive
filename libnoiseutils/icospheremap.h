//
// Created by martin on 08/03/17.
//

#ifndef CALENHAD_ICOSPHEREMAP_H
#define CALENHAD_ICOSPHEREMAP_H


#include "libnoise/module/modulebase.h"
#include "../icosphere/Bounds.h"
#include <memory>
#include <QtCore/QObject>

namespace icosphere {
    class Icosphere;
}


namespace noise {

    namespace module {

        /// @addtogroup libnoise
        /// @{

        /// @addtogroup modules
        /// @{

        /// @defgroup combinermodules Combiner Modules
        /// @addtogroup combinermodules
        /// @{

        /// Noise module that creates a quantised icosphere map from the source module.
        ///
        /// @image html moduleadd.png
        ///
        /// This noise module requires one source module.
        using namespace noise::module;

        class IcosphereMap : public QObject, public Module {
        Q_OBJECT

        public:

            /// Constructor.
            IcosphereMap (std::shared_ptr<icosphere::Icosphere> icosphere = nullptr);

            virtual int GetSourceModuleCount () const {
                return 1;
            }

            virtual double GetValue (double x, double y, double z) const;
            void buildIcosphere (const unsigned& depth, const icosphere::Bounds& bounds);

        public slots:
           void icosphereBuilt (std::shared_ptr<icosphere::Icosphere>);
            void cancelBuild();

        signals:
            void available (std::shared_ptr<icosphere::Icosphere>);
            void buildCancelled();


        protected:
            std::shared_ptr<icosphere::Icosphere> _icosphere;
            void buildIcosphere ();
        };

        /// @}

        /// @}

        /// @}
    }
}

#endif //CALENHAD_ICOSPHEREMAP_H
