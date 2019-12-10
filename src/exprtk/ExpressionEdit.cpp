//
// Created by martin on 26/07/17.
//

#include <QtCore/QAbstractItemModel>
#include <QtCore/QStringListModel>
#include "src/CalenhadServices.h"
#include "Calculator.h"
#include "ExpressionEdit.h"
#include <QCompleter>
#include <iostream>


using namespace calenhad::expressions;
using namespace calenhad::controls;

// never set a parent, because the text box always appears free
ExpressionEdit::ExpressionEdit (QLineEdit* shortBox) : TextEdit(), _shortBox (shortBox) {

}

ExpressionEdit::~ExpressionEdit () {

}

// when the editor is shown we set focus on it and put the text cursor in the same place as in the short editor
void ExpressionEdit::showEvent (QShowEvent* e) {
    setFocus();
    int position = _shortBox -> cursorPosition ();
    QTextCursor c = textCursor();
    c.setPosition (position);
    setTextCursor (c);
}

// when the editor loses focus we dismiss it and send the text off in a signal
void ExpressionEdit::focusOutEvent (QFocusEvent* e) {
    emit dismissed (toPlainText ());
}

ExpressionLineEdit::ExpressionLineEdit (QWidget* parent) : QLineEdit (parent) { }

void ExpressionLineEdit::focusInEvent (QFocusEvent* e) {
    // update the completer with the current variables list
    QStringList words = CalenhadServices::calculator() -> reservedWords;
    words.append (CalenhadServices::calculator() -> variables().keys());
    words.sort (Qt::CaseInsensitive);
    QAbstractItemModel* model = new QStringListModel (words, completer());
    completer() -> setModel (model);
    QLineEdit::focusInEvent (e);
}
