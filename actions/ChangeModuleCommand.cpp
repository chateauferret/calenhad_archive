//
// Created by martin on 18/04/17.
//

#include <QtXml/QDomDocument>
#include "ChangeModuleCommand.h"
#include "qmodule/Module.h"
#include "nodeedit/Port.h"

using namespace calenhad::actions;
using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;

ChangeModuleCommand::ChangeModuleCommand (Node* node,  const QString& property, const QVariant& oldValue, const QVariant& newValue, const int& portIndex, const int& portType) :
    _node (node),
    _property (property),
    _oldValue (oldValue),
    _newValue (newValue),
    _portIndex (portIndex),
    _portType (portType) {
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
        if (_node -> property (_property.toStdString ().c_str ()) != _newValue) {
            _node->setProperty (_property.toStdString ().c_str (), _newValue);
        }
    } else {
        for (Port* port : _node -> ports()) {
            if (port->index() == _portIndex && port -> portType() == _portType) {
                if (port -> property (_property.toStdString ().c_str ()) != _newValue) {
                    port->setProperty (_property.toStdString ().c_str (), _newValue);
                }
            }
        }
    }
}

void ChangeModuleCommand::undo () {
    if (_portIndex < 0) {
        if (_node -> property (_property.toStdString ().c_str ()) != _oldValue) {
            _node->setProperty (_property.toStdString ().c_str (), _oldValue);
        }
    } else {
        for (Port* port : _node -> ports()) {
            if (port->index () == _portIndex && port -> portType() == _portType) {
                if (port -> property (_property.toStdString ().c_str ()) != _oldValue) {
                    port->setProperty (_property.toStdString ().c_str (), _oldValue);
                }
            }
        }
    }
}
