//
// Created by martin on 05/06/17.
//

#include "../messages/QNotificationHost.h"
#include <src/CalenhadServices.h>
#include <iostream>
#include "Calculator.h"
#include "../preferences/PreferencesService.h"
#include "../messages/QNotificationHost.h"
#include <QCache>

using namespace exprtk;
using namespace calenhad::expressions;
using namespace calenhad::notification;

Calculator::Calculator() {

}

Calculator::~Calculator () {
    for (QString name : _variables.keys()) {
        deleteVariable (name);
    }
}

QMap<QString, CalenhadVariable*> Calculator::variables () {
    return _variables;
}

void Calculator::publish() {
    emit variableChanged();
}

void Calculator::insertVariable (QString name, const QString& notes, const double& value, const bool& publish) {
    CalenhadVariable* cv = new CalenhadVariable (name, notes, value);
    _variables.insert (name, cv);
    _symbols.add_variable (name.toStdString(), cv -> _value);
    if (publish) {
        emit variableChanged (name, value);
    }
}

void Calculator::updateVariable (const QString& name, const QString& notes, const double& value, const bool& publish) {
    if (_variables.keys().contains (name)) {
        _variables.find (name).value() -> _value = value;
        _variables.find (name).value() -> _notes = notes;
        if (publish) {
            emit variableChanged (name, value);
        }
    } else {
        insertVariable (name, notes, value, publish);
    }
}

void Calculator::deleteVariable (const QString& name) {
    CalenhadVariable* cv = _variables.value (name);
    _variables.remove (name);
    delete cv;
-   _symbols.remove_variable (name.toStdString());
}

void Calculator::clear () {
    _variables.clear();
    _symbols.clear();
}

bool Calculator::validateVariableName (const QString& name, QString& message) {
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

bool Calculator::validateVariableValue (const QString& value, QString& message) {
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

bool Calculator::isReservedWord (const QString& term) {
    return reservedWords.contains (term, Qt::CaseInsensitive);
}


void Calculator::inflate (const QDomElement& element) {
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
            insertVariable (name, notes, value, false);
        } else {
            CalenhadServices::messages() -> message ("Expression parse error", "Failed to parse value of variable " + name + " = " + value, NotificationStyle::WarningNotification);
        }
    }
}

void Calculator::serialize (QDomElement& element) {
    QDomDocument doc = element.ownerDocument();
    _element = doc.createElement ("variables");
    doc.documentElement().appendChild (_element);
    for (QString key : _variables.keys()) {
        QDomElement itemElement = doc.createElement ("variable");
        _element.appendChild (itemElement);
        QDomElement notesElement = doc.createElement ("notes");
        CalenhadVariable* cv = _variables.value (key);
        QDomText notesText = doc.createTextNode (cv ->_notes);
        notesElement.appendChild (notesText);
        itemElement.appendChild (notesElement);
        itemElement.setAttribute ("name", key);
        itemElement.setAttribute ("value", cv ->_value);
    }
}

QStringList Calculator::errors () {
    return _errors;
}

bool Calculator::hasErrors() {
    return ! _errors.isEmpty();
}

double Calculator::compute (const QString& expression) {
    _errors.clear ();
    if (expression.isNull()) { return 0.0; }
    if (! _cache.contains (expression)) {

        parser<double> p;
        exprtk::expression<double>* exp = new exprtk::expression<double>();
        exp -> register_symbol_table (_symbols);

        QString e;
        if (! (p.compile (expression.toStdString(), *exp))) {
            for (std::size_t i = 0; i < p.error_count(); ++i) {
                parser_error::type error = p.get_error (i);
                QString e (QString (error.mode) + "error at position " + error.token.position + ": " + QString (error.diagnostic.c_str()));
                _errors.append (e);
                std::cout << expression.toStdString() << ": Error " << e.toStdString () << "\n";
                return 0.0;
            }
        }
        _cache.insert (expression, exp);
    }
    exprtk::expression<double>* e = _cache [expression];
    return  e -> value();
}

CalenhadVariable::CalenhadVariable (const QString& name, const QString& notes, const double& value) : _name (name), _notes (notes), _value (value) {

}

CalenhadVariable::CalenhadVariable (const CalenhadVariable& other) : _name (other._name), _notes (other._notes), _value (other._value){

}


