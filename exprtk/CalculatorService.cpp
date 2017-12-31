//
// Created by martin on 05/06/17.
//

#include <CalenhadServices.h>
#include <iostream>
#include "CalculatorService.h"
#include "../preferences/PreferencesService.h"
#include "../messages/QNotificationHost.h"
#include <vector>

using namespace exprtk;
using namespace calenhad::expressions;

CalculatorService::CalculatorService ()  {

}

CalculatorService::~CalculatorService () {
}

QMap<QString, CalenhadVariable> CalculatorService::variables () {
    return _variables;
}

void CalculatorService::publish() {
    emit variableChanged();
}

void CalculatorService::insertVariable (QString name, const QString& notes, const double& value, const bool& publish) {
    CalenhadVariable cv (name, notes, value);
    _variables.insert (name, cv);
    if (publish) {
        emit variableChanged (name, value);
    }
}

void CalculatorService::updateVariable (const QString& name, const QString& notes, const double& value, const bool& publish) {
    if (_variables.keys().contains (name)) {
        _variables.find (name).value()._value = value;
        _variables.find (name).value()._notes = notes;
        if (publish) {
            emit variableChanged (name, value);
        }
    } else {
        insertVariable (name, notes, value, publish);
    }
}

void CalculatorService::deleteVariable (const QString& name) {
    _variables.remove (name);
}

void CalculatorService::clear () {
    _variables.clear();
}

bool CalculatorService::validateVariableName (const QString& name, QString& message) {
    bool result = true;
    // name is required
    if (name.isNull () || name.isEmpty ()) {
        result = false;
        message += "Name is required\n";
    } else {

        // name must not contain whitespace or other bullshit
        QRegExp rx ("^[a-zA-Z_\\.$][a-zA-Z_\\.$0-9]*$");
        int n = rx.indexIn (name);
        if (n == -1) {
            result = false;
            message += "Name contains bollocks characters\n";
        }

        // name must start with a letter
        if (!name.at (0).isLetter()) {
            result = false;
            message += "Name does not start with a letter\n";
        }

        // name must not be stupidly long
        if (name.length () > CalenhadServices::preferences() -> calenhad_variables_name_maxlength) {
            result = false;
            message += "Name is too long: maximum magnitude " + QString::number (CalenhadServices::preferences() -> calenhad_variables_name_maxlength) + "\n";
        }

        // name must not be a reserved word
        if (isReservedWord (name)) {
            result = false;
            message += "Name cannot be a reserved word\n";
        }
    }
    return result;
}

bool CalculatorService::validateVariableValue (const QString& value, QString& message) {
    bool result = true;
    // value is required
    if (value.isNull () || value.isEmpty ()) {
        result = false;
        message += "Name is required\n";
    } else {
        bool ok = true;
        double v = value.toDouble (&ok);
        if (!ok) {
            result = false;
            message += "Value is not a number\n";
        }
    }
    return result;
}

bool CalculatorService::isReservedWord (const QString& term) {
    return reservedWords.contains (term, Qt::CaseInsensitive);
}


void CalculatorService::inflate (const QDomElement& element) {
    _variables.clear();
    _element = element;
    QDomNodeList items = ((QDomElement) element).elementsByTagName ("variable");
    for (int i = 0; i < items.length(); i++) {
        bool ok;
        QString name = items.at (i).attributes().namedItem ("name").nodeValue();
        double value = items.at (i).attributes().namedItem ("value").nodeValue().toDouble (&ok);
        QDomNode notesNode = items.at (i).firstChildElement ("notes");
        QString notes = notesNode.firstChild().toText().nodeValue ();
        if (ok) {
            _variables.insert (name, CalenhadVariable (name, notes, value));
            emit variableChanged (name, value);
        } else {
            CalenhadServices::messages() -> message ("Error", "Failed to parse value of variable " + name + " = " + value);
        }
    }
}

void CalculatorService::serialize (QDomDocument& doc) {
    _element = doc.createElement ("variables");
    doc.documentElement().appendChild (_element);
    for (QString key : _variables.keys()) {
        QDomElement itemElement = doc.createElement ("variable");
        _element.appendChild (itemElement);
        QDomElement notesElement = doc.createElement ("notes");
        CalenhadVariable cv (_variables.value (key));
        QDomText notesText = doc.createTextNode (cv._notes);
        notesElement.appendChild (notesText);
        itemElement.appendChild (notesElement);
        itemElement.setAttribute ("name", key);
        itemElement.setAttribute ("value", cv._value);
    }
}

QStringList CalculatorService::errors () {
    return _errors;
}


CalenhadVariable::CalenhadVariable (const QString& name, const QString& notes, const double& value) : _name (name), _notes (notes), _value (value) {

}

CalenhadVariable::CalenhadVariable (const CalenhadVariable& other) : _name (other._name), _notes (other._notes), _value (other._value){

}
