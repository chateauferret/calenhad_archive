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
#include <cfloat>
#include "exprtk.hpp"

namespace calenhad {
    namespace module {
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
            explicit ExpressionWidget (QWidget* parent = nullptr);

            ~ExpressionWidget () override;
            void setText (const QString& text);
            QString text();
            bool isValid ();
            double value();
            QString errors();
            bool hasErrors ();
        public slots:

            bool prepare();
            void openLongBox ();
            void editText ();
            void setValidator (calenhad::module::ParamValidator* validator);
            void variableChanged (const QString& name, const double& value);

        signals:
            void compiled (const double& value);
            void expressionChanged();
            void errorFound();
            void editingText();
            void editingFinished();
            void validated();

        protected:
            QCompleter* _completer;
            ExpressionLineEdit* _expressionShortBox;
            QPushButton* _longBoxButton;
            QPixmap _statusOrright, _statusGoosed,  _statusQuery;
            QStringList _errors;
            exprtk::parser<double>* _parser;
            bool _goosed;

            void reportErrors ();

            ExpressionEdit* _expressionLongBox;
            QLabel* _statusLabel;
            calenhad::module::ParamValidator* _validator;

            void focusOutEvent (QFocusEvent* event) override;

            double compute (const QString& e);

            void focusInEvent (QFocusEvent* e) override;
            double _value;

            void showEvent (QShowEvent* event) override;
        };
    }
}


#endif //CALENHAD_EXPRESSIONWIDGET_H
