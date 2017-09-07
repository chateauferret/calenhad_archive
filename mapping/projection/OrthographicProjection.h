//
// Created by martin on 04/09/17.
//

#ifndef CALENHAD_ORTHOGRAPHICPROJECTION_H
#define CALENHAD_ORTHOGRAPHICPROJECTION_H


#include "Projection.h"

namespace calenhad {
    namespace mapping {
        namespace projection {


            class OrthographicProjection : public Projection {

            public:
                OrthographicProjection();

                virtual ~OrthographicProjection ();

                bool inverse (const QPointF& point, geoutils::Geolocation& loc) override;

                bool forward (const geoutils::Geolocation& loc, QPointF& point) override;

                QString name () override;

                QString notes () override;

                int id() override;

                void setDatum (const geoutils::Geolocation& point);

                geoutils::Geolocation _datum;   // _datum.longitude is the central meridian of the projection
                // _datum.latitude is ignored
                QString glsl () override;

                bool isVisible (const geoutils::Geolocation& geolocation);

                QPointF range() override;
            };

        }
    }
}



#endif //CALENHAD_ORTHOGRAPHICROJECTION_H
