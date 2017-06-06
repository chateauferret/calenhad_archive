//
// Created by martin on 16/11/16.
//

#include <QtWidgets/QFormLayout>
#include "QModule.h"
#include "../nodeedit/qneblock.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"

using namespace icosphere;
using namespace noise::utils;

int QModule::seed = 0;
noise::NoiseQuality QModule::noiseQuality = noise::NoiseQuality::QUALITY_STD;

QModule::QModule (noise::module::Module* m, QWidget* parent) : QNode (parent), _module (m) {
    _legend = CalenhadServices::legends() -> defaultLegend();
    for (QString s : CalenhadServices::legends() -> all().keys ()) {
        std::cout << "Legend " << s.toStdString () << "\n";
    }
    std::cout <<
    connect (this, &QNode::initialised, this, &QModule::setupPreview);
}

QModule::~QModule () {

}

void QModule::initialise() {
    QNode::initialise();

    // all modules have an output
    QNEPort* output = new QNEPort (QNEPort::OutputPort, 0, "Output");
    addPort (output);
}

void QModule::setupPreview() {
    // preview panel
    _preview = new QNoiseMapViewer (this, this);
    _preview -> setSource (this);
    _previewIndex = QNode::addPanel (tr ("Preview"), _preview);

    _preview -> initialise();
    connect (this, &QNode::nodeChanged, _preview, &CalenhadPreview::render);
}

void QModule::setHandle (QNEBlock* h) {
    _handle = h;
}

QNEBlock* QModule::handle() {
    return _handle;
}

std::shared_ptr<QImage> QModule::overview() {
    return _preview -> image();
}

noise::module::Module* QModule::module () {
    return _module;
}

void QModule::addInputPorts() {
    // find the input configuration from the libnoiseutils owner and create ports to match
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
        QNEPort* input = new QNEPort (portType, i, name);
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

void QModule::setLegend (Legend* legend) {
    _legend = legend;
    emit nodeChanged();
}

Legend* QModule::legend () {
    return _legend;
}

