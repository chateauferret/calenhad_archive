//
// Created by martin on 18/04/17.
//

#include <QtXml/QDomDocument>
#include "ChangeModuleCommand.h"
#include "../qmodule/QModule.h"

ChangeModuleCommand::ChangeModuleCommand (QModule* module,  const QString& property, const QVariant& oldValue, const QVariant& newValue) :
    _module (module),
    _property (property),
    _oldValue (oldValue),
    _newValue (newValue) {
    std::cout << oldValue.toString ().toStdString () << " " << newValue.toString ().toStdString () << "\n";
    setText ("Change module parameter " + property + " from " + oldValue.toString () + " to " + newValue.toString ());
}

ChangeModuleCommand::~ChangeModuleCommand () {

}

void ChangeModuleCommand::redo() {
    _module -> setProperty (_property.toStdString().c_str(), _newValue);
}

void ChangeModuleCommand::undo () {
    _module -> setProperty (_property.toStdString().c_str(), _oldValue);
}
