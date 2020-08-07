//
// Created by martin on 17/05/17.
//

#include "ProjectionService.h"
#include "EquirectangularProjection.h"
#include "OrthographicProjection.h"
#include "MercatorProjection.h"

using namespace calenhad::mapping::projection;

ProjectionService::ProjectionService () {
    _projections.insert ("Orthographic", new OrthographicProjection());
    _projections.insert ("Equirectangular", new EquirectangularProjection());
    _projections.insert ("Mercator", new MercatorProjection());
    //_projections.insert ("Gnomonic (looks like shite)", Projection::Gnomonic);
    //_projections.insert ("Stereographic", Projection::Stereographic);
    //_proojections.insert ("Lambert azimuthal", Projection::LambertAzimuthal);
    //_projections.insert ("Azimuthal equidistant", Projection::AzimuthalEquidistant);
    //_projections.insert ("Vertical perspective", Projection::VerticalPerspective);
}

ProjectionService::~ProjectionService() {
    for (Projection* p : _projections.values ()) {
        delete p;
    }
}

Projection* ProjectionService::fetch (const QString& name) {
    Projection* p = _projections.value (name, _projections.value ("Equirectangular"));
    return p;
}

const QMap<QString, Projection*>& ProjectionService::all () const {
    return _projections;
}

QString ProjectionService::glslInverse() {
    QString code = "";
    for (Projection* p : _projections) {
        code += p -> glslInverse();
    }
    return code;
}

QString ProjectionService::glslForward() {
    QString code = "";
    for (Projection* p : _projections) {
        code += p -> glslForward();
    }
    return code;
}
