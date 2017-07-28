//
// Created by martin on 05/06/17.
//

#include <CalenhadServices.h>
#include "CalculatorService.h"
#include "../preferences/PreferencesService.h"
#include "../messages/QNotificationService.h"

using namespace exprtk;
using namespace calenhad::expressions;

CalculatorService::CalculatorService () {

}

CalculatorService::~CalculatorService () {

}

QMap<QString, double> CalculatorService::variables () {
    return _variables;
}

void CalculatorService::insertVariable (const QString& name, double& value) {
    _variables.insert (name, value);
    _symbols.add_variable (name.toStdString(), _variables [name]);
}

void CalculatorService::updateVariable (const QString& name, double& value) {
    if (_variables.keys().contains (name)) {
        _variables.find (name).value() = value;
    } else {
        insertVariable (name, value);
    }
}

void CalculatorService::deleteVariable (const QString& name) {
    _variables.remove (name);
    _symbols.remove_variable (name.toStdString());
}

void CalculatorService::clear () {
    _variables.clear();
}


// Get a new expression object registered with the variable table. Caller is responsible for deleting the returned object.
expression<double>* CalculatorService::makeExpression() {
    expression<double>* exp = new expression<double>();
    exp -> register_symbol_table (_symbols);
    return exp;
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
            message += "Name is too long: maximum length " + QString::number (CalenhadServices::preferences() -> calenhad_variables_name_maxlength) + "\n";
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
        if (ok) {
            _variables.insert (name, value);
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
        itemElement.setAttribute ("name", key);
        itemElement.setAttribute ("value", _variables . value (key));
    }
}


