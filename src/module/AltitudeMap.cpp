//
// Created by martin on 31/03/17.
//

#include <mapping/CubicSpline.h>
#include <mapping/TerraceCurve.h>
#include <controls/altitudemap/AltitudeMapping.h>
#include "AltitudeMap.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../controls/altitudemap/AltitudeMapEditor.h"
#include "../controls/altitudemap/AltitudeMapping.h"
#include "nodeedit/Port.h"
#include "../actions/XmlCommand.h"
#include "../nodeedit/CalenhadController.h"
#include "src/CalenhadServices.h"

using namespace calenhad::module;
using namespace calenhad::controls::altitudemap;
using namespace calenhad::actions;
using namespace calenhad::nodeedit;
using namespace calenhad::mapping;

AltitudeMap::AltitudeMap (QWidget* parent) : Module (CalenhadServices::preferences ()->calenhad_module_altitudemap, nullptr), _editor (nullptr), _curve (nullptr) {
    makeContentPanel();
}

AltitudeMap::~AltitudeMap() {
    for (Curve* c : _curves.values ()) { delete c; }
    delete _editor;
}

void AltitudeMap::makeContentPanel() {

    addContentPanel();
    // prepare the editor
    _editor = new AltitudeMapEditor (this);
    connect (_editor, SIGNAL (accepted()), this, SLOT (updateEntries()));
    connect (_editor, SIGNAL (rejected()), this, SLOT (editingFinished()));

    // set up the curve type roster with curve models
    _curves.insert (CurveType::QuarticSpline, new CubicSpline());
    _curves.insert (CurveType::Terrace, new TerraceCurve());
    _curves.insert (CurveType::Linear, new Curve());
    TerraceCurve* invertedTerrace = new TerraceCurve();
    invertedTerrace -> InvertTerraces (true);
    _curves.insert (CurveType::InvertedTerrace, invertedTerrace);
    _curve = _curves.value (CurveType::QuarticSpline);

    QPushButton* editButton = new QPushButton (this);
    editButton -> setText ("Edit mapping");
    connect (editButton, SIGNAL (pressed()), this, SLOT (editAltitudeMap()));
    _contentLayout -> addRow ("", editButton);
    resetMap ();
}

void AltitudeMap::editAltitudeMap() {
    // preserve the existing state so that we can undo
    preserve();

    if (_curve) {
        if (dynamic_cast<TerraceCurve*> (_curve)) {
            _editor->setCurveType (dynamic_cast<TerraceCurve*> (_curve) -> IsTerracesInverted () ? CurveType::InvertedTerrace : CurveType::Terrace);
        } else {
            if (dynamic_cast<CubicSpline*> (_curve)) {
                _editor -> setCurveType (CurveType::QuarticSpline);
            } else {
                _editor -> setCurveType (CurveType::Linear);
            }
        }
    }
    QVector<calenhad::controls::altitudemap::AltitudeMapping> points = entries();
    _editor -> setEntries (points);
    _editor -> setModal (false);
    _editor -> show();
}

// retrieve parameters from the curve editing dialog: control points and curve type (function)
// based on those assign the relevant curve and set tbe control points on it,
void AltitudeMap::updateEntries() {

    CurveType curveType = _editor -> curveType();
    _curve = _curves.find (curveType).value();
    clearMap();
    QVector<calenhad::controls::altitudemap::AltitudeMapping> e = _editor -> getEntries();
    if (_editor) {
        _editor -> hide();
    }
    for (AltitudeMapping point : e) {
        addEntry (point);
    }

    // preserve the change for undo purposes
    QDomDocument doc;
    QDomElement root = doc.createElement ("calenhad");
    doc.appendChild (root);
    QDomElement element = doc.documentElement();
    serialize (element);
    QString newXml = doc.toString();

    XmlCommand* c = new XmlCommand (_model, _oldXml);
    _model -> controller() -> doCommand (c);
    c -> setNewXml (newXml);

    emit nodeChanged();
    //editingFinished();
}

void AltitudeMap::editingFinished() {
    if (isVisible() && _editor) {
        _editor -> hide();
    }
}

void AltitudeMap::addEntry (const QString& x, const QString& y) {
    addEntry (AltitudeMapping (x, y));
}

void AltitudeMap::addEntry (const AltitudeMapping& entry) {
    for (Curve* curve : _curves.values ()) {
        curve -> addMapping (entry);
    }
}

QVector<AltitudeMapping>  AltitudeMap::entries () const {
    return _curve -> mappings();
};

void AltitudeMap::resetMap () {
    clearMap();
    addEntry ("-1.0", "-1.0");
    addEntry ("1.0", "1.0");
    addEntry ("0.0", "0.0");
    addEntry ("-0.5", "-0.5");
    addEntry ("0.5", "0.5");
    emit nodeChanged();
}

void AltitudeMap::clearMap() {
    for (Curve* curve : _curves) {
        curve -> clear();
    }
}

AltitudeMap* AltitudeMap::clone () {
    AltitudeMap* n = (AltitudeMap*) Node::clone();
    QDomDocument doc;
    QDomElement root = doc.createElement ("calenhad");
    doc.appendChild (root);
    QDomElement element = doc.documentElement();
    serialize (element);
    inflate (root.firstChildElement ("map"));
    return n;
}

