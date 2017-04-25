//
// Created by martin on 31/03/17.
//

#include "QAltitudeMap.h"
#include "QTranslateModule.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"
#include "../controls/AltitudeMapEditor.h"
#include "../messagefactory.h"
#include "../controls/AltitudeMapPlot.h"
#include "../actions/ChangeAltitudeMapCommand.h"
#include "../nodeedit/CalenhadController.h"

using noise::module::Module;

QAltitudeMap::QAltitudeMap (QWidget* parent) : QModule (new TranslatePoint(), parent), _editor (nullptr) {

}

QAltitudeMap::~QAltitudeMap() {
    if (_editor) { delete _editor; }
}

void QAltitudeMap::initialise() {
    QModule::initialise();
    _name = "New Altitude Map";

    // prepare the editor
    _editor = new AltitudeMapEditor ();
    connect (_editor, SIGNAL (accepted()), this, SLOT (updateEntries()));
    connect (_editor, SIGNAL (rejected()), this, SLOT (editingFinished()));

    // set up the curve type roster with modules
    Curve* curve = new Curve();
    _modules.insert (CurveType::AltitudeCurve, curve);
    Terrace* terrace = new Terrace();
    _modules.insert (CurveType::TerraceCurve, terrace);
    Terrace* invertedTerrace = new Terrace();
    invertedTerrace -> InvertTerraces (true);
    _modules.insert (CurveType::InvertedTerraceCurve, invertedTerrace);
    _module = curve;

    QPushButton* editButton = new QPushButton (this);
    editButton->setText ("Edit mapping");
    connect (editButton, SIGNAL (pressed()), this, SLOT (editAltitudeMap()));
    _contentLayout -> addRow ("", editButton);
    resetMap ();
    _isInitialised = true;

    // see to it that changes to the input module are propogated to all the modules we might use
    connect (this, &QNode::nodeChanged, this, [=] () {
        for (Module* module : _modules.values ()) {
            try {
                module->SetSourceModule (0, _module->GetSourceModule (0));
            } catch (...) {}
        }
    });
    emit initialised();

}

void QAltitudeMap::editAltitudeMap() {
    // preserve the existing state so that we can undo
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    serialise (doc);
    _oldXml = doc.toString();

    QVector<QPointF> entries = getEntries();
    _editor -> setEntries (entries);
    if (dynamic_cast<Terrace*> (_module)) {
        _editor -> setCurveType (dynamic_cast<Terrace*> (_module) -> IsTerracesInverted() ? CurveType::InvertedTerraceCurve : CurveType::TerraceCurve);
    } else {
        _editor -> setCurveType (CurveType::AltitudeCurve);
    }
    _editor -> exec();
}

// retrieve parameters from the curve editing dialog: control points and curve type (function)
// based on those assign the relevant module and set tbe control points on it,
void QAltitudeMap::updateEntries() {

    CurveType curveType = _editor -> curveType();
    _module = _modules.find (curveType).value();
    clearMap();
    QVector<QPointF> entries = _editor -> getEntries();

    for (QPointF point : entries) {
        addEntry (point.x (), point.y ());
    }

    // preserve the change for undo purposes
    QDomDocument doc;
    QDomElement root = doc.createElement ("model");
    doc.appendChild (root);
    serialise (doc);
    QString newXml = doc.toString();

    ChangeAltitudeMapCommand* c = new ChangeAltitudeMapCommand (this, _oldXml, newXml);
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
    for (Module* module : _modules) {
        if (dynamic_cast<Curve*> (module)) {
            Curve* c = dynamic_cast<Curve*> (module);
            c->AddControlPoint (in, out);
        }
        if (dynamic_cast<Terrace*> (module)) {
            Terrace* t = dynamic_cast<Terrace*> (module);
            t->AddControlPoint (in);
        }
    }
}

void QAltitudeMap::removeEntry (const double& key) {
    QVector<QPointF> entries = getEntries ();
    clearMap();
    for (Module* module : _modules) {
        for (QPointF point : entries) {
            for (QPointF point : entries) {
                if (point.x() != key) {
                    addEntry (point.x (), point.y ());
                }
            }
        }
    }
}

QVector<QPointF>  QAltitudeMap::getEntries() const {
    QVector<QPointF> entries;
    int count;
    if (dynamic_cast<Curve*> (_module)) {
        Curve* c = dynamic_cast<Curve*> (_module);
        count = c -> GetControlPointCount();
        const ControlPoint* points = c -> GetControlPointArray();
        for (int i = 0; i < count; i++) {
            entries.append (QPointF ((points + i) -> inputValue, (points + i) -> outputValue));
        }
    }
    if (dynamic_cast<Terrace*> (_module)) {
        Terrace* t = dynamic_cast<Terrace*> (_module) ;
        count = t -> GetControlPointCount();
        const double* points = t -> GetControlPointArray();
        for (int i = 0; i < count; i++) {
            entries.append (QPointF (*(points + i), *(points + i)));
        }
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
    for (Module* module : _modules) {
        if (dynamic_cast<Curve*> (module)) {
            Curve* c = dynamic_cast<Curve*> (module);
            c->ClearAllControlPoints ();
        }
        if (dynamic_cast<Terrace*> (module)) {
            Terrace* t = dynamic_cast<Terrace*> (module);
            t->ClearAllControlPoints ();
        }
    }
}

QAltitudeMap* QAltitudeMap::newInstance () {
    QAltitudeMap* qm = new QAltitudeMap();
    qm -> initialise();
    return qm;
}

QString QAltitudeMap::moduleType () {
    return CalenhadServices::preferences() -> calenhad_module_altitudemap;
}

QAltitudeMap* QAltitudeMap::addCopy (CalenhadModel* model) {
    QAltitudeMap* qm = QAltitudeMap::newInstance();
    if (qm) {
        qm -> setModel (model);

    }
    return qm;
}

void QAltitudeMap::inflate (const QDomElement& element) {
    QModule::inflate (element);
    QDomElement mapElement = element.firstChildElement ("map");
    if (mapElement.attribute ("function") == "terrace") {
        if (mapElement.attribute ("inverted") == "true") {
            _module = _modules.find (CurveType::InvertedTerraceCurve).value();
        } else {
            _module = _modules.find (CurveType::TerraceCurve).value();
        }
    } else {
        _module = _modules.find (CurveType::AltitudeCurve).value();
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
            CalenhadServices::messages() -> message ("", "Couldn't read coordinates (" + attributes.namedItem ("x").nodeValue() + ", " + attributes.namedItem ("y").nodeValue()
                    + " for " + moduleType() + " module " + name());
        }
    }
}

void QAltitudeMap::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    QDomElement mapElement = _document.createElement ("map");
    _element.appendChild (mapElement);
    if (dynamic_cast<Terrace*> (_module)) {
        mapElement.setAttribute ("function", "terrace");
        mapElement.setAttribute ("inverted", dynamic_cast<Terrace*> (_module) -> IsTerracesInverted ());
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