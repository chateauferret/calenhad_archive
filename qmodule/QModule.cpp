//
// Created by martin on 16/11/16.
//

#include <QtWidgets/QFormLayout>
#include "QModule.h"
#include "../nodeedit/qnemainwindow.h"
#include "../pipeline/CalenhadModel.h"

int QModule::seed = 0;
noise::NoiseQuality QModule::noiseQuality = noise::NoiseQuality::QUALITY_STD;

QModule::QModule (noise::module::Module* m, QWidget* parent) : QNode (parent), _module (m) {
    _id = QUuid::createUuid ();
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
    int index = QNode::addPanel (tr ("Preview"), _preview);
    connect (this, &QNode::nodeChanged, this, [=] () { _expander -> setItemEnabled (index, isRenderable ()); });
    connect (this, SIGNAL (nodeChanged (const char*, QVariant)), _preview, SLOT (render()));
    _preview -> initialise();
    setEnabled (false);
}

Module* QModule::module () {
    return _module;
}

void QModule::addInputPorts() {
    // find the input configuration from the libnoiseutils module and create ports to match
    // Rules: if there are < 3 inputs none of them are controls,
    // if there are 3 inputs, 0 and 1 are data and 2 is control,
    // if there are > 3 inputs, 1 is data and the rest are controls
    int inputs = _module -> GetSourceModuleCount();
    for (int i = 0; i < inputs; i++) {

        QString name = "";
        int portType;
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

void QModule::invalidate() {
    setEnabled (true);
    emit (nodeChanged ("inputs", 0));
}

void QModule::setUniqueName() {
    int i = 0;
    QString name = "New " + typeString();
    while (_model -> findModule (name)) {
        i++;
        name = QString ("New ") + typeString() + " " + QString::number (i);
    }
    setName (name);
    _nameEdit -> setText (_name);
}

QUuid QModule::id () {
    return _id;
}


