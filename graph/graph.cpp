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
#include "../qmodule/QNode.h"
#include "../nodeedit/qneconnection.h"
#include "../pipeline/ModuleFactory.h"
#include "../mapping/Curve.h"
#include "../mapping/TerraceCurve.h"
#include "../mapping/CubicSpline.h"
#include "../legend/Legend.h"
#include "../exprtk/CalculatorService.h"
#include <QList>
using namespace calenhad;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::preferences;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace calenhad::expressions;
using namespace exprtk;

Graph::Graph (const QString& xml, const QString& nodeName) : _xml (xml), _nodeName (nodeName), _colorMapBuffer (nullptr), _parser (new parser<double>()) {
    _doc.setContent (_xml);

}

Graph::Graph (const QDomDocument& doc, const QString& nodeName): _doc (doc), _nodeName (nodeName), _colorMapBuffer (nullptr), _parser (new parser<double>()) {

}

Graph::~Graph () {
    if (_colorMapBuffer) { delete (_colorMapBuffer); }
    delete _parser;
}


QString Graph::readParameter (const QDomElement& element, const QString param) {
    QDomElement e = element.firstChildElement ("parameter");
    for (; ! e.isNull(); e = e.nextSiblingElement ("parameter")) {
        if (e.attributeNode ("name").value () == param) {
            QString expr = e.attributeNode ("value").value ();
            expression<double>* exp = CalenhadServices::calculator() -> makeExpression (expr);
            if (exp) {
                double value = exp -> value ();
                delete exp;
                return QString::number (value);
            } else {
                std::cout << "Expression goosed\n";
                QStringList errors = CalenhadServices::calculator () -> errors();
                for (QString error : errors) {
                    std::cout << error.toStdString () << "\n";
                    delete exp;
                    return 0;
                }
            }
        }
    }
    return QString::null;
}

QString Graph::glsl () {
    list = _doc.documentElement().elementsByTagName ("module");
    for (int i = 0; i < list.length (); i++) {
        QDomElement nameNode = list.at (i).firstChildElement ("name");
        QString name = nameNode.text();
        if (name == _nodeName) {
            _node = list.at (i);
            break;
        }
    }

    _connections = _doc.documentElement().elementsByTagName ("connection");
    _code =  glsl (_node);
    _code.append ("float value (vec3 cartesian) {\n");
    _code.append ("    return _" + _nodeName + " (cartesian);\n");
    _code.append ("}\n");
    parseLegend();
    return _code;
};

QString Graph::glsl (const QDomNode& node) {

    // Find any modules with connections to this module and recursively parse them
    QDomElement nameNode = node.firstChildElement ("name");
    QString name = nameNode.text();
    for (int i = 0; i < _connections.length(); i++) {
        QDomNode c = _connections.at (i);
        QDomElement toElement = c.firstChildElement ("target");
        if (toElement.attributes().namedItem ("module").nodeValue() == name) {
            QString from = c.firstChildElement ("source").attributes().namedItem ("module").nodeValue();
            for (int i = 0; i < list.length(); i++) {
                if (list.at (i).firstChildElement ("name").text() == from) {
                    _code = glsl (list.at (i));

                    // remove them from the process once done because the glsl won't compile if they appear twice
                    _doc.documentElement ().removeChild (list.at (i));
                }
            }
        }
    }

    // Find out what type of module this node wants and construct glsl for it
    QString type = node.attributes().namedItem ("type").nodeValue();
    QDomElement element = node.toElement();
    std::cout << type.toStdString () << "\n";
    if (type == CalenhadServices::preferences() -> calenhad_module_altitudemap) {

        // this version uses branching directly on the GPU - seems to be OK
        // if we try doing it with precalculated buffers managing the buffers is a pain in the arse.
        QDomElement mapElement = element.firstChildElement ("map");
        QDomNodeList entries = mapElement.elementsByTagName ("entry");
        QList<QPair<double, double>> mapping;
        for (int i = 0; i < entries.length(); i++) {
            QDomElement n = entries.at (i).toElement ();
            bool ok;
            double x = n.attribute ("x").toDouble (&ok);
            double y = n.attribute ("y").toDouble (&ok);
            if (ok) {
                mapping.append (QPair<double, double> (x, y));
            }
        }
        // input is below the bottom of the range
        _code += "float %n (vec3 v) {\n";
        _code += "  float value = %0 (v);\n";
        _code += "  if (value < " + QString::number (mapping.first().first) + ") { return " + QString::number (mapping.last().second) + "; }\n";


        for (int j = 1; j < mapping.size(); j++) {
            QString mapFunction;

            // compose the mapping function

            // spline function
            if (mapElement.attribute ("function") == "spline") {
                double x [4], y [4];
                for (int i = 0; i < 4; i++) {
                    int k = std::max (j + i - 2, 0);
                    k = std::min (k, mapping.size() - 1);
                    x [i] = mapping.at (k).first;
                    y [i] = mapping.at (k).second;
                }

                // Compute the alpha value used for cubic interpolation.
                mapFunction += "        float alpha = ((value - " + QString::number (x [1]) + ") / " + QString::number (x [2] - x [1]) + ");\n";
                mapFunction += "        return cubicInterpolate (" +
                        QString::number (y [0]) + ", " +
                        QString::number (y [1]) + ", " +
                        QString::number (y [2]) + ", " +
                        QString::number (y [3]) + ", alpha);";
                _code += "  if (value > " + QString::number (x [1]) + " && value <= " + QString::number (x [2]) + ") {\n" + mapFunction + "\n   }\n";
            }

            // terrace function
            bool inverted = mapElement.attribute ("inverted") == "1" || mapElement.attribute ("inverted").toLower() == "true"  ;
            if (mapElement.attribute ("function") == "terrace") {
                double x [2], y [2];
                for (int i = 0; i < 2; i++) {
                    int k = std::max (j + i - 1, 0);
                    k = std::min (k, mapping.size() - 1);
                    x [i] = mapping.at (k).first;
                    y [i] = mapping.at (k).second;
                }

                // Compute the alpha value used for cubic interpolation.
                mapFunction += "        float alpha = ((value - " + QString::number (x [0]) + ") / " + QString::number (x [1] - x [0]) + ");\n";
                if (inverted) { mapFunction += "        alpha = 1 - alpha;\n"; }
                mapFunction += "        alpha *= alpha;\n";
                mapFunction += "        return mix ( float(" +
                               QString::number (inverted ? y [1] : y [0]) + "), float (" +
                               QString::number (inverted ? y [0] : y [1]) + "), " +
                               "alpha);";
                _code += "  if (value > " + QString::number (x [0]) + " && value <= " + QString::number (x [1]) + ") {\n" + mapFunction + "\n   }\n";
            }
        }

        // input is beyond the top of the range
        _code += "  if (value > " + QString::number (mapping.last().first) + ") { return " + QString::number (mapping.last().second) + "; }\n";
        _code += "}\n";

    } else {
        _code.append (CalenhadServices::modules() -> codes() -> value (type));
    }
    // replace the name marker with the name of the module which will be the member variable name for its output in glsl
    _code.replace ("%n", "_" + name);

    // replace the input module markers with their names referencing their member variables in glsl
    for (int i = 0; i < _connections.length(); i++) {
        QDomNode c = _connections.at (i);
        QDomElement toElement = c.firstChildElement ("target");

        if (toElement.attributes ().namedItem ("module").nodeValue () == name) {
            QDomElement fromElement = c.firstChildElement ("source");
            QString index = toElement.attributes ().namedItem ("input").nodeValue ();
            QString source = fromElement.attributes ().namedItem ("module").nodeValue ();
            _code.replace ("%" + index, "_" + source);
        }
    }
    // fill in attribute values by looking for words beginning with % and replacing them with the parameter values from the XML
    for (QString param : CalenhadServices::modules () -> paramNames()) {
        _code.replace ("%" + param, readParameter (element, param));
    }

    return _code;
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

    QDomElement legendElement;
    QString legendName = _node.attributes ().namedItem ("legend").nodeValue();
    QDomElement legendRoot = _doc.documentElement ().firstChildElement ("legends");
    QDomNodeList legendElements = legendRoot.elementsByTagName ("legend");
    for (int i = 0; i < legendElements.size(); i++) {
        if (legendElements.at (i).firstChildElement ("name").firstChild ().nodeValue() == legendName) {
            legendElement = legendElements.at (i).toElement();
        }
    }

    Legend* legend = new Legend();
    legend -> inflate (legendElement);
    std::cout << legendElement . toDocument () . toString () . toStdString () << "\n";
    float dx = (1 / (float) size) * 2 ;
    for (int i = 0; i < size * 4; i+= 4)  {
        QColor c = legend -> lookup (i * dx - 1);
        _colorMapBuffer [i + 0] = (float) c.redF();
        _colorMapBuffer [i + 1] = (float) c.greenF();
        _colorMapBuffer [i + 2] = (float) c.blueF();
        _colorMapBuffer [i + 3] = (float) c.alphaF();
    }
}


