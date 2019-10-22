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
#include "../CalenhadServices.h"

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

void AltitudeMap::initialise() {
    Module::initialise();
}

void AltitudeMap::makeContentPanel() {

    addContentPanel();
    // prepare the editor
    _editor = new AltitudeMapEditor (this);
    connect (_editor, SIGNAL (accepted()), this, SLOT (updateEntries()));
    connect (_editor, SIGNAL (rejected()), this, SLOT (editingFinished()));

    // set up the curve type roster with curve models
    _curves.insert (CurveType::Altitude, new CubicSpline());

    _curves.insert (CurveType::Terrace, new TerraceCurve());
    TerraceCurve* invertedTerrace = new TerraceCurve();
    invertedTerrace -> InvertTerraces (true);
    _curves.insert (CurveType::InvertedTerrace, invertedTerrace);
    _curve = _curves.value (CurveType::Altitude);

    QPushButton* editButton = new QPushButton (this);
    editButton->setText ("Edit mapping");
    connect (editButton, SIGNAL (pressed()), this, SLOT (editAltitudeMap()));
    _contentLayout -> addRow ("", editButton);
    resetMap ();
}

void AltitudeMap::editAltitudeMap() {
    // preserve the existing state so that we can undo
    preserve();


    if (dynamic_cast<TerraceCurve*> (_curve)) {
        _editor -> setCurveType (dynamic_cast<TerraceCurve*> (_curve) -> IsTerracesInverted() ? CurveType::InvertedTerrace : CurveType::Terrace);
    } else {
        _editor -> setCurveType (CurveType::Altitude);
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
    if (mapElement.attribute ("function") == "terrace") {
        if (mapElement.attribute ("inverted") == "true") {
            _curve = _curves.find (CurveType::InvertedTerrace).value();
        } else {
            _curve = _curves.find (CurveType::Terrace).value();
        }
    } else {
        _curve = _curves.find (CurveType::Altitude).value();
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
        return ("spline");
    }

}

bool AltitudeMap::isFunctionInverted() {
    if (curveFunction() == "terrace") {
        return dynamic_cast<TerraceCurve*> (_curve)->IsTerracesInverted();
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

