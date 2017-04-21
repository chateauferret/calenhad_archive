//
// Created by martin on 16/11/16.
//

#include <QtWidgets/QFormLayout>
#include "QModule.h"
#include "../nodeedit/qneblock.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "../libnoiseutils/GradientLegend.h"

using namespace icosphere;
using namespace noise::utils;

int QModule::seed = 0;
noise::NoiseQuality QModule::noiseQuality = noise::NoiseQuality::QUALITY_STD;

QModule::QModule (noise::module::Module* m, QWidget* parent) : QNode (parent), _module (m) {

    // for noe
    GradientLegend* gl = new GradientLegend ("default");
    _legend = gl;

}

QModule::~QModule () {

}

void QModule::initialise() {
    QNode::initialise();

    // all modules have an output
    QNEPort* output = new QNEPort (QNEPort::OutputPort, 0, "Output");
    addPort (output);

    // preview panel

    _previewLayout = new QFormLayout();
    _preview = new QNoiseMapViewer();

    _preview -> setLayout (_previewLayout);
    _preview -> setSource (this);
    _previewIndex = QNode::addPanel (tr ("Preview"), _preview);

    _preview -> initialise();
    connect (this, &QNode::nodeChanged, _preview, &QNoiseMapViewer::render);
}


void QModule::changeBounds (const GeoDataLatLonBox& bounds) {
    _preview->setBounds (bounds);
}

void QModule::setHandle (QNEBlock* h) {
    _handle = h;
}

QNEBlock* QModule::handle() {
    return _handle;
}


noise::module::Module* QModule::module () {
    return _module;
}

void QModule::addInputPorts() {
    // find the input configuration from the libnoiseutils module and create ports to match
    // Rules: if there are < 3 inputs none of them are controls,
    // if there are 3 inputs, 0 and 1 are data and 2 is control,
    // if there are > 3 inputs, 1 is data and the rest are controls
    int portType;
    QString name;
    int inputs = _module -> GetSourceModuleCount();
    for (int i = 0; i < inputs; i++) {
        if (inputs < 3 || i == 0 || (i == 1 && inputs == 3)) {
            name = "Input " + QString::number (i + 1);
            portType = QNEPort::InputPort;
        } else {
            name = "Control " + QString::number (i - 1);
            portType = QNEPort::ControlPort;
        }
        QNEPort* input = new QNEPort (portType, 0, name);
        addPort (input);
    }
}


void QModule::setUniqueName() {
    int i = 0;
    QString type = moduleType ();
    QString name = "New " + moduleType ();

    while (_model -> findModule (name)) {
        i++;
        name = QString ("New ") + moduleType () + " " + QString::number (i);
    }
    setName (name);
}

void QModule::inflate (const QDomElement& element) {
    QNode::inflate (element);
    // position is retrieved in CalenhadModel
}

void QModule::serialise (QDomDocument& doc) {
    QNode::serialise (doc);
    QDomElement positionElement = doc.createElement ("position");
    _element.appendChild (positionElement);
    positionElement.setAttribute ("y", handle() -> scenePos().y());
    positionElement.setAttribute ("x", handle() -> scenePos().x());
    _element.setAttribute ("type", moduleType());

}

void QModule::invalidate() {
    QNode::invalidate();
}

void QModule::setModel (CalenhadModel* model) {
    QNode::setModel (model);
}

void QModule::setLegend (icosphere::Legend* legend) {
    _legend = legend;
    emit nodeChanged ("legend", 0);
}

icosphere::Legend* QModule::legend () {
    return _legend;
}
