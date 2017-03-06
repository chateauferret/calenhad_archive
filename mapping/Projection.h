//
// Created by martin on 12/02/17.
//

#ifndef CALENHAD_PROJECTION_H
#define CALENHAD_PROJECTION_H

#include <QtCore/QString>
#include "../geoutils.h"
#include <proj_api.h>
#include <QtCore/QPoint>

class Projection {
public:
    Projection (const QString& proj = "+proj=merc +ellps=sphere +lat_ts=33");
    virtual ~Projection();
    geoutils::Geolocation forward (double x, double y);
    std::pair<double, double> inverse (const geoutils::Geolocation& latlon);

protected:
    projPJ pj_out, pj_latlong;
    double* _x, * _y;
};


#endif //CALENHAD_PROJECTION_H
