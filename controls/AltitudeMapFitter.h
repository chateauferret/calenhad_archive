//
// Created by martin on 03/04/17.
//

#ifndef CALENHAD_ALTITUDEMAPFITTER_H
#define CALENHAD_ALTITUDEMAPFITTER_H


#include <qwt/qwt_curve_fitter.h>
#include <QtGui/QPainterPath>

class AltitudeMapFitter : public QwtSplineCurveFitter {
public:
    AltitudeMapFitter();
    virtual ~AltitudeMapFitter();
    QPolygonF fitCurve (const QPolygonF& points) const override;
protected:
    double getY (const double& x, const QPolygonF& points) const;
};


#endif //CALENHAD_ALTITUDEMAPFITTER_H
