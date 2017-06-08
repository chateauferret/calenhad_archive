//
// Created by martin on 06/03/17.
//

#include "QIcosphereMap.h"
#include "../pipeline/CalenhadModel.h"
#include <QThread>
#include "../nodeedit/qneconnection.h"
#include "../nodeedit/Calenhad.h"
#include "../preferences.h"
#include "../CalenhadServices.h"

using namespace noise::module;
using namespace noise::utils;
using namespace icosphere;

QIcosphereMap::QIcosphereMap (QWidget* parent) : QModule (nullptr), _depth (5), _bounds (Bounds()), _icosphere (nullptr) {

}

QIcosphereMap::~QIcosphereMap () { }

void QIcosphereMap::initialise() {
    QModule::initialise();
    _depthSpin = countParameterControl ("Depth");
    _depthSpin -> setMinimum (3);
    _depthSpin -> setMaximum (13);

    _vertexCountLabel = new QLabel();
    _contentLayout -> addRow (tr ("Vertices"), _vertexCountLabel);
    connect (_depthSpin, SIGNAL (editingFinished()), this, SLOT (setIcosphereDepth()));
    _contentLayout -> addRow (tr ("Depth"), _depthSpin);
    _isInitialised = true;

    connect (this, SIGNAL (icosphereChangeRequested()), this, SLOT (buildIcosphere()));
    connect (this, SIGNAL (initialised()), this, SLOT (buildIcosphere()));
    connect (_ports [0], &QNEPort::connected, this, &QIcosphereMap::generateMap);
    setBounds (_bounds);
    setIcosphereDepth (_depth);

    emit initialised();
}

void QIcosphereMap::generateMap() {
    if (! _module) {
        IcosphereMap* map = new IcosphereMap ();
        _module = map;
    }
    QNEPort* port = _ports [0];
    if (! (port -> connections().isEmpty ())) {
        QNEConnection* c = port -> connections() [0];
        QNEPort* p = c -> port1 () == _ports[0] ? c -> port2() : c -> port1();
        Module* source = ((QModule*) p -> owner()) -> module();
        if (source) {
            _module -> SetSourceModule (0, *source);
            connect ((IcosphereMap*) _module, SIGNAL (available (std::shared_ptr<icosphere::Icosphere>)), this, SLOT (icosphereBuilt (std::shared_ptr<icosphere::Icosphere>)));
            connect (this, SIGNAL (nodeChanged()), (IcosphereMap*) _module, SLOT (cancelBuild()));
            ((IcosphereMap*) _module) -> buildIcosphere (_depth, _bounds);
        }
    }
}

void QIcosphereMap::buildIcosphere () {
    generateMap ();
    if (_icosphere) {
        _vertexCountLabel -> setText (QString::number (_icosphere->vertexCount ()) + " vertices generated");
    } else {
        _vertexCountLabel -> setText ("No vertices generated");
    }
}

void QIcosphereMap::addInputPorts() {
    QNEPort* input = new QNEPort (QNEPort::InputPort, 0, "Input");
    addPort (input);
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
        emit icosphereChangeRequested();
    }
}

void QIcosphereMap::icosphereBuilt (std::shared_ptr<icosphere::Icosphere> icosphere) {
    if (icosphere != _icosphere) {
        _icosphere = icosphere;
    }
    _vertexCountLabel -> setText (QString::number (_icosphere -> vertexCount ()) + " vertices generated");
    emit nodeChanged();
}

void QIcosphereMap::setBounds (const icosphere::Bounds& bounds) {
    _bounds = bounds;
    generateMap();
    emit icosphereChangeRequested();
}


bool QIcosphereMap::isRenderable() {
    return _module != nullptr && QNode::isRenderable();
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

QString QIcosphereMap::nodeType () {
    return CalenhadServices::preferences() -> calenhad_module_icospheremap;
}

QIcosphereMap* QIcosphereMap::clone () {
    QIcosphereMap* qm = QIcosphereMap::newInstance();
    if (qm) {
        qm -> setIcosphereDepth (_depth);
    }
    return qm;
}

void QIcosphereMap::inflate (const QDomElement& element) {
    QModule::inflate (element);
    bool ok;
    double depth = _model -> readParameter (element, "depth").toDouble (&ok);
    if (ok) { setIcosphereDepth (depth); }

    QDomElement boundsElement = element.firstChildElement ("bounds");
    QDomElement latlonElement1 = boundsElement.firstChildElement ("geolocation");
    double lat1 = M_PI, lat2 = -M_PI, lon1 = -M_PI * 2, lon2 = M_PI * 2;
    lat1 = latlonElement1.attributeNode ("lat").value ().toDouble (&ok);
    if (ok) { lon1 = latlonElement1.attributeNode ("_lon").value ().toDouble (&ok); }

    QDomElement latlonElement2 = latlonElement1.nextSiblingElement ("geolocation");
    if (ok) { lat2 = latlonElement2.attributeNode ("lat").value ().toDouble (&ok); }
    if (ok) { lon2 = latlonElement2.attributeNode ("_lon").value ().toDouble (&ok); }
    if (ok) {
        bool crossesDateline = boundsElement.attributeNode ("crossesDateline").value ().toLower () == "y";
        Bounds bounds = Bounds (lat1, lat2, lon1, lon2, crossesDateline);
        setBounds (bounds);
    }
}

void QIcosphereMap::serialise (QDomDocument& doc) {
    QModule::serialise (doc);
    _model -> writeParameter (_element, "depth", QString::number (_depth));
    QDomElement boundsElement = doc.createElement ("bounds");
    _element.appendChild (boundsElement);
    QDomElement latlonElement = doc.createElement ("geolocation");
    boundsElement.appendChild (latlonElement);
    latlonElement.setAttribute ("lat", _bounds.lat1);
    latlonElement.setAttribute ("_lon", _bounds.lon1);
    latlonElement = doc.createElement ("geolocation");
    boundsElement.appendChild (latlonElement);
    latlonElement.setAttribute ("lat", _bounds.lat2);
    latlonElement.setAttribute ("_lon", _bounds.lon2);
    boundsElement.setAttribute ("crossesDateline", _bounds._crossesDateline ? "y" : "n");
}

int QIcosphereMap::icosphereDepth () {
    return _depth;
}
