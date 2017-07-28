//
// Created by martin on 17/05/17.
//

#ifndef CALENHAD_PROJECTIONSERVICE_H
#define CALENHAD_PROJECTIONSERVICE_H


#include <marble/MarbleGlobal.h>
#include <QtCore/QMap>

namespace calenhad {
    namespace controls {
        namespace globe {

            class ProjectionService {
            public:
                ProjectionService ();

                virtual ~ProjectionService ();

                Marble::Projection fetch (const QString& name);

                const QMap<QString, Marble::Projection>& all () const;

            protected:
                QMap<QString, Marble::Projection> _projections;

            };
        }
    }
}


#endif //CALENHAD_PROJECTIONSERVICE_H
