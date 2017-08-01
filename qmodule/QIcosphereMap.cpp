//
// Created by martin on 06/03/17.
//

#include "QIcosphereMap.h"
#include "../pipeline/CalenhadModel.h"
#include <QThread>
#include <messages/QProgressNotification.h>
#include <icosphere/IcosphereBuilder.h>
#include "../nodeedit/qneconnection.h"
#include "../nodeedit/Calenhad.h"
#include "preferences/preferences.h"
#include "../CalenhadServices.h"
#include "../nodeedit/qneport.h"
#include "../controls/QNoiseMapViewer.h"
#include <QMenu>

using namespace noise::module;
using namespace noise::utils;
using namespace icosphere;
using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;

QIcosphereMap::QIcosphereMap (QWidget* parent) : QModule (CalenhadServices::preferences() -> calenhad_module_icospheremap, new IcosphereModule()), _depth (5), _bounds (Bounds()) {
    makeContentPanel();
}

QIcosphereMap::~QIcosphereMap () { }

void QIcosphereMap::makeContentPanel() {
    addContentPanel();
    std::cout << "Content " << _expander -> indexOf (_content) << "\n";
    _depthSpin = countParameterControl ("Depth");
    _depthSpin -> setMinimum (3);
    _depthSpin -> setMaximum (13);

    _vertexCountLabel = new QLabel();
    _contentLayout -> addRow (tr ("Vertices"), _vertexCountLabel);
    connect (_depthSpin, SIGNAL (editingFinished()), this, SLOT (setIcosphereDepth()));
    _contentLayout -> addRow (tr ("Depth"), _depthSpin);

    _boundsLabel = new QLabel();
     _boundsButton = new QPushButton ("...");
    QHBoxLayout* boundsLayout = new QHBoxLayout (this);
    boundsLayout -> addWidget (_boundsLabel);
    boundsLayout -> addWidget (_boundsButton);
    _contentLayout -> addRow (tr ("Bounds"), boundsLayout);
    connect (_boundsButton, SIGNAL (pressed()), this, SLOT (boundsMenuRequested()));

    _boundsMenu = new QMenu (this);
    _wholeWorldBoundsAction = new QAction ("Whole world");
    _wholeWorldBoundsAction -> setEnabled (false);
    connect (_wholeWorldBoundsAction, &QAction::triggered, this, &QIcosphereMap::boundsChangeRequested);
    _displayedBoundsAction = new QAction ("Currently displayed map");
    _displayedBoundsAction -> setEnabled (false);
    connect (_displayedBoundsAction, &QAction::triggered, this, &QIcosphereMap::boundsChangeRequested);
    _boundsMenu -> addAction (_wholeWorldBoundsAction);
    _boundsMenu -> addAction (_displayedBoundsAction);

    connect (this, SIGNAL (icosphereChangeRequested()), this, SLOT (generateMap()));
    connect (this, SIGNAL (initialised()), this, SLOT (generateMap()));
    connect (_ports [0], &QNEPort::connected, this, &QIcosphereMap::generateMap);

    setBounds (_bounds);
    setIcosphereDepth (_depth);
    connect ((IcosphereModule*) _module, &IcosphereModule::available, this, &QNode::invalidate);
    emit initialised();
}

bool QIcosphereMap::generateMap() {
    //int estimate = _bounds.estimateVertexCount (_depth);

    QNEPort* port = _ports [0];
    if (! (port -> connections().isEmpty ())) {
        QNEConnection* c = port -> connections() [0];
        QNEPort* p = c -> port1 () == _ports[0] ? c -> port2() : c -> port1();
        Module* source = ((QModule*) p -> owner()) -> module();
        if (source) {
            ((IcosphereModule*) _module) -> SetSourceModule (0, *source);
            ((IcosphereModule*) _module) -> setKey (_key);
            ((IcosphereModule*) _module) -> buildIcosphere (_bounds, _depth);
            _vertexCountLabel -> setText (QString::number (((IcosphereModule*) _module) -> vertexCount ()) + " vertices generated");
            emit nodeChanged();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
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

void QIcosphereMap::boundsChangeRequested() {
    QAction* action = (QAction*) sender();
    if (action == _wholeWorldBoundsAction) {
        setBounds (Bounds());
    }
    if (action == _displayedBoundsAction) {
        setBounds (_preview -> bounds());
    }
}

void QIcosphereMap::setBounds (const icosphere::Bounds& bounds) {
    _bounds = bounds;
    emit icosphereChangeRequested();
}

bool QIcosphereMap::isComplete() {
    return QModule::isComplete() && _module != nullptr;
}

QIcosphereMap* QIcosphereMap::clone () {
    QIcosphereMap* n = (QIcosphereMap*) QNode::clone();
    n -> setIcosphereDepth (_depth);
    return n;
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
        Bounds bounds = Bounds (lat1, lat2, lon1, lon2);
        setBounds (bounds);
    }
}

void QIcosphereMap::serialize (QDomDocument& doc) {
    QModule::serialize (doc);
    _model -> writeParameter (_element, "depth", QString::number (_depth));
    QDomElement boundsElement = doc.createElement ("bounds");
    _element.appendChild (boundsElement);
    QDomElement latlonElement = doc.createElement ("geolocation");
    boundsElement.appendChild (latlonElement);
    latlonElement.setAttribute ("lat", _bounds.south());
    latlonElement.setAttribute ("_lon", _bounds.west());
    latlonElement = doc.createElement ("geolocation");
    boundsElement.appendChild (latlonElement);
    latlonElement.setAttribute ("lat", _bounds.north());
    latlonElement.setAttribute ("_lon", _bounds.east());
    boundsElement.setAttribute ("crossesDateline", _bounds.crossesDateLine() ? "y" : "n");
}

IcosphereModule* QIcosphereMap::module () {
    return (IcosphereModule*) _module;
}

QMenu* QIcosphereMap::boundsMenu () {
    return _boundsMenu;
}

void QIcosphereMap::boundsMenuRequested () {
    _boundsMenu -> show();
}
