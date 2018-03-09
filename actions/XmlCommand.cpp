//
// Created by martin on 25/04/17.
//

#include "XmlCommand.h"
#include "qmodule/Node.h"

using namespace calenhad::actions;
using namespace calenhad::qmodule;


XmlCommand::XmlCommand (Node* node, const QString& oldXml, const QString& newXml) :
    _node (node), _oldXml (oldXml), _newXml (newXml) {
}

XmlCommand::~XmlCommand () {

}

void XmlCommand::redo () {
    // populate the new state
    QDomDocument doc;
    doc.setContent (_newXml);
    QDomElement element = doc.documentElement().firstChildElement ("model");
    QDomNodeList list = element.elementsByTagName ("module");
    for (int i = 0; i < list.size(); i++) {
        QDomNode nameNode = list.at (i).firstChildElement ("name");
        if (_node -> name() == nameNode.toElement().text()) {
            _node -> inflate (list.at (i).toElement ());
        }
    }

}

void XmlCommand::undo() {
    QDomDocument doc;
    doc.setContent (_oldXml);
    QDomElement element = doc.documentElement();
    QDomNodeList list = element.elementsByTagName ("module");
    for (int i = 0; i < list.size(); i++) {
        QDomNode nameNode = list.at (i).firstChildElement ("name");
        if (_node -> name() == nameNode.toElement().text()) {
            _node -> inflate (list.at (i).toElement ());
        }
    }
}
