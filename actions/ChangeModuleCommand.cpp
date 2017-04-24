//
// Created by martin on 18/04/17.
//

#include <QtXml/QDomDocument>
#include "ChangeModuleCommand.h"
#include "../qmodule/QModule.h"

ChangeModuleCommand::ChangeModuleCommand (QNode* node,  const QString& property, const QVariant& oldValue, const QVariant& newValue, const int& portIndex, const int& portType) :
    _node (node),
    _property (property),
    _oldValue (oldValue),
    _newValue (newValue),
    _portIndex (portIndex),
    _portType (portType) {
    std::cout << oldValue.toString ().toStdString () << " " << newValue.toString ().toStdString () << "\n";
    if (portIndex < 0) {
        setText ("Change parameter " + property + " from " + oldValue.toString () + " to " + newValue.toString ());
    } else {
        setText ("Change port property " + property + " from " + oldValue.toString () + " to " + newValue.toString ());
    }
}

ChangeModuleCommand::~ChangeModuleCommand () {

}

void ChangeModuleCommand::redo() {
    if (_portIndex < 0) {
        _node -> setProperty (_property.toStdString ().c_str (), _newValue);
    } else {
        for (QNEPort* port : _node -> ports()) {
            std::cout << "Port " << port -> index () << " : " << _portIndex << " type " << port -> type () << " : " << _portType << "\n";
            if (port->index() == _portIndex && port -> portType() == _portType) {
                port->setProperty (_property.toStdString ().c_str (), _newValue);
            }
        }
    }
    //std::cout << "Redo " << text().toStdString () << "\n";
}

void ChangeModuleCommand::undo () {
    if (_portIndex < 0) {
        _node -> setProperty (_property.toStdString().c_str(), _oldValue);
    } else {
        for (QNEPort* port : _node -> ports()) {

            if (port->index () == _portIndex && port -> portType() == _portType) {
                port->setProperty (_property.toStdString ().c_str (), _oldValue);
            }
        }
    }
    //std::cout << "Undo " << text().toStdString () << "\n";
}
