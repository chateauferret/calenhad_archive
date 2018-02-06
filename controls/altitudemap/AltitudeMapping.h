//
// Created by martin on 05/02/18.
//

#ifndef CALENHAD_ALTITUDEMAPPING_H
#define CALENHAD_ALTITUDEMAPPING_H


#include <QtCore/QPointF>
#include <QtXml/QtXml>

// A wrapper for a QPointF with some functionality to support AltitudeMapPlot and QAltitudeMap
namespace calenhad {
    namespace controls {
        namespace altitudemap {

            class AltitudeMapping : public QPointF {

            public:
                AltitudeMapping (const QPointF& point);
                AltitudeMapping();
                AltitudeMapping (const double& x, const double& y);

                virtual ~AltitudeMapping();

            protected:


            };

        }
    }
}

#endif //CALENHAD_ALTITUDEMAPPING_H
