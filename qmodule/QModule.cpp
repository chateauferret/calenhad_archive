//
// Created by martin on 16/11/16.
//


#include "QModule.h"
#include "../nodeedit/QNodeBlock.h"
#include "../nodeedit/qneport.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../legend/LegendService.h"
#include "../legend/Legend.h"
#include "../pipeline/ModuleFactory.h"
#include <QMenu>
#include <controls/globe/CalenhadGlobeDialog.h>

using namespace icosphere;
using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::pipeline;
using namespace calenhad::legend;


int QModule::seed = 0;

QModule::QModule (const QString& nodeType, int inputs, QWidget* parent) : QNode (nodeType, inputs, parent), _globe (nullptr) {
    _legend = CalenhadServices::legends() -> defaultLegend();
    initialise();
}

QModule::~QModule () {
    if (_globe) { delete _globe; }
}

/// Initialise a QModule ready for use. Creates the UI.
void QModule::initialise() {
    QNode::initialise();

    // all modules have an output
    QNEPort* output = new QNEPort (QNEPort::OutputPort, 0, "Output");
    addPort (output);
    setContextMenuPolicy(Qt::CustomContextMenu);
    _contextMenu = new QMenu (this);
    QAction* globeAction = new QAction ("Show globe");
    connect (globeAction, &QAction::triggered, this, &QModule::showGlobe);
    _contextMenu -> addAction (globeAction);
}

void QModule::showGlobe() {
    if (!_globe) {
        _globe = new CalenhadGlobeDialog (this, this);
        _globe -> initialise ();
    }
    _globe -> show();
}

void QModule::setupPreview() {
    _preview = new CalenhadMapView (this);
    _preview -> setSource (this);
    _previewIndex = addPanel (tr ("Preview"), _preview);
    connect (_preview, &QWidget::customContextMenuRequested, this, &QModule::showContextMenu);
}

void QModule::showContextMenu (const QPoint& point) {
    _contextMenu -> exec (QCursor::pos());
}


void QModule::addInputPorts() {
    // find the input configuration from the libnoiseutils owner and create ports to match
    // Rules: if there are < 3 inputs none of them are controls,
    // if there are 3 inputs, 0 and 1 are data and 2 is control,
    // if there are > 3 inputs, 1 is data and the rest are controls
    int portType;
    QString name;

    for (int i = 0; i < _inputCount; i++) {
        if (_inputCount < 3 || i == 0 || (i == 1 && _inputCount == 3)) {
            name = "Input " + QString::number (i + 1);
            portType = QNEPort::InputPort;
        } else {
            name = "Control " + QString::number (i - 1);
            portType = QNEPort::ControlPort;
        }
        QNEPort* input = new QNEPort (portType, i, name);
        addPort (input);
    }
}

void QModule::inflate (const QDomElement& element) {
    QNode::inflate (element);
    QString legendName = element.attribute ("legend", "default");
    _legend = CalenhadServices::legends() -> find (legendName);
    // position is retrieved in CalenhadModel
}

void QModule::serialize (QDomDocument& doc) {
    QNode::serialize (doc);
    _element.setAttribute ("legend", _legend -> name());
    QDomElement positionElement = doc.createElement ("position");
    _element.appendChild (positionElement);
    positionElement.setAttribute ("y", handle() -> scenePos().y());
    positionElement.setAttribute ("x", handle() -> scenePos().x());
    _element.setAttribute ("type", nodeType ());
}

void QModule::invalidate() {
    _expander -> setItemEnabled (_previewIndex, isComplete());
    QNode::invalidate();
}

void QModule::setModel (CalenhadModel* model) {
    QNode::setModel (model);
    setupPreview();
}

void QModule::setLegend (Legend* legend) {
    _legend = legend;
    emit nodeChanged();
}

Legend* QModule::legend () {
    return _legend;
}

bool QModule::generateMap () {
    return true;
}

bool QModule::isComplete() {
    bool complete = QNode::isComplete();
    return complete;
}

void QModule::contextMenuEvent (QContextMenuEvent* e) {
    _contextMenu -> exec();
}
