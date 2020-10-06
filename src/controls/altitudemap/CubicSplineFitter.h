//
// Created by martin on 03/04/17.
//

#ifndef CALENHAD_CUBICSPLINEFITTER_H
#define CALENHAD_CUBICSPLINEFITTER_H


#include <qwt/qwt_curve_fitter.h>
#include <QtGui/QPainterPath>


namespace calenhad {
    namespace controls {
        namespace altitudemap {

            class CubicSplineFitter : public QwtSplineCurveFitter {
            public:
                CubicSplineFitter ();

                virtual ~CubicSplineFitter ();

                QPolygonF fitCurve (const QPolygonF& points) const override;

            protected:
                double getY (const double& x, const QPolygonF& points) const;
            };
        }
    }
}


#endif //CALENHAD_CUBICSPLINEFITTER_H
