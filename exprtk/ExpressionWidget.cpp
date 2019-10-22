//
// Created by martin on 25/07/17.
//

#include "ExpressionWidget.h"
#include "../CalenhadServices.h"
#include "Calculator.h"
#include <QtWidgets/QHBoxLayout>
#include "../module/ParamValidator.h"
#include <iostream>
#include <QtCore/QEvent>
#include <QKeyEvent>
#include <QtWidgets/QCompleter>
#include <QtCore/QStringListModel>
#include "ExpressionEdit.h"

using namespace exprtk;
using namespace calenhad;
using namespace calenhad::expressions;
using namespace calenhad::module;
using namespace calenhad::controls;

ExpressionWidget::ExpressionWidget (QWidget* parent) : QWidget (parent), _parser (new parser<double>()), _goosed (false), _validator (new AcceptAnyRubbish()), _value (0.0) {
    QLayout* l = new QHBoxLayout();
    l -> setContentsMargins (0, 0, 0, 0);
    setLayout (l);
    setFocusPolicy (Qt::StrongFocus);
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
    _completer -> setModelSorting (QCompleter::CaseInsensitivelySortedModel);
    _completer -> setCaseSensitivity (Qt::CaseInsensitive);
    _completer -> setWrapAround (false);
    _expressionShortBox -> setCompleter (_completer);
    _expressionLongBox -> setCompleter (_completer);
    setFocusProxy (_expressionShortBox);
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

    connect (CalenhadServices::calculator(), &Calculator::variableChanged, this, &ExpressionWidget::variableChanged);
    layout() -> setMargin (0);
    setContentsMargins (0, 0, 0, 0);
    adjustSize();
}


ExpressionWidget::~ExpressionWidget() {
    delete _parser;
    delete _completer;
    delete _validator;
}

void ExpressionWidget::variableChanged (const QString& name, const double& value) {
    QString text = _expressionShortBox -> text();
    if (text == QString::null || text.contains (name)) {
        prepare();
    }
}

bool ExpressionWidget::isValid() {
    return ! _goosed;
}

void ExpressionWidget::editText() {

    if (sender() == _expressionShortBox) {
        // If text in the short box is close to its capacity, bring up the long box

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
    _expressionLongBox -> setText (_expressionShortBox->text ());
    _expressionLongBox -> move (mapToGlobal (_expressionShortBox->pos ()));
    _expressionLongBox -> setWindowFlags (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    if (isVisible()) {
        _expressionLongBox -> setFocus ();
        _expressionLongBox -> show ();
    }
}

double ExpressionWidget::compute (const QString& e) {
    return CalenhadServices::calculator() -> compute (e);
}


bool ExpressionWidget::hasErrors() {
    return ! _errors.isEmpty();
}

bool ExpressionWidget::prepare() {
    _errors.clear();
    QString text = _expressionShortBox -> text();
    double v = compute (text);
    _errors.append (CalenhadServices::calculator() -> errors());
    if (hasErrors()) {
        _goosed = true;
        _statusLabel -> setPixmap (_statusGoosed);
         reportErrors ();
         emit errorFound();
    } else {
        if (! (_validator -> isInValidSet (v))) {
            _errors.append (_validator -> toString (v));
            _goosed = true;
            _statusLabel -> setPixmap (_statusGoosed);
            reportErrors ();
            emit errorFound();
        } else {
            _goosed = false;
            if (_validator -> isInBestSet (v)) {
                _statusLabel -> setPixmap (_statusOrright);
                setToolTip (text + ": " + QString::number (v));
            } else {
                _statusLabel -> setPixmap (_statusQuery);
                setToolTip (_validator -> toString (v));
            }
            _value = v;
            emit compiled (v);
        }
    }
    emit expressionChanged();
    return ! _goosed;
}

double ExpressionWidget::value() {
    return _value;
}

void ExpressionWidget::reportErrors () {
    setToolTip (errors());
}

void ExpressionWidget::setText (const QString& text) {
    if (_expressionShortBox -> text() != text) {
        _expressionShortBox -> setText (text);
    }
    prepare();
}

QString ExpressionWidget::text () {
    return _expressionShortBox -> text();
}

void ExpressionWidget::setValidator (calenhad::module::ParamValidator* validator) {
    delete _validator;
    _validator = validator;
}

void ExpressionWidget::showEvent (QShowEvent* event) {
    prepare();
    QWidget::showEvent (event);
}

void ExpressionWidget::focusOutEvent (QFocusEvent* event) {
    prepare();
    QWidget::focusOutEvent (event);
}

void ExpressionWidget::focusInEvent (QFocusEvent* event) {
    emit editingText();
    QWidget::focusInEvent (event);
    _expressionShortBox -> selectAll();
}

QString ExpressionWidget::errors () {
    QString report = "";
    for (QString error : _errors) {
        report += error + "\n";
    }

    return report;
}
