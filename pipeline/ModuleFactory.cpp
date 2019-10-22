//
// Created by martin on 11/11/16.
//

#include <iostream>
#include "ModuleFactory.h"
#include "module/NodeGroup.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "module/AltitudeMap.h"
#include "../CalenhadServices.h"
#include <QList>
#include <module/RasterModule.h>
#include <nodeedit/Port.h>
#include "../noiseconstants.h"

using namespace calenhad;
using namespace calenhad::pipeline;
using namespace calenhad::module;
using namespace calenhad::expressions;
using namespace calenhad::nodeedit;

ModuleFactory::ModuleFactory() {
    initialise ();
}


ModuleFactory:: ~ModuleFactory() {
    for (QPixmap* icon : _icons) { delete icon; }
}

QPixmap* ModuleFactory::getIcon (const QString& type) {
   return _icons.value (type);
}

void ModuleFactory::initialise() {
    QDomDocument doc;
    if (CalenhadServices::readXml (CalenhadServices::preferences() -> calenhad_moduletypes_filename, doc)) {
        std::cout << "Modules definition at " << CalenhadServices::preferences() -> calenhad_moduletypes_filename.toStdString() << "\n";
        QDomElement paletteElement = doc.documentElement();
        QDomNodeList nodes = paletteElement.elementsByTagName ("module");
        for (int i = 0; i < nodes.size (); i++) {
            QDomElement element = nodes.at (i).toElement ();
            QString type = element.attribute ("name");
            _types.insert (type, element);

            // Create code snippet to be inserted into the compute shader to realise each module's function.
            // %n - will be replaced with the module's name which will serve as the method name representing it in the generated shader code.
            // %0, %1, etc - will be replaced with calls to the module outputs serving the corresponding port as input.
            // %frequency, %lacunarity etc - will be replaced with the value of that parameter, for parameters named in calenhad::graph::Graph::_params.

            QDomElement glslElement = element.firstChildElement ("glsl");
            QString glsl = glslElement.text();
            _moduleCodes.insert (type, glsl);
            std::cout << glsl.toStdString () << "\n";
        }

        // Create types metadata
        QStringList list = _types.keys ();
        for (QString key : list) {
            QDomElement element = _types.value (key);
            QString label = element.attribute ("label");
            _moduleLabels.insert (key, label);
            QDomElement descriptionElement = element.firstChildElement ("documentation");
            QString description = descriptionElement.text ();
            _moduleDescriptions.insert (key, description);
            QString icon = key.toLower ();
            icon.replace (" ", "");
            QString iconFile = CalenhadServices::preferences() -> calenhad_moduletypes_icons_path + icon + ".png";
            QPixmap* pixmap = new QPixmap (iconFile);
            _icons.insert (key, pixmap);
        }

        _moduleLabels.insert ("altitudemap", "Altitude map");
        _moduleLabels.insert ("nodegroup", "Node group");
        _moduleLabels.insert ("raster", "Raster");

        _moduleDescriptions.insert ("altitudemap", "Altitude map");
        _moduleDescriptions.insert ("nodegroup", "Node group");
        _moduleDescriptions.insert ("raster", "Raster");

        _icons.insert ("altitudemap", new QPixmap (CalenhadServices::preferences() -> calenhad_moduletypes_icons_path + "altitudemap.png"));
        _icons.insert ("nodegroup", new QPixmap (CalenhadServices::preferences() -> calenhad_moduletypes_icons_path + "nodegroup.png"));
        _icons.insert ("raster", new QPixmap (CalenhadServices::preferences() -> calenhad_moduletypes_icons_path + "raster.png"));
    } else {
        std::cout << "Couldn't read file " << CalenhadServices::preferences() -> calenhad_moduletypes_filename.toStdString() << "\n";
    }
}

QDomElement ModuleFactory::xml (const QString& type) {
    return _types.value (type);
}

