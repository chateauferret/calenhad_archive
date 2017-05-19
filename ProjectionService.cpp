//
// Created by martin on 17/05/17.
//

#include "ProjectionService.h"

using namespace Marble;

ProjectionService::ProjectionService () {
    _projections.insert ("Spherical (Orthographic)", Projection::Spherical);
    _projections.insert ("Equirectangular (Plate carée)", Projection::Equirectangular);
    _projections.insert ("Mercator", Projection::Mercator);
    _projections.insert ("Gnomonic (looks like shite)", Projection::Gnomonic);
    _projections.insert ("Stereographic", Projection::Stereographic);
    _projections.insert ("Lambert azimuthal", Projection::LambertAzimuthal);
    _projections.insert ("Azimuthal equidistant", Projection::AzimuthalEquidistant);
    _projections.insert ("Vertical perspective", Projection::VerticalPerspective);
}

ProjectionService::~ProjectionService() {

}

Marble::Projection ProjectionService::fetch (const QString& name) {
    return _projections.value (name, Projection::Spherical);
}

const QMap<QString, Marble::Projection>& ProjectionService::all () const {
    return _projections;
}
