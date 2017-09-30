//
// Created by martin on 31/03/17.
//

#include <mapping/CubicSpline.h>
#include <mapping/TerraceCurve.h>
#include "QAltitudeMap.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../controls/altitudemap/AltitudeMapEditor.h"
#include "messages/QNotificationHost.h"
#include "../nodeedit/qneport.h"
#include "../actions/XmlCommand.h"
#include "../nodeedit/CalenhadController.h"
#include "../CalenhadServices.h"

using namespace calenhad::qmodule;
using namespace calenhad::controls::altitudemap;
using namespace calenhad::actions;
using namespace calenhad::nodeedit;
using namespace calenhad::mapping;

QAltitudeMap::QAltitudeMap (QWidget* parent) : QModule (CalenhadServices::preferences() -> calenhad_module_altitudemap, 1, parent), _editor (nullptr) {
    makeContentPanel();
}

QAltitudeMap::~QAltitudeMap() {
    for (Curve* c : _curves.values ()) { delete c; }
    if (_editor) { delete _editor; }
}

void QAltitudeMap::makeContentPanel() {

    addContentPanel();
    // prepare the editor
    _editor = new AltitudeMapEditor ();
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

void QAltitudeMap::editAltitudeMap() {
    // preserve the existing state so that we can undo
    preserve();

    QVector<QPointF> entries = getEntries();
    _editor -> setEntries (entries);
    if (dynamic_cast<TerraceCurve*> (_curve)) {
        _editor -> setCurveType (dynamic_cast<TerraceCurve*> (_curve) -> IsTerracesInverted() ? CurveType::InvertedTerrace : CurveType::Terrace);
    } else {
        _editor -> setCurveType (CurveType::Altitude);
    }
    _editor -> exec();
}

// retrieve parameters from the curve editing dialog: control points and curve type (function)
// based on those assign the relevant owner and set tbe control points on it,
void QAltitudeMap::updateEntries() {

    CurveType curveType = _editor -> curveType();
    _curve = _curves.find (curveType).value();
    clearMap();
    QVector<QPointF> entries = _editor -> getEntries();

    for (QPointF point : entries) {
        addEntry (point.x (), point.y ());
    }

    // preserve the change for undo purposes
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    serialize (doc);
    QString newXml = doc.toString();

    XmlCommand* c = new XmlCommand (this, _oldXml, newXml);
    _model -> controller() -> doCommand (c);

    emit nodeChanged();
    editingFinished();
}

void QAltitudeMap::editingFinished() {
    if (_editor) {
        _editor -> hide();
    }
}


void QAltitudeMap::addEntry (const double& in, const double& out) {
    for (Curve* curve : _curves.values ()) {
        if (dynamic_cast<CubicSpline*> (curve)) {
            CubicSpline* c = dynamic_cast<CubicSpline*> (curve);
            c->AddControlPoint (in, out);
        }
        if (dynamic_cast<TerraceCurve*> (curve)) {
            TerraceCurve* t = dynamic_cast<TerraceCurve*> (curve);
            t->AddControlPoint (in, in);
        }
    }
}

QVector<QPointF>  QAltitudeMap::getEntries() const {
    QVector<QPointF> entries;
    int count = _curve -> GetControlPointCount();
    const ControlPoint* points = _curve -> GetControlPointArray();
    for (int i = 0; i < count; i++) {
        entries.append (QPointF ((points + i) -> inputValue, (points + i) -> outputValue));
    }

    std::sort (entries.begin(), entries.end(), [] (const QPointF & a, const QPointF & b) -> bool { return a.x() < b.x(); });
    return entries;
};

void QAltitudeMap::resetMap () {
    clearMap();
    addEntry (-1.0, -1.0);
    addEntry (1.0, 1.0);
    addEntry (0.0, 0.0);
    addEntry (-0.5, -0.5);
    addEntry (0.5, 0.5);
    emit nodeChanged();
}

void QAltitudeMap::clearMap() {
    for (Curve* curve : _curves) {
        curve -> ClearAllControlPoints();
    }
}

QAltitudeMap* QAltitudeMap::clone () {
    QAltitudeMap* n = (QAltitudeMap*) QNode::clone();
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    serialize (doc);
    inflate (root.firstChildElement ("map"));
}

void QAltitudeMap::inflate (const QDomElement& element) {
    QModule::inflate (element);
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
        double x, y;
        bool ok;
        x = attributes.namedItem ("x").nodeValue().toDouble (&ok);
        if (ok) {
            y = attributes.namedItem ("y").nodeValue ().toDouble (&ok);
        }
        if (ok) {
            addEntry (x, y);
        } else {
            CalenhadServices::messages() -> message ("warning", "Couldn't read coordinates (" + attributes.namedItem ("x").nodeValue() + ", " + attributes.namedItem ("y").nodeValue()
                    + " for " + nodeType () + " owner " + name());
        }
    }
}

void QAltitudeMap::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    QDomElement mapElement = _document.createElement ("map");
    _element.appendChild (mapElement);
    if (dynamic_cast<TerraceCurve*> (_curve)) {
        mapElement.setAttribute ("function", "terrace");
        mapElement.setAttribute ("inverted", dynamic_cast<TerraceCurve*> (_curve) -> IsTerracesInverted ());
    } else {
        mapElement.setAttribute ("function", "spline");
    }

    QVector<QPointF> entries = getEntries();
    for (QPointF entry : entries) {
        QDomElement entryElement = _document.createElement ("entry");
        entryElement.setAttribute ("x", entry.x());
        entryElement .setAttribute ("y", entry.y());
        mapElement.appendChild (entryElement);
    }
}

void QAltitudeMap::preserve () {
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    serialize (doc);
    _oldXml = doc.toString();
}
