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

using noise::module::Module;

QAltitudeMap::QAltitudeMap (QWidget* parent) : QModule (new TranslatePoint(), parent) {

}

QAltitudeMap::~QAltitudeMap() {
    if (_editor) { delete _editor; }
}

void QAltitudeMap::initialise() {
    QModule::initialise();
    _name = "New Altitude Map";
    _module = new Curve();

    QPushButton* editButton = new QPushButton (this);
    editButton->setText ("Edit mapping");
    connect (editButton, SIGNAL (pressed()), this, SLOT (editAltitudeMap()));
    _contentLayout -> addRow ("", editButton);
    clearMap();
    _isInitialised = true;
    emit initialised();

}

void QAltitudeMap::editAltitudeMap() {
    _editor = new AltitudeMapEditor();
    connect (_editor, SIGNAL (accepted()), this, SLOT (updateEntries()));
    connect (_editor, SIGNAL (rejected()), this, SLOT (editingFinished()));
    QVector<QPointF> entries = getEntries();
    _editor -> setEntries (entries);
    _editor -> exec();
}

void QAltitudeMap::updateEntries() {
    QVector<QPointF> entries = _editor -> getEntries();

    if (dynamic_cast<Curve*> (_module)) {
        Curve* c = dynamic_cast<Curve*> (_module);
        c -> ClearAllControlPoints();
    }
    if (dynamic_cast<Terrace*> (_module)) {
        Terrace* t = dynamic_cast<Terrace*> (_module) ;
        t -> ClearAllControlPoints();
    }
    for (QPointF point : entries) {
        addEntry (point.x(), point.y());
    }
    emit nodeChanged ("entries", entries.size ());
    editingFinished();
}

void QAltitudeMap::editingFinished() {
    if (_editor) { delete _editor; }
}


void QAltitudeMap::addEntry (const double& in, const double& out) {
    if (dynamic_cast<Curve*> (_module)) {
        Curve* c = dynamic_cast<Curve*> (_module);
        c -> AddControlPoint (in, out);
     }
    if (dynamic_cast<Terrace*> (_module)) {
        Terrace* t = dynamic_cast<Terrace*> (_module) ;
        t -> AddControlPoint (in);
    }

}

void QAltitudeMap::removeEntry (const double& key) {
    if (dynamic_cast<Curve*> (_module)) {
        Curve* c = dynamic_cast<Curve*> (_module);
        c -> ClearAllControlPoints();
    }
    if (dynamic_cast<Terrace*> (_module)) {
        Terrace* t = dynamic_cast<Terrace*> (_module) ;
        t -> ClearAllControlPoints();
    }
    QVector<QPointF> entries = getEntries();

    for (QPointF point : entries) {
        for (QPointF point : entries) {
            addEntry (point.x(), point.y());
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

void QAltitudeMap::clearMap() {
    if (dynamic_cast<Curve*> (_module)) {
        Curve* c = dynamic_cast<Curve*> (_module);
        c -> ClearAllControlPoints();
    }
    if (dynamic_cast<Terrace*> (_module)) {
        Terrace* t = dynamic_cast<Terrace*> (_module) ;
        t -> ClearAllControlPoints();
    }
    addEntry (-1.0, -1.0);
    addEntry (1.0, 1.0);
    addEntry (0.0, 0.0);
    addEntry (-0.5, -0.5);
    addEntry (0.5, 0.5);
    emit nodeChanged ("entries", 2);
}

QAltitudeMap* QAltitudeMap::newInstance () {
    QAltitudeMap* qm = new QAltitudeMap();
    qm -> initialise();
    return qm;
}

QString QAltitudeMap::moduleType () {
    return Calenhad::preferences -> calenhad_module_altitudemap;
}

QAltitudeMap* QAltitudeMap::addCopy (CalenhadModel* model) {
    QAltitudeMap* qm = QAltitudeMap::newInstance();
    if (qm) {
        qm -> setModel (model);

    }
    return qm;
}

void QAltitudeMap::inflate (const QDomElement& element, MessageFactory* messages) {
    QModule::inflate (element, messages);
    bool ok;
}

void QAltitudeMap::serialise (QDomDocument& doc, MessageFactory* messages) {
    QModule::serialise (doc, messages);

}
