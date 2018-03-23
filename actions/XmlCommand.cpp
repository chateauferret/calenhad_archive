//
// Created by martin on 25/04/17.
//

#include "XmlCommand.h"
#include "qmodule/Node.h"

using namespace calenhad::actions;
using namespace calenhad::qmodule;
using namespace calenhad::pipeline;


XmlCommand::XmlCommand (CalenhadModel* model, const QString& oldXml, const QString& newXml) :
    _model (model), _oldXml (oldXml), _newXml (newXml) {
}

XmlCommand::~XmlCommand () {

}

void XmlCommand::redo () {
    // populate the new state
    _model -> restore (_newXml);

}

void XmlCommand::undo() {
    _model -> restore (_oldXml);
}

void XmlCommand::setNewXml (const QString& xml) {
    _newXml = xml;
}
