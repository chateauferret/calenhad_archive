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

using namespace calenhad;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::preferences;
using namespace calenhad::graph;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace calenhad::expressions;
using namespace exprtk;

Graph::Graph (const QString& xml, const QString& nodeName) : _xml (xml), _nodeName (nodeName), _altitudeMapBuffer (nullptr), _colorMapBuffer (nullptr), _parser (new parser<double>()) {
    _doc.setContent (_xml);

}

Graph::Graph (const QDomDocument& doc, const QString& nodeName): _doc (doc), _nodeName (nodeName), _altitudeMapBuffer (nullptr), _colorMapBuffer (nullptr), _parser (new parser<double>()) {

}

Graph::~Graph () {
    if (_altitudeMapBuffer) { free (_altitudeMapBuffer); }
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
                std::cout << value << "\n";
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
    std::cout << _doc.toString ().toStdString () << "\n";
    if (_altitudeMapBuffer) {
        free (_altitudeMapBuffer);
        _altitudeMapBuffer = nullptr;
    }
    _bufferCount = 0;

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

        // to avoid lots of branching in complex altitude maps we will generate a list of precalculated mappings and
        // linearly interpolate between them. Then the index of the nearest point to a given input value is a simple function of the value.
        // This buffer is the list of precalculated mappings

        QString call = addAltitudeMapBuffer (element.firstChildElement ("map"));
        _code.append (call);
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

QString Graph::addAltitudeMapBuffer (QDomElement map) {
    Curve* c;
    QString function = map.attribute ("function");
    QDomNodeList mapNodes = map.elementsByTagName ("entry");
    if (function == "spline") {
        CubicSpline* cs = new CubicSpline();
        c = cs;
        for (int i = 0; i < mapNodes.size(); i++) {
            bool ok;
            float x = mapNodes.at (i).attributes().namedItem ("x").nodeValue ().toFloat (&ok);
            float y = mapNodes.at (i).attributes().namedItem ("y").nodeValue().toFloat (&ok);
           if (ok) {
                cs -> AddControlPoint (x, y);
            }
        }

    } else {
        TerraceCurve* tc = new TerraceCurve();
        c = tc;
        bool inverted = map.attribute ("inverted").toLower () == "true";
        tc->InvertTerraces (inverted);
        for (int i = 0; i < mapNodes.size(); i++) {
            bool ok;
            float x = mapNodes.at (i).attributes().namedItem ("x").nodeValue ().toFloat (&ok);
            if (ok) {
                tc->AddControlPoint (x);
            }
        }
    }

    int size = CalenhadServices::preferences () -> calenhad_altitudemap_buffersize;
    float output [size];

    if (_altitudeMapBuffer) {
        _bufferCount++;
        _altitudeMapBuffer = (float*) realloc (_altitudeMapBuffer, size * sizeof (float) * _bufferCount);
    } else {
        _altitudeMapBuffer = (float*) malloc (size * sizeof (float));
        _bufferCount = 1;
    }
    c -> GetValues (output, size);
    for (int i = 0; i < size; i++) {
        float x = size * (i / size);
        _altitudeMapBuffer [i] = output [i];
    }
    return "float %n (vec3 v) { return map (%0 (v), " + QString::number (_bufferCount - 1) + "); }\n";
}

float* Graph::altitudeMapBuffer () {
    return _altitudeMapBuffer;
}

// returns the number of bytes required to store the altitude map buffer
int Graph::altitudeMapBufferSize () {
    return CalenhadServices::preferences() -> calenhad_altitudemap_buffersize * _bufferCount * sizeof (float);
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
    float dx = (1 / (float) size) * 2 ;
    for (int i = 0; i < size * 4; i+= 4)  {
        QColor c = legend -> lookup (i * dx - 1);
        _colorMapBuffer [i + 0] = (float) c.redF();
        _colorMapBuffer [i + 1] = (float) c.greenF();
        _colorMapBuffer [i + 2] = (float) c.blueF();
        _colorMapBuffer [i + 3] = (float) c.alphaF();
    }

}


