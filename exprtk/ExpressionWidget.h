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
#include "exprtk.hpp"

namespace calenhad {
    namespace expressions {
        class ExpressionEdit;

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

            bool prepare (const QString& text);
            void openLongBox ();
            void editText ();

        signals:
            void compiled (const double& value);
            void expressionChanged ();
            void errorFound();

        protected:
            QLineEdit* _expressionShortBox;
            QPushButton* _longBoxButton, * _variablesButton;
            QPixmap _statusOrright, _statusGoosed;
            exprtk::expression<double>* _expression;
            QStringList _errors;
            exprtk::parser<double>* _parser;
            bool _goosed;

            void reportErrors ();

            QString _text;
            ExpressionEdit* _expressionLongBox;
            QLabel* _statusLabel;


        };
    }
}


#endif //CALENHAD_EXPRESSIONWIDGET_H
