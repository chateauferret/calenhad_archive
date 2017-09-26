//
// Created by martin on 01/09/17.
//

#ifndef CALENHAD_PROJECTION_H
#define CALENHAD_PROJECTION_H

#include <geoutils.h>
#include <QtCore/QPointF>
#include <QPointF>
#include "geoutils.h"

namespace calenhad {
    namespace mapping {
        namespace projection {
            class Projection {
            public:
                Projection() {};

                virtual ~Projection () {};

                virtual bool inverse (const QPointF& point, geoutils::Geolocation& loc) = 0;

                virtual bool forward (const geoutils::Geolocation& loc, QPointF& point) = 0;

                virtual QString name () = 0;

                virtual QString notes () = 0;

                virtual QString glslInverse () = 0;
                virtual QString glslForward() = 0;

                virtual int id() = 0;

                virtual QPointF range() { return QPointF (M_PI * 2, M_PI); }

                virtual void setDatum (const geoutils::Geolocation& datum) = 0;

                virtual double aspectRatio() = 0;
            };
        }
    }
}

#endif //CALENHAD_PROJECTION_H
