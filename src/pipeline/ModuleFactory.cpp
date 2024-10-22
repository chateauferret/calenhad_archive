//
// Created by martin on 11/11/16.
//

#include <iostream>
#include "ModuleFactory.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "module/AltitudeMap.h"
#include "pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include <QList>
#include <module/StructuredGrid.h>
#include <nodeedit/Port.h>
#include <src/module/CubeRasterModule.h>
#include "../module/RasterModule.h"
#include "../module/Cache.h"
#include "../module/Map.h"
#include "../noiseconstants.h"
#include "../nodeedit/NodeBlock.h"
#include "../module/TectonicsModule.h"
//#include "../module/Erosion.h"

using namespace calenhad;
using namespace calenhad::pipeline;
using namespace calenhad::module;
using namespace calenhad::module::algorithm;
using namespace calenhad::expressions;
using namespace calenhad::nodeedit;

ModuleFactory::ModuleFactory() {
    initialise();
}


ModuleFactory:: ~ModuleFactory() {

}

QPixmap ModuleFactory::getIcon (const QString& type) {
   return makeIcon (type);
}

void ModuleFactory::initialise() {
    QDomDocument doc;
    QString moduleFile = CalenhadServices::preferences() -> calenhad_moduletypes_filename;
    if (! QFileInfo (moduleFile).exists()) {
        moduleFile = ":/modules.xml";
    }
    if (CalenhadServices::readXml (moduleFile, doc)) {
        std::cout << "Modules definition at " << CalenhadServices::preferences() -> calenhad_moduletypes_filename.toStdString() << "\n";
        QDomElement paletteElement = doc.documentElement();
        QDomNodeList nodes = paletteElement.elementsByTagName ("module");
        for (int i = 0; i < nodes.size(); i++) {
            QDomElement element = nodes.at (i).toElement();
            QString type = element.attribute ("name");
            _types.insert (type, element);


            // Create code snippet to be inserted into the compute shader to realise each module's function.
            // %n - will be replaced with the module's name which will serve as the method name representing it in the generated shader code.
            // %0, %1, etc - will be replaced with calls to the module outputs serving the corresponding port as input.
            // %frequency, %lacunarity etc - will be replaced with the value of that parameter, for parameters named in calenhad::graph::Graph::_params.

            QDomElement glslElement = element.firstChildElement ("glsl");
            QString glsl = glslElement.text();
            _moduleCodes.insert (type, glsl);
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
        }


    } else {
        CalenhadServices::messages() -> message ("File error", "Couldn't read file " + CalenhadServices::preferences() -> calenhad_moduletypes_filename);
    }
}

QString ModuleFactory::getIconFile (const QString& icon) {
    QString iconFile = CalenhadServices::preferences() -> calenhad_moduletypes_icons_path + icon + ".png";
    if (! QFileInfo (iconFile).exists()) {
        iconFile = ":/icons/" + icon + ".png";
    }
    return iconFile;
}

QDomElement ModuleFactory::xml (const QString& type) {
    return _types.value (type);
}

