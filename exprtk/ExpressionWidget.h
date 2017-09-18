//
// Created by martin on 25/07/17.
//

#ifndef CALENHAD_EXPRESSIONWIDGET_H
#define CALENHAD_EXPRESSIONWIDGET_H


#include <QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QLabel>
#include <float.h>
#include "exprtk.hpp"

namespace calenhad {
    namespace qmodule {
        class ParamValidator;
    }
    namespace controls {
        class TextEdit;
    }
    namespace expressions {
        class ExpressionEdit;
        class ExpressionLineEdit;

        class ExpressionWidget : public QWidget {
            Q_OBJECT

        public:
            ExpressionWidget (QWidget* parent);

            virtual ~ExpressionWidget ();
            void setText (QString text);
            const QString& text();
            exprtk::expression<double>* expression ();
            bool isValid ();

        public slots:

            bool prepare();
            void openLongBox ();
            void editText ();
            void setValidator (calenhad::qmodule::ParamValidator* validator);

        signals:
            void compiled (const double& value);
            void expressionChanged ();
            void errorFound();

        protected:
            QCompleter* _completer;
            ExpressionLineEdit* _expressionShortBox;
            QPushButton* _longBoxButton;
            QPixmap _statusOrright, _statusGoosed,  _statusQuery;
            exprtk::expression<double>* _expression;
            QStringList _errors;
            exprtk::parser<double>* _parser;
            bool _goosed;

            void reportErrors ();

            QString _text;
            ExpressionEdit* _expressionLongBox;
            QLabel* _statusLabel;
            calenhad::qmodule::ParamValidator* _validator;

            void focusOutEvent (QFocusEvent* event) override;
        };
    }
}


#endif //CALENHAD_EXPRESSIONWIDGET_H