//
// Created by martin on 25/07/17.
//

#include "ExpressionWidget.h"
#include "../CalenhadServices.h"
#include "CalculatorService.h"
#include <QtWidgets/QHBoxLayout>
#include "../qmodule/ParamValidator.h"
#include <iostream>
#include <QtCore/QEvent>
#include <QKeyEvent>
#include <QtWidgets/QCompleter>
#include <QtCore/QStringListModel>
#include "ExpressionEdit.h"

using namespace exprtk;
using namespace calenhad;
using namespace calenhad::expressions;
using namespace calenhad::qmodule;
using namespace calenhad::controls;

ExpressionWidget::ExpressionWidget (QWidget* parent) : QWidget (parent), _parser (new parser<double>()), _goosed (false), _validator (nullptr) {
    QLayout* l = new QHBoxLayout();
    l -> setContentsMargins (5, 0, 5, 0);
    setLayout (l);

    _statusLabel = new QLabel (this);

    _statusOrright = QPixmap (":/appicons/status/orright.png");
    _statusGoosed = QPixmap (":/appicons/status/goosed.png");
    _statusQuery = QPixmap (":/appicons/status/query.png");
    _statusLabel -> setPixmap (_statusOrright);

    _expressionShortBox = new ExpressionLineEdit (this);
    _expressionShortBox -> setText ("0.0");
    QFontMetrics m (_expressionShortBox -> font()) ;
    int rowHeight = m.lineSpacing() ;
    _expressionShortBox -> setFixedHeight  (rowHeight + 6) ;
    _expressionLongBox = new ExpressionEdit (_expressionShortBox);
    _expressionLongBox -> setText ("0.0");

    _longBoxButton = new QPushButton (this);
    _longBoxButton -> setText ("...");


    _completer = new QCompleter (this);
    _completer -> setCompletionMode (QCompleter::PopupCompletion);
    _completer->setModelSorting (QCompleter::CaseInsensitivelySortedModel);
    _completer->setCaseSensitivity (Qt::CaseInsensitive);
    _completer->setWrapAround  (false);
    _expressionShortBox->setCompleter (_completer);
    _expressionLongBox->setCompleter (_completer);
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

    layout() -> addWidget (_statusLabel);
    layout() -> addWidget (_expressionShortBox);
    _longBoxButton -> setFixedWidth (_longBoxButton -> height());
    layout() -> addWidget (_longBoxButton);

    connect (CalenhadServices::calculator(), &CalculatorService::variableChanged, this, &ExpressionWidget::variableChanged);
}


ExpressionWidget::~ExpressionWidget() {
    delete _parser;
    delete _completer;
    if (_validator) { delete _validator; }
}

void ExpressionWidget::variableChanged (const QString& name, const double& value) {
    if (_text.contains (name)) {
        prepare();
    }
}

bool ExpressionWidget::isValid() {
    return ! _goosed;
}

void ExpressionWidget::editText() {

    if (sender() == _expressionShortBox) {
        // If text in the short box is close to its capacity, bring up the long box
        _text = _expressionShortBox->text ();
        if (!(_expressionLongBox->isVisible ())) {
            if (_expressionLongBox->toPlainText () != _expressionShortBox->text ()) {
                QFont f = _expressionShortBox->font ();
                QFontMetrics fm (f);
                int textWidth = fm.width (_expressionShortBox->text ());
                if (textWidth > _expressionShortBox->width () * 0.9) {
                    openLongBox ();
                }
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

bool ExpressionWidget::prepare() {
    QString text = _expressionShortBox -> text();
    _expression = CalenhadServices::calculator() -> makeExpression (text);
    if (! _expression) {
        _goosed = true;
        _statusLabel -> setPixmap (_statusGoosed);
        _errors.append (CalenhadServices::calculator() -> errors());
         reportErrors ();
         emit errorFound();
    } else {
        double v = _expression -> value();
        if (! (_validator -> isInValidSet (v))) {
            _errors.append (_validator->toString (v));
            _goosed = true;
            _statusLabel->setPixmap (_statusGoosed);
            reportErrors ();
            emit errorFound();
        } else {

            _goosed = false;
            if (_validator -> isInBestSet (v)) {
                _statusLabel->setPixmap (_statusOrright);
                setToolTip (QString::number (v));
            } else {
                _statusLabel->setPixmap (_statusQuery);
                setToolTip (_validator->toString (v));
            }

            emit compiled (v);
        }
    }
    emit expressionChanged();
    return ! _goosed;
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
    _expressionShortBox -> setText (text);
    prepare();

}

const QString& ExpressionWidget::text () {
    return _text;
}

void ExpressionWidget::setValidator (calenhad::qmodule::ParamValidator* validator) {
    if (_validator) { delete _validator; }
    _validator = validator;
}

void ExpressionWidget::focusOutEvent (QFocusEvent* event) {
    prepare();
}