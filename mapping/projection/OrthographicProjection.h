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

                ProjectionId id() override;

                void setDatum (const geoutils::Geolocation& datum) override;

                geoutils::Geolocation _datum;   // _datum.longitude is the central meridian of the projection
                // _datum.latitude is ignored
                QString glslInverse () override;

                bool isVisible (const geoutils::Geolocation& geolocation);

                QPointF range() override;

                QString glslForward () override;

                double aspectRatio() override;
            };

        }
    }
}



#endif //CALENHAD_ORTHOGRAPHICROJECTION_H
