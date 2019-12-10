//
// Created by martin on 17/05/17.
//

#ifndef CALENHAD_PROJECTIONSERVICE_H
#define CALENHAD_PROJECTIONSERVICE_H


#include <QtCore/QMap>

namespace calenhad {
    namespace mapping {
        namespace projection {
            class Projection;
            class ProjectionService {
            public:
                ProjectionService ();

                virtual ~ProjectionService ();

                calenhad::mapping::projection::Projection* fetch (const QString& name);

                const QMap<QString, calenhad::mapping::projection::Projection*>& all () const;
                QString glslInverse ();
                QString glslForward ();
            protected:
                QMap<QString, calenhad::mapping::projection::Projection*> _projections;


            };
        }
    }
}


#endif //CALENHAD_PROJECTIONSERVICE_H