Node* ModuleFactory::inflateModule (const QString& type, CalenhadModel* model) {
    if (_types.keys().contains (type)) {
        bool ok;
        QDomElement element = _types.value (type);

        bool suppressRender = element.hasAttribute ("render") && element.attribute ("render").toLower() == "false";

        // special types which need more than generic construction code
        Node* n = nullptr;
        Module* qm = nullptr;
        if (type == "altitudemap") { AltitudeMap* am = new AltitudeMap(); qm = am; n = qm; }
        if (type == "raster") { RasterModule* rm = new RasterModule(); qm = rm; n = qm; }

        if (! n) {
            qm = new Module (type, nullptr);
            n = qm;
        }

       double height = 1.0, width = 1.0;
        if (element.hasAttribute ("height")) {
            double h = element.attribute ("height").toDouble (&ok);
            height = ok ? h : 1.0;
        }

        if (element.hasAttribute ("width")) {
            double w = element.attribute ("width").toDouble (&ok);
            width = ok ? w : 1.0;
        }

        QSizeF scale (width, height);
        _moduleScales.insert (type, scale);

        n -> setModel (model);

        if (qm) {
            QDomElement portsElement = element.firstChildElement ("ports");
            QDomNodeList portNodesList = portsElement.elementsByTagName ("input");
            for (int i = 0; i < portNodesList.size (); i++) {
                QDomElement portNode = portNodesList.at (i).toElement ();
                QString portType = portNode.attribute ("type");
                int pt = portType == "value" ? Port::InputPort : Port::ControlPort;
                QString portName = portNode.attribute ("name");
                QString label = portNode.attribute ("label");
                int index = portNode.attribute ("index").toInt (&ok);
                if (ok) {
                    bool required = portNode.hasAttribute ("required") ? portNode.attribute ("required") == "true" : false;
                    if (portNode.hasAttribute ("default")) {
                        double defaultValue = portNode.attribute ("default").toDouble (&ok);
                        if (! ok) {
                            defaultValue = 0.0;
                        }

                        qm -> addInputPort (index, pt, portName, label, defaultValue, required);
                    } else {
                        qm -> addInputPort (index, pt, portName, label, 0.0, required);
                    }
                }
            }

            QDomElement paramsElement = element.firstChildElement ("parameters");
            QDomNodeList paramNodesList = paramsElement.elementsByTagName ("parameter");
            for (int i = 0; i < paramNodesList.size (); i++) {
                QDomElement paramElement = paramNodesList.at (i).toElement ();
                QString paramName = paramElement.attribute ("name");
                if (!_paramNames.contains (paramName)) {
                    _paramNames << paramName;
                }
                QString paramType = paramElement.attribute ("type");
                QString paramLabel = paramElement.hasAttribute ("label") ? paramElement.attribute ("label") : paramName;

                if (paramType == "boolean") {
                    bool initial = false;
                    if (paramElement.hasAttribute ("default")) {
                        initial = paramElement.attribute ("default") == "true";
                    }
                    qm->addParameter (paramLabel, paramName, initial);
                } else {
                    bool ok;
                    double initial = paramElement.attribute ("default").toDouble (&ok);
                    if (!ok) { initial = 0; }
                    QString validatorType = paramElement.firstChildElement ("validator").text ();
                    ParamValidator* v;
                    if (validatorType == "AcceptRange") {
                        bool ok;
                        double min = paramElement.attribute ("minimum").toDouble (&ok);
                        if (!ok) { min = -1.0; }
                        double max = paramElement.attribute ("maximum").toDouble (&ok);
                        if (!ok) { max = std::min (1.0, min); }
                        double bestMin = paramElement.attribute ("bestMinimum").toDouble (&ok);
                        if (!ok) { bestMin = min; }
                        double bestMax = paramElement.attribute ("bestMaximum").toDouble (&ok);
                        if (!ok) { bestMax = max; }
                        v = new AcceptRange (bestMin, bestMax, min, max);
                    } else {
                        v = validator (validatorType);
                    }
                    qm -> addParameter (paramLabel, paramName, initial, v);
                    if (paramElement.hasAttribute ("display")) {
//                        qm->showParameter (paramName, paramElement.attribute ("display").toLower () == "edit");
                    }
                }

            }
        }
        bool showName = true;
        //if (element.hasAttribute ("showName")) {
        //    showName = element.attribute ("showName") == "true";
        //}

        // n -> showName (showName);
        return n;
    } else {
        return nullptr;
    }
}

ParamValidator* ModuleFactory::validator (const QString& validatorType) {
    if (validatorType == "AcceptPositive") { return new AcceptPositive(); }
    if (validatorType == "AcceptNoiseValue") { return new AcceptNoiseValue(); }
    if (validatorType == "PreferNoiseValue") { return new PreferNoiseValue(); }
    if (validatorType == "AcceptAngleDegrees") { return new AcceptAngleDegrees (); }
    if (validatorType == "AcceptRange") { return new AcceptRange (-1.0, 1.0); }
    if (validatorType == "AcceptInteger") { return new AcceptInteger(); }
    if (validatorType == "PreferInteger") { return new PreferInteger(); }
    return new AcceptAnyRubbish();
}

Node* ModuleFactory::createModule (const QString& type, CalenhadModel* model) {
    return inflateModule (type, model);
}

Node* ModuleFactory::clone (Node* other) {
    Node* n = createModule (other -> nodeType(), other -> model());
    for (QString key : n -> parameters()) {
        n -> setParameter (key, other -> parameter (key));
    }
    return n;
}

int ModuleFactory::seed () {
    return _seed;
}

QStringList ModuleFactory::paramNames() {
    return _paramNames;
}

QString ModuleFactory::label (const QString& type) {
    return _moduleLabels.contains (type) ? _moduleLabels.value (type) : QString();
}

QSizeF ModuleFactory::scale (const QString& type) {
    return _moduleScales.contains (type) ? _moduleScales.value (type) : QSizeF (1.0, 1.0);
}

QString ModuleFactory::description (const QString& type) {
    return _moduleDescriptions.contains (type) ? _moduleDescriptions.value (type) : QString();
};

QString ModuleFactory::glsl (const QString& type) {
    return _moduleCodes.contains (type) ? _moduleCodes.value (type) : QString();
}

QStringList ModuleFactory::types () {
    QStringList list = _types.keys();
    return list;
}

