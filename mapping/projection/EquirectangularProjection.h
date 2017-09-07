//
// Created by martin on 01/09/17.
//

#ifndef CALENHAD_EQUIRECTANGULARPROJECTION_H
#define CALENHAD_EQUIRECTANGULARPROJECTION_H

#include "Projection.h"

namespace calenhad {
    namespace mapping {
        namespace projection {


            class EquirectangularProjection : public Projection {

            public:
                EquirectangularProjection();

                virtual ~EquirectangularProjection ();

                bool inverse (const QPointF& point, geoutils::Geolocation& loc) override;

                bool forward (const geoutils::Geolocation& loc, QPointF& point) override;

                QString name () override;

                QString notes () override;

                int id() override;

                void setDatum (const geoutils::Geolocation& point);

                geoutils::Geolocation _datum;   // _datum.longitude is the central meridian of the projection
                                                // _datum.latitude is ignored
                QString glsl () override;
            };

        }
    }
}


#endif //CALENHAD_EQUIRECTANGULARPROJECTION_H