void AltitudeMap::inflate (const QDomElement& element) {
    Module::inflate (element);
    QDomElement mapElement = element.firstChildElement ("map");
    _curve = nullptr;
    if (mapElement.attribute ("function") == "terrace") {
        if (mapElement.attribute ("inverted") == "true") {
            _curve = _curves.find (CurveType::InvertedTerrace).value();
        } else {
            _curve = _curves.find (CurveType::Terrace).value();
        }
    } else {
        if (mapElement.attribute ("function") != "linear") {
            _curve = _curves.find (CurveType::QuarticSpline).value ();
        } else {
            _curve = _curves.find (CurveType::Linear).value ();
        }
    }
    QDomNodeList entryElements = mapElement.elementsByTagName ("entry");
    clearMap ();
    for (int i = 0; i < entryElements.size(); i++) {
        QDomNamedNodeMap attributes = entryElements.at (i).attributes();
        QString x, y;
        x = attributes.namedItem ("x").nodeValue();
        y = attributes.namedItem ("y").nodeValue ();
        addEntry (x, y);
    }
}

void AltitudeMap::serialize (QDomElement& element) {
    Module::serialize (element);
    QDomElement mapElement = _document.createElement ("map");
    _element.appendChild (mapElement);
    mapElement.setAttribute ("function", curveFunction());
    if (isFunctionInverted()) {
        mapElement.setAttribute ("inverted", true);
    }
    QVector<AltitudeMapping> e = entries ();
    for (AltitudeMapping entry : e) {
        QDomElement entryElement = _document.createElement ("entry");
        entryElement.setAttribute ("x", entry.expressionX());
        entryElement .setAttribute ("y", entry.expressionY());
        mapElement.appendChild (entryElement);
    }
}

QString AltitudeMap::curveFunction() {
    if (dynamic_cast<TerraceCurve*> (_curve)) {
        return ("terrace");
    } else {
        if (dynamic_cast<CubicSpline*> (_curve)) {
            return ("spline");
        } else return "linear";
    }

}

bool AltitudeMap::isFunctionInverted() {
    if (curveFunction() == "terrace") {
        return dynamic_cast<TerraceCurve*> (_curve) -> IsTerracesInverted();
    } else {
        return false;
    }
}

void AltitudeMap::preserve () {
    QDomDocument doc;
    QDomElement root = doc.createElement ("calenhad");
    doc.appendChild (root);
    QDomElement element = doc.documentElement();
    serialize (element);
    _oldXml = doc.toString();
}

QString AltitudeMap::glsl() {
    // input is below the bottom of the range
    QString code;// = "float _" + name() + " (ivec3 pos, vec3 c, vec2 g) {\n";
    code += "  float value = %0 ;\n";
    code += "  if (value < " + QString::number (entries().first().x()) + ") { return " + QString::number (entries().first().y()) + "; }\n";

    for (int j = 0; j < entries().size (); j++) {
        // Do we need to sort the entries?
        QString mapFunction;

        // compose the mapping function
        // spline function
        if (curveFunction() == "spline") {
            double x[4], y[4];

            for (int i = 0; i < 4; i++) {
                int k = std::max (j + i - 2, 0);
                k = std::min (k, entries().size () - 1);
                x[i] = entries().at (k).x ();
                y[i] = entries().at (k).y ();
            }

            // Compute the alpha value used for cubic interpolation.
            mapFunction += "        float alpha = ((value - " + QString::number (x[1]) + ") / " + QString::number (x[2] - x[1]) + ");\n";
            mapFunction += "        return cubicInterpolate (" +
                           QString::number (y[0]) + ", " +
                           QString::number (y[1]) + ", " +
                           QString::number (y[2]) + ", " +
                           QString::number (y[3]) + ", alpha);";
            code += "  if (value > " + QString::number (x[1]) + " && value <= " + QString::number (x[2]) + ") {\n" + mapFunction + "\n   }\n";
        }

        // terrace function
        if (curveFunction() == "terrace" || curveFunction() == "linear") {
            double x[2], y[2];
            for (int i = 0; i < 2; i++) {
                int k = std::max (j + i - 1, 0);
                k = std::min (k, entries().size () - 1);
                x[i] = entries().at (k).x ();
                y[i] = entries().at (k).y ();
            }

            // Compute the alpha value used for cubic interpolation.
            mapFunction += "        float alpha = ((value - " + QString::number (x[0]) + ") / " + QString::number (x[1] - x[0]) + ");\n";
            if (isFunctionInverted ()) { mapFunction += "        alpha = 1 - alpha;\n"; }
            if (curveFunction() != "linear") {
                mapFunction += "        alpha *= alpha;\n";
            }
            mapFunction += "        return mix ( float(" +
                           QString::number (isFunctionInverted () ? y[1] : y[0]) + "), float (" +
                           QString::number (isFunctionInverted () ? y[0] : y[1]) + "), " +
                           "alpha);";
            code += "  if (value > " + QString::number (x[0]) + " && value <= " + QString::number (x[1]) + ") {\n" + mapFunction + "\n   }\n";
        }
    }

    // input is beyond the top of the range
    code += "  if (value > " + QString::number (entries().last ().x ()) + ") { return " + QString::number (entries().last ().y ()) + "; }\n";
    code += "}\n";
    expandCode (code);
    return code;
}
