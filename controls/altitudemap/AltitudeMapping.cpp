//
// Created by martin on 05/02/18.
//

#include "AltitudeMapping.h"
using namespace calenhad::controls::altitudemap;


AltitudeMapping::AltitudeMapping () : QPointF () {

}

AltitudeMapping::AltitudeMapping (const QPointF& point) : QPointF (point) {

}


AltitudeMapping::AltitudeMapping (const double& x, const double& y) : QPointF (x, y) {

}

AltitudeMapping::~AltitudeMapping () {

}

