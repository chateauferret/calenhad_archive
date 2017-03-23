//
// Created by martin on 06/03/17.
//

#include "QIcosphereMap.h"
#include "../pipeline/CalenhadModel.h"
#include <QThread>
#include "../nodeedit/qneconnection.h"

using namespace noise::module;
using namespace noise::utils;
using namespace icosphere;

QIcosphereMap::QIcosphereMap (QWidget* parent) : QModule (nullptr), _depth (5), _bounds (Bounds()) { }

QIcosphereMap::~QIcosphereMap () { }

void QIcosphereMap::initialise() {
    QModule::initialise();
    _depthSpin = countParameterControl ("Depth");
    _depthSpin -> setMinimum (3);
    _depthSpin -> setMaximum (13);

    _vertexCountLabel = new QLabel();
    _contentLayout -> addRow (tr ("Vertices"), _vertexCountLabel);
    connect (_depthSpin, SIGNAL (editingFinished()), this, SLOT (setIcosphereDepth()));
    connect (this, SIGNAL (nodeChanged (const QString&, const QVariant&)), this, SLOT (resetUI (const QString&)));
    _contentLayout -> addRow (tr ("Depth"), _depthSpin);
    _isInitialised = true;

    emit nodeChanged ("Initialised", 0);
}


void QIcosphereMap::generateMap() {
    setEnabled (false);
    emit nodeChanged ("Generating", 0);
    IcosphereMap* map = new IcosphereMap();
    _module = map;
    QNEPort* port = _ports [0];
    if (! (port -> connections().isEmpty ())) {
        QNEConnection* c = port -> connections() [0];
        QNEPort* p = c -> port1 () == _ports[0] ? c -> port2() : c -> port1();
        Module* source = p->module ();
        if (source) {
            map -> SetSourceModule (0, *source);
            connect (map, SIGNAL (available (std::shared_ptr<icosphere::Icosphere>)), this, SLOT (icosphereBuilt (std::shared_ptr<icosphere::Icosphere>)));
            connect (this, SIGNAL (nodeChanged (const QString&, const QVariant&)), map, SLOT (cancelBuild()));
            map -> buildIcosphere (_depth, _bounds);
        }
    }
}

void QIcosphereMap::resetUI (const QString& type) {
    if (type == "Updated") {
        _vertexCountLabel -> setText (QString::number (_icosphere -> vertexCount ()) + " vertices generated");
    }
    if (type == "Bounds" || type == "Inputs" || type == "Depth") {
        generateMap();
    }
}

void QIcosphereMap::addInputPorts() {
    QNEPort* input = new QNEPort (QNEPort::InputPort, 0, "Input");
    addPort (input);
}

void QIcosphereMap::setUniqueName() {
    int i = 0;
    QString name = "New Icosphere";
    while (_model -> findModule (name)) {
        i++;
        name = QString ("New Icosphere " + QString::number (i));
    }
    setName (name);
    _nameEdit -> setText (_name);
}

void QIcosphereMap::setIcosphereDepth() {
    int depth = _depthSpin -> value();
    setIcosphereDepth (depth);
}

void QIcosphereMap::setIcosphereDepth (const unsigned& depth) {
    if (_depth > 2 && depth < 14) {
        _depth = depth;
        if (_depthSpin -> value () != depth) {
            _depthSpin -> setValue (depth);
        }
        emit nodeChanged ("Depth", depth);
    }
}

void QIcosphereMap::icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere) {
    if (icosphere != _icosphere) {
        _icosphere = icosphere;
    }
    _vertexCountLabel -> setText (QString::number (_icosphere -> vertexCount ()) + " vertices generated");
    setEnabled (true);
    emit nodeChanged ("Updated", 0);
}

void QIcosphereMap::setBounds (const icosphere::Bounds& bounds) {
    _bounds = bounds;
    generateMap();
    emit nodeChanged ("Bounds", 0);
}


bool QIcosphereMap::isRenderable() {
    return _module != nullptr && isEnabled();
}

IcosphereMap* QIcosphereMap::module () {
    IcosphereMap* map = dynamic_cast<IcosphereMap*> (_module);
    return map;
}

QIcosphereMap* QIcosphereMap::newInstance() {
    QIcosphereMap* qm = new QIcosphereMap();
    qm -> initialise();
    return qm;
}

ModuleType QIcosphereMap::type() {
    return ModuleType::ICOSPHEREMAP;
}

QIcosphereMap* QIcosphereMap::addCopy (CalenhadModel* model)  {
    QIcosphereMap* qm = QIcosphereMap::newInstance();
    if (qm) {
        qm -> setModel (model);
        qm -> setIcosphereDepth (_depth);
    }
    return qm;
}

QString QIcosphereMap::typeString () {
    return "Icosphere Map";
}
