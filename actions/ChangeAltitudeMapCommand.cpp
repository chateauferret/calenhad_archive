//
// Created by martin on 25/04/17.
//

#include "ChangeAltitudeMapCommand.h"

ChangeAltitudeMapCommand::ChangeAltitudeMapCommand (QAltitudeMap* node, const QString& oldXml, const QString& newXml) :
    _node (node), _oldXml (oldXml), _newXml (newXml) {
}

ChangeAltitudeMapCommand::~ChangeAltitudeMapCommand () {

}

void ChangeAltitudeMapCommand::redo () {
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

void ChangeAltitudeMapCommand::undo() {
    QDomDocument doc;
    doc.setContent (_oldXml);
    std::cout << _oldXml.toStdString () << "\n";
    QDomElement element = doc.documentElement();
    QDomNodeList list = element.elementsByTagName ("module");
    for (int i = 0; i < list.size(); i++) {
        QDomNode nameNode = list.at (i).firstChildElement ("name");
        if (_node -> name() == nameNode.toElement().text()) {
            _node -> inflate (list.at (i).toElement ());
        }
    }
}
