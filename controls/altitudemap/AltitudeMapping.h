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

            class AltitudeMapping {

            public:
                AltitudeMapping();
                AltitudeMapping (const QPointF& point);
                AltitudeMapping (const double& x, const double& y);
                AltitudeMapping (const QString& x, const QString& y);

                virtual ~AltitudeMapping();

                void setPoint (const double& x, const double& y);
                void setPoint (const QString& x, const QString& y);
                void setPoint (const QPointF& point);
                QPointF point()const;
                double x()const;
                double y()const;
                QString expressionX()const;
                QString expressionY()const;
                void setExpressionX (const QString& x);
                void setExpressionY (const QString& y);
                bool isComputed() const;
                bool isValid() const;
            protected:
                QString _x, _y;

            };

        }
    }
}

#endif //CALENHAD_ALTITUDEMAPPING_H
