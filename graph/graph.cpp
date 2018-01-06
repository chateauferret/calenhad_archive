//
// Created by martin on 08/08/17.
//

#include <libnoise/module/perlin.h>
#include <CalenhadServices.h>
#include <QMap>
#include <iostream>
#include "../nodeedit/qneport.h"
#include "preferences/preferences.h"
#include "graph.h"
#include "../qmodule/QModule.h"
#include "../nodeedit/QNodeBlock.h"
#include "../nodeedit/qneconnection.h"
#include "../pipeline/ModuleFactory.h"
#include "../mapping/Curve.h"
#include "../mapping/TerraceCurve.h"
#include "../mapping/CubicSpline.h"
#include "../legend/Legend.h"
#include "exprtk/VariablesService.h"
#include <QList>
#include <qmodule/QAltitudeMap.h>
#include <qmodule/QRasterModule.h>
#include "../messages/QNotificationHost.h"
#include <QColor>
#include "../icosphere/Bounds.h"

using namespace calenhad;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::preferences;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace icosphere;
using namespace calenhad::expressions;
using namespace exprtk;
/*
Graph::Graph (const QString& xml, const QString& nodeName) : _xml (xml), _nodeName (nodeName), _colorMapBuffer (nullptr), _parser (new parser<double>()), _rasterId = 0; {
    _doc.setContent (_xml);

}

Graph::Graph (const QDomDocument& doc, const QString& nodeName): _doc (doc), _nodeName (nodeName), _colorMapBuffer (nullptr), _parser (new parser<double>()) {

}
*/

Graph::Graph (calenhad::qmodule::QModule* module) : _module (module), _nodeName (module -> name()), _colorMapBuffer (nullptr), _parser (new parser<double>()), _rasterId (0) {

}


Graph::~Graph () {
    if (_colorMapBuffer) { delete (_colorMapBuffer); }
    delete _parser;
    for (int n : _rasters.keys()) {
        QImage* image = _rasters.value (n);
        _rasters.remove (n);
        if (image) {
            delete image;
        }
    }
}

QString Graph::glsl() {
    _code =  glsl (_module);
    if (_code != QString::null) {
        _code.append ("float value (vec3 cartesian) {\n");
        _code.append ("    return _" + _nodeName + " (cartesian);\n");
        _code.append ("}\n");
        parseLegend ();
    }
    return _code;
};

