//
// Created by martin on 25/07/17.
//

#include "ExpressionWidget.h"
#include "../CalenhadServices.h"
#include "CalculatorService.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <iostream>
#include "ExpressionEdit.h"

using namespace exprtk;
using namespace calenhad;
using namespace calenhad::expressions;

ExpressionWidget::ExpressionWidget (QWidget* parent) : QWidget (parent), _parser (new parser<double>()), _goosed (false) {
    setLayout (new QHBoxLayout());
    _statusLabel = new QLabel (this);

    _statusOrright = QPixmap (":/appicons/status/orright.png");
    _statusGoosed = QPixmap (":/appicons/status/goosed.png");
    _statusLabel -> setPixmap (_statusOrright);

    _expressionShortBox = new QLineEdit (this);
    _expressionShortBox -> setText ("0.0");

    _expressionLongBox = new ExpressionEdit (_expressionShortBox);
    _expressionLongBox -> setText ("0.0");

    _longBoxButton = new QPushButton (this);
    _longBoxButton -> setText ("...");

    connect (_longBoxButton, &QPushButton::pressed, this, &ExpressionWidget::openLongBox);
    connect (_expressionShortBox, &QLineEdit::textChanged, this, &ExpressionWidget::editText);

    // whenever the text of the expression is changed, try to recompile it
    connect (_expressionShortBox, &QLineEdit::textChanged, this, &ExpressionWidget::prepare);

    // if the _expressionLongBox is finished with, copy its text back and dismiss it
    connect (_expressionLongBox, &ExpressionEdit::dismissed, this, [=] () {
        _expressionShortBox -> setText (_expressionLongBox -> toPlainText());
        int c = _expressionLongBox -> textCursor().position();
        _expressionShortBox -> setCursorPosition (c);
        _expressionLongBox -> hide();
    });


    prepare (_expressionShortBox -> text());

    layout() -> addWidget (_statusLabel);
    layout() -> addWidget (_expressionShortBox);
    _longBoxButton -> setFixedWidth (_longBoxButton -> height());
    layout() -> addWidget (_longBoxButton);
}


ExpressionWidget::~ExpressionWidget() {
    delete _parser;
}

bool ExpressionWidget::isValid() {
    return ! _goosed;
}

void ExpressionWidget::editText() {
    _text = _expressionShortBox -> text();
    if (! (_expressionLongBox -> isVisible())) {
        if (_expressionLongBox->toPlainText () != _expressionShortBox->text ()) {
            QFont f = _expressionShortBox->font ();
            QFontMetrics fm (f);
            int textWidth = fm.width (_expressionShortBox->text ());
            if (textWidth > _expressionShortBox->width () * 0.9) {
                openLongBox();
            }
        }
    }
}

void ExpressionWidget::openLongBox() {
    // move editing to _expressionLongBox when the smaller box is nearly full
    _expressionLongBox -> setText (_expressionShortBox -> text());
    _expressionLongBox -> move (mapToGlobal (_expressionShortBox -> pos()));
    _expressionLongBox -> setWindowFlags (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    _expressionLongBox -> show();
}

bool ExpressionWidget::prepare (const QString& text) {
    _errors.clear();
    _expression = CalenhadServices::calculator() -> makeExpression();
    const std::string s = text.toStdString();
    bool ok = _parser -> compile (s, *_expression);
    if (! ok) {
        for (std::size_t i = 0; i < _parser -> error_count(); ++i)         {
            exprtk::parser_error::type error = _parser -> get_error (i);
            _errors.append (QString (error.mode) + "error at position " + error.token.position + ": " + QString (error.diagnostic.c_str ()));
            _goosed = true;
            _statusLabel -> setPixmap (_statusGoosed);
            reportErrors();
            emit errorFound();
        }
    } else {
        setToolTip (QString::number (_expression -> value()));
        _goosed = false;
        _statusLabel -> setPixmap (_statusOrright);
        emit compiled (_expression -> value());
    }
    emit expressionChanged();
    return ok;
}

exprtk::expression<double>* ExpressionWidget::expression () {
    return _expression;
}

void ExpressionWidget::reportErrors () {
    QString report = "";
    for (QString error : _errors) {
        report += error + "\n";
    }
    setToolTip (report);
}

void ExpressionWidget::setText (QString text) {
    std::cout << text.toStdString () << "\n";
    _expressionShortBox -> setText (text);
}

const QString& ExpressionWidget::text () {
    return _text;
}