Node* ModuleFactory::inflateModule (const QString& type, CalenhadModel* model) {
    if (_types.keys().contains (type)) {
        bool ok;
        QDomElement element = _types.value (type);
        QString role = element.attribute ("role");
        bool suppressRender = element.hasAttribute ("render") && element.attribute ("render").toLower() == "false";

        // special types which need more than generic construction code
        Node* n = nullptr;
        Module* qm = nullptr;
        if (type == CalenhadServices::preferences() -> calenhad_module_altitudemap) { AltitudeMap* am = new AltitudeMap(); qm = am; n = qm; }
        if (type == CalenhadServices::preferences() -> calenhad_module_raster) { RasterModule* rm = new RasterModule(); qm = rm; n = qm; }
        if (type == CalenhadServices::preferences() -> calenhad_module_cuberaster) { CubeRasterModule* rm = new CubeRasterModule(); qm = rm; n = qm; }
        if (type == CalenhadServices::preferences() -> calenhad_module_cache) { Cache* cm = new Cache (type); qm = cm; n = qm; }
        if (type == CalenhadServices::preferences() -> calenhad_module_map) { Map* mm = new Map(); qm = mm; n = qm; }
        if (type == CalenhadServices::preferences() -> calenhad_module_tectonics) { TectonicsModule* t = new TectonicsModule(); qm = t; n = qm; }
        //if (type == CalenhadServices::preferences() -> calenhad_module_erosion) { Erosion* e = new Erosion(); qm = e; n = qm; }

        if (role == "algorithm") {
            Cache* c = new Cache (type); qm = c; n = qm;
            QString algorithmName = element.attribute ("algorithm");
            if (algorithmName != QString::null) {
                Algorithm* a = algorithm (algorithmName);
                if (!a) {
                    std::cout << "No algorithm '" << algorithmName.toStdString() << "' available for module " << type.toStdString() << "\n";
                }
            } else {
                std::cout << "No algorithm specified for module " << type.toStdString() << "\n";
            }
        }

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

        if (model) {
            n -> setModel (model);
        }

        if (qm) {
            QDomElement portsElement = element.firstChildElement ("ports");
            QDomNodeList portNodesList = portsElement.elementsByTagName ("input");
            for (int i = 0; i < portNodesList.size(); i++) {
                QDomElement portNode = portNodesList.at (i).toElement ();
                QString portType = portNode.attribute ("type");
                int pt = portType == "value" ? Port::InputPort : Port::ControlPort;
                QString portName = portNode.attribute ("name");
                QString label = portNode.attribute ("label");
                int index = portNode.attribute ("index").toInt (&ok);
                if (ok) {
                    bool required = portNode.hasAttribute ("required") && portNode.attribute ("required") == "true";
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
            for (int i = 0; i < paramNodesList.size(); i++) {
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
                    qm -> addParameter (paramLabel, paramName, initial);
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
                          //qm->showParameter (paramName, paramElement.attribute ("display").toLower () == "edit");
                    }
                }

            }

            QDomElement rangeElement = element.firstChildElement ("range");
            if (rangeElement.hasAttribute ("min")) {
                qm -> setMinExpr (rangeElement.attribute ("min"));
            } else {
                qm -> setMinExpr ("-1");
            }
            if (rangeElement.hasAttribute ("max")) {
                qm -> setMaxExpr (rangeElement.attribute ("max"));
            } else {
                qm -> setMaxExpr ("1");
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
    else return new AcceptAnyRubbish();
}


Algorithm* ModuleFactory::algorithm (const QString& algorithmType) {
//    if (algorithmType == "normalise") { return new Normalise(); }
//    if (algorithmType == "") { return new (); }
//    if (algorithmType == "") { return new (); }
//    if (algorithmType == "") { return new  (); }
//  if (algorithmType == "") { return new ; }
//    if (algorithmType == "") { return new (); }
//    if (algorithmType == "") { return new (); }
//    else
    return nullptr;
}


Node* ModuleFactory::createModule (const QString& name, const QString& type, CalenhadModel* model) {
    Node* n = inflateModule (type, model);
    n -> setName (name);
    return n;
}

Node* ModuleFactory::clone (Node* other) {
    Node* n = createModule (other -> model() -> uniqueName (other -> name()), other -> nodeType(), other -> model());
    for (QString key : n -> parameters()) {
        n -> setParameter (key, other -> parameter (key));
    }
    return n;
}

int ModuleFactory::seed () const {
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
    QString  code = // "float value (vec3 c, vec2 g, int i0, int i1, int i2, int i3) {\n";
    //code +=
            "\n    " + (_moduleCodes.contains (type) ? _moduleCodes.value (type) : QString()) + ";\n";
    code += "}\n";
    return code;
}

QAction* ModuleFactory::makeModuleTool (const QString& type) {
    QAction* action = new QAction (label (type));
    action -> setToolTip (description (type));
    action -> setData (type);
    action -> setCheckable (true);
    action -> setChecked (false);
    return action;
}

QStringList ModuleFactory::types () {
    QStringList list = _types.keys();
    return list;
}

QPixmap ModuleFactory::makeIcon (const QString& type) {
    if (type != QString()) {
        Module *module = (Module *) createModule(QString(), type, nullptr);
        if (module) {
            QGraphicsItem *handle = module->makeHandle();
            NodeBlock *block = (NodeBlock *) handle;
            for (Port *port : module -> ports()) {
                block -> addPort (port);
            }
            QSize r = block -> boundingRect().size().toSize();
            QPixmap img (32, 32);
            img.fill (Qt::transparent);
            QPainter painter (&img);
            //painter.setRenderHint (QPainter::Antialiasing);
            //painter.translate (-1 * (block -> boundingRect ().topLeft ()));
            QStyleOptionGraphicsItem opt;

            QBrush _brush = QBrush (CalenhadServices::preferences() -> calenhad_module_brush_color_normal);
            painter.setBrush (_brush);
            QPen _pen = QPen (CalenhadServices::preferences() -> calenhad_module_text_color_normal);
            painter.setPen (_pen);
            painter.drawRect (10, 10, r.width() - 20, r.height() - 20);
            delete module;
            delete handle;
            return img;
        } else return QPixmap();
    } else return QPixmap();
}