QString Graph::glsl (QModule* module) {
    if (module -> isComplete()) {
        QString name = module->name ();

        // Compile any antecedent modules recurisvely
        for (QNEPort* port : module->ports ()) {
            if (port->portType () != QNEPort::OutputPort && !port->connections ().empty ()) {
                std::cout << port->portName ().toStdString () << ": " << port->type () << "\n";
                for (QNEConnection* c : port->connections ()) {
                    QNEPort* p = c->otherEnd (port);
                    if (p) {
                        QNode* other = p->owner ();
                        if (other && other != module) {
                            QModule* qm = static_cast<QModule*> (other);
                            if (qm) {
                                _code = glsl (qm);
                            }
                        }
                    }
                }
            }
        }

        QModule* qm = static_cast<QModule*> (module);
        if (qm) {

            // Find out what type of module this node wants and construct glsl for it
            QString type = qm->nodeType ();
            std::cout << type.toStdString () << "\n";

            // if it's an altitude map, compile the decision tree
            if (type == CalenhadServices::preferences ()->calenhad_module_altitudemap) {
                QAltitudeMap* am = static_cast<QAltitudeMap*> (qm);
                QVector<QPointF> entries = am->entries ();

                // input is below the bottom of the range
                _code += "float %n (vec3 v) {\n";
                _code += "  float value = %0 (v);\n";
                _code += "  if (value < " + QString::number (entries.first ().x ()) + ") { return " + QString::number (entries.first ().y ()) + "; }\n";

                for (QPointF point : entries) {
                    // Do we need to sort the entries?
                    QString mapFunction;

                    // compose the mapping function
                    int j = entries.indexOf (point);
                    // spline function
                    if (am->curveFunction () == "spline") {
                        double x[4], y[4];

                        for (int i = 0; i < 4; i++) {
                            int k = std::max (j + i - 2, 0);
                            k = std::min (k, entries.size () - 1);
                            x[i] = entries.at (k).x ();
                            y[i] = entries.at (k).y ();
                        }

                        // Compute the alpha value used for cubic interpolation.
                        mapFunction += "        float alpha = ((value - " + QString::number (x[1]) + ") / " + QString::number (x[2] - x[1]) + ");\n";
                        mapFunction += "        return cubicInterpolate (" +
                                       QString::number (y[0]) + ", " +
                                       QString::number (y[1]) + ", " +
                                       QString::number (y[2]) + ", " +
                                       QString::number (y[3]) + ", alpha);";
                        _code += "  if (value > " + QString::number (x[1]) + " && value <= " + QString::number (x[2]) + ") {\n" + mapFunction + "\n   }\n";
                    }

                    // terrace function
                    if (am->curveFunction () == "terrace") {
                        double x[2], y[2];
                        for (int i = 0; i < 2; i++) {
                            int k = std::max (j + i - 1, 0);
                            k = std::min (k, entries.size () - 1);
                            x[i] = entries.at (k).x ();
                            y[i] = entries.at (k).y ();
                        }

                        // Compute the alpha value used for cubic interpolation.
                        mapFunction += "        float alpha = ((value - " + QString::number (x[0]) + ") / " + QString::number (x[1] - x[0]) + ");\n";
                        if (am->isFunctionInverted ()) { mapFunction += "        alpha = 1 - alpha;\n"; }
                        mapFunction += "        alpha *= alpha;\n";
                        mapFunction += "        return mix ( float(" +
                                       QString::number (am->isFunctionInverted () ? y[1] : y[0]) + "), float (" +
                                       QString::number (am->isFunctionInverted () ? y[0] : y[1]) + "), " +
                                       "alpha);";
                        _code += "  if (value > " + QString::number (x[0]) + " && value <= " + QString::number (x[1]) + ") {\n" + mapFunction + "\n   }\n";
                    }
                }

                // input is beyond the top of the range
                _code += "  if (value > " + QString::number (entries.last ().x ()) + ") { return " + QString::number (entries.last ().y ()) + "; }\n";
                _code += "}\n";

            } else {
                _code.append (CalenhadServices::modules ()->codes ()->value (type));
            }

            // if it's a raster module, compile and upload the raster content to the raster buffer
            if (type == CalenhadServices::preferences ()->calenhad_module_raster) {
                QRasterModule* rm = (QRasterModule*) qm;
                Bounds bounds = rm->bounds ();
                QImage* image = rm->raster ();
                QString boundsCode;

                _rasters.insert (_rasterId++, image);

                // replace the bounds marker with the module's declared bounds
                boundsCode.append ("vec2 (" + QString::number (bounds.west ()) + ", " + QString::number (bounds.north ()) + ")");
                boundsCode.append (", ");
                boundsCode.append ("vec2 (" + QString::number (bounds.east ()) + ", " + QString::number (bounds.south ()) + ")");
                _code.replace ("%bounds", boundsCode);
            }


            // replace the name marker with the name of the module which will be the member variable name for its output in glsl
            _code.replace ("%n", "_" + name);

            // replace the input module markers with their names referencing their member variables in glsl
            int i = 0;
            for (QNEPort* port : qm->ports ()) {
                if (port->portType () != QNEPort::OutputPort) {
                    QString index = QString::number (i);
                    QNode* other = port->connections ()[0]->otherEnd (port)->owner ();
                    QString source = other->name ();
                    _code.replace ("%" + index, "_" + source);
                }
            }
            // fill in attribute values by looking for words beginning with % and replacing them with the parameter values from the XML
            for (QString param : CalenhadServices::modules ()->paramNames ()) {
                if (qm->parameters ().contains (param)) {
                    _code.replace ("%" + param, QString::number (qm -> parameterValue (param)));
                }
            }

            // populate index number for a raster
            QRasterModule* rm = dynamic_cast<QRasterModule*> (module);
            if (rm) {
                _code.replace ("%index", QString::number (_rasterId - 1));
            }
        }
        return _code;
    } else {
        return QString::null;
    }
}

float* Graph::colorMapBuffer () {
    return _colorMapBuffer;
}

// returns the number of bytes required to store the colour map buffer
int Graph::colorMapBufferSize() {
    return CalenhadServices::preferences() -> calenhad_colormap_buffersize * sizeof (float) * 4;
}

void Graph::parseLegend () {
    int size = CalenhadServices::preferences () -> calenhad_colormap_buffersize;
    if (! _colorMapBuffer) {
        _colorMapBuffer = new float [size * 4];
    }

    QString legendName = _module -> legend() -> name();
    QList<LegendEntry> legendElements = _module -> legend() -> entries();

    float dx = (1 / (float) size) * 2 ;
    for (int i = 0; i < size * 4; i+= 4)  {
        QColor c = _module -> legend() -> lookup (i * dx - 1);
        _colorMapBuffer [i + 0] = (float) c.redF();
        _colorMapBuffer [i + 1] = (float) c.greenF();
        _colorMapBuffer [i + 2] = (float) c.blueF();
        _colorMapBuffer [i + 3] = (float) c.alphaF();
    }
}

int Graph::rasterCount() {
    return _rasterId;
}

QImage* Graph::raster (const int& index) {
    return _rasters.value (index);
}
