//
// Created by martin on 06/09/17.
//

#ifndef CALENHAD_VIEWPORT_H
#define CALENHAD_VIEWPORT_H

#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QObject>
#include "src/geoutils.h"
#include "../icosphere/Bounds.h"

namespace calenhad {
    namespace mapping {
        namespace projection {
            class Projection;
        }

        class Viewport : public QObject {
        Q_OBJECT
        public:
            Viewport ();
            virtual ~Viewport ();
            void setProjection (const QString& projection);
            void setDatum (const geoutils::Geolocation& datum);
            void setZoom (const double& zoom);
            QPointF flatMapCoordinates (const QPoint& screenCoordinates);
            icosphere::Bounds bounds();
            bool screenCoordinates (geoutils::Geolocation geolocation, QPointF& screenCoordinates);
            bool geoCoordinates (const QPoint& pos, geoutils::Geolocation& geolocation);
            void setSize (const QSize& size);
            QSize size();

            projection::Projection* projection ();

            geoutils::Geolocation datum ();

            double zoom ();

        signals:
            void changed();

        protected:
            double _zoom;
            geoutils::Geolocation _datum;
            projection::Projection* _projection;
            QSize _size;


        };
    }
}

#endif //CALENHAD_VIEWPORT_H
