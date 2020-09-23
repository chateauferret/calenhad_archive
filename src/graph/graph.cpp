//
// Created by martin on 08/08/17.
//

#include <src/CalenhadServices.h>
#include <iostream>
#include "nodeedit/Port.h"
#include "preferences/preferences.h"
#include "graph.h"
#include "module/Module.h"
#include "nodeedit/NodeBlock.h"
#include "nodeedit/Connection.h"
#include "../pipeline/ModuleFactory.h"
#include "../mapping/Curve.h"
#include "exprtk/Calculator.h"
#include <QList>
#include <module/AltitudeMap.h>
#include <module/StructuredGrid.h>
#include <src/module/RasterModule.h>
#include <src/module/Cache.h>
#include "../messages/QNotificationHost.h"
#include "../legend//Legend.h"

using namespace calenhad;
using namespace calenhad::nodeedit;
using namespace calenhad::module;
using namespace calenhad::preferences;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace grid;
using namespace calenhad::expressions;
using namespace calenhad::controls::altitudemap;
using namespace exprtk;
/*
Graph::Graph (const QString& xml, const QString& nodeName) : _xml (xml), _nodeName (nodeName), _colorMapBuffer (nullptr), _parser (new parser<double>()), _rasterId = 0; {
    _doc.setContent (_xml);

}

Graph::Graph (const QDomDocument& doc, const QString& nodeName): _doc (doc), _nodeName (nodeName), _colorMapBuffer (nullptr), _parser (new parser<double>()) {

}
*/

Graph::Graph (calenhad::module::Module* module) :
    _model (module -> model()),
    _module (module),
    _nodeName (module -> name()),
    _colorMapBuffer (nullptr),
    _parser (new parser<double>()),
    _index (0) {

}


Graph::~Graph () {
    delete (_colorMapBuffer);
    delete _parser;

}

QString Graph::glsl() {
    _code =  glsl (_module);

    if (_code != QString::null) {
        _code.append ("\n\nfloat value (ivec3 pos, vec3 cartesian, vec2 geolocation) {\n");
        _code.append ("    return _" + _nodeName + " (pos, cartesian, geolocation);\n");
        _code.append ("}\n");
    }
    //std::cout << _code.toStdString () << "\n\n";
    return _code;
}

QString Graph::glsl (Module* module) {
    _index = 0;
    _rasters.clear();
    if (module -> isComplete()) {
        QString name = module -> name ();
        if (! _code.contains ("float _" + name)) {
            // Compile any antecedent modules recurisvely,
            // unless this one is a cache, in which case don't bother your arse
            StructuredGrid* cm = dynamic_cast<StructuredGrid*> (module);
            if (! cm) {
                for (unsigned p: module -> inputs().keys()) {
                    Port* port = module -> inputs().value (p);
                    if (!port -> connections().empty()) {
                        for (Connection* c : port -> connections()) {
                            Port* source = c -> otherEnd (port);
                            if (source) {
                                Node* other = source -> owner();
                                if (other && other != module) {
                                    Module* qm = dynamic_cast<Module*> (other);
                                    _code = glsl (qm);
                                }
                            }
                        }
                    }
                }
            }

            Module* qm = dynamic_cast<Module*> (module);
            if (qm) {

                // Find out what type of module this node wants and construct glsl for it
                QString type = qm->nodeType ();

                // if it's an altitude map, compile the decision tree
                QString func = qm -> glsl();
                _code.append ("float _" + name + " (ivec3 pos, vec3 c, vec2 g) { \n");
                if (! func.contains ("return")) {
                    _code.append ("    return ");
                }
                _code += func;
                    //_code.append ("; }\n");

                // if it's a raster or cache module, compile and queue the module for raster upload
                if (type == CalenhadServices::preferences() -> calenhad_module_raster) {
                    RasterModule* rm = (RasterModule*) qm;
                    _rasters.append (rm);
                    _code.replace ("%gridIndex", QString::number (_index));
                    _index += CalenhadServices::preferences() -> calenhad_compute_gridsize;
                    _code.append ("; }\n");
                }

                Cache* cache = dynamic_cast<Cache*> (qm);
                if (cache) {
                    _rasters.append (qm);
                    _code.replace ("%gridIndex", QString::number (_index));
                    _index += CalenhadServices::preferences() -> calenhad_compute_gridsize;
                    _code.append ("; }\n");
                }
            }
        }
        return _code;
    } else {
        return QString::null;
    }
}

int Graph::rasterCount() const {
    return _rasters.size();
}

void Graph::cube (const int& index, CubicSphere* cube) const {
    std::cout << "Extracting " << _rasters [index] -> name().toStdString() << "\n"
;    _rasters [index] -> fetch (cube);
}

Module* Graph::module () const {
    return _module;
}



