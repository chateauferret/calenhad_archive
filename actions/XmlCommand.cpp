//
// Created by martin on 25/04/17.
//

#include "XmlCommand.h"

#include <utility>
#include "module/Node.h"

using namespace calenhad::actions;
using namespace calenhad::module;
using namespace calenhad::pipeline;


XmlCommand::XmlCommand (CalenhadModel* model, QString  oldXml, QString  newXml) :
    _model (model), _oldXml (std::move(oldXml)), _newXml (std::move(newXml)) {
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
