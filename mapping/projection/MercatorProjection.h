//
// Created by martin on 04/09/17.
//

#ifndef CALENHAD_MERCATORPROJECTION_H
#define CALENHAD_MERCATORPROJECTION_H


#include "Projection.h"

namespace calenhad {
    namespace mapping {
        namespace projection {


            class MercatorProjection : public Projection {

            public:
                MercatorProjection();

                virtual ~MercatorProjection ();

                bool inverse (const QPointF& point, geoutils::Geolocation& loc) override;

                bool forward (const geoutils::Geolocation& loc, QPointF& point) override;

                QString name () override;

                QString notes () override;

                int id() override;

                void setDatum (const geoutils::Geolocation& datum) override;

                geoutils::Geolocation _datum;   // _datum.longitude is the central meridian of the projection
                                                // _datum.latitude is ignored
                QString glslInverse() override;

                QString glslForward() override;

                double aspectRatio() override;
            };

        }
    }
}



#endif //CALENHAD_MERCATORPROJECTION_H
