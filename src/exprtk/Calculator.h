//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_VARIABLESSERVICE_H
#define CALENHAD_VARIABLESSERVICE_H


#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QCache>
#include "exprtk.hpp"
#include "src/Serializable.h"

namespace calenhad {
    namespace expressions {

        struct CalenhadVariable {
        public:
            CalenhadVariable (QString  name, QString  notes, const double& value);
            CalenhadVariable (const CalenhadVariable& other);
            CalenhadVariable () {}
            QString _name;
            QString _notes;
            double _value;
        };

        class Calculator : public QObject, Serializable {
            Q_OBJECT

        public:
            // these are reserved words in the exprtk expression language and can't be used as variable names
            const QStringList reservedWords = {"abs", "acos", "acosh", "and", "asin", "asinh", "atan", "atan2", "atanh", "avg",
                                               "break", "case", "ceil", "clamp", "continue", "cosh", "cos", "cot", "csc",
                                               "default", "deg2grad", "deg2rad", "else", "equal", "erfc", "erf", "exp",
                                               "expm1", "false", "floor", "for", "frac", "grad2deg", "hypot", "iclamp", "if",
                                               "ilike", "in", "inrange", "in", "like", "log", "log10", "log1p", "log2", "logn",
                                               "mand", "max", "min", "mod", "mor", "mul", "nand", "ncdf", "nor", "not",
                                               "not_equal", "not", "null", "or", "pow", "rad2deg", "repeat", "return",
                                               "root", "roundn", "round", "sec", "sgn", "shl", "shr", "sinc", "sinh", "sin",
                                               "sqrt", "sum", "swap", "switch", "tanh", "tan", "true", "trunc", "until", "var", "while", "xnor", "xor" };

            Calculator();

            virtual ~Calculator ();

            QMap<QString, CalenhadVariable*> variables ();

            QStringList errors();

            void updateVariable (const QString& name, const QString& notes, const double& value, const bool& publish = false);

            void insertVariable (QString name, const QString& notes, const double& value, const bool& publish = false);

            void deleteVariable (const QString& name);

            bool validateVariableName (const QString& name, QString& message);

            bool validateVariableValue (const QString& value, QString& message);

            void clear();

            void serialize (QDomElement& doc) override;

            void inflate (const QDomElement& element) override;

            double compute (QString expression);

            void publish();

            bool hasErrors ();
        signals:
            void variableChanged (const QString& name = QString::null, const double& value = 0);

        protected:

            QMap<QString, CalenhadVariable*> _variables;

            bool isReservedWord (const QString& term);

            QCache<QString, exprtk::expression<double>> _cache;
            QDomNode _element;
            QStringList _errors;

            exprtk::symbol_table<double> _symbols;
        };
    }
}

#endif //CALENHAD_VARIABLESSERVICE_H
