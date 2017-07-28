//
// Created by martin on 21/04/17.
//

#include "EditableLabel.h"
#include <QGraphicsScene>
#include <iostream>
#include "QNodeBlock.h"

using namespace calenhad::nodeedit;

EditableLabel::EditableLabel (QGraphicsItem* parent) : QGraphicsTextItem (parent), _textEdit (nullptr), _proxy (nullptr) {
    setCursor (Qt::IBeamCursor);
}

EditableLabel::~EditableLabel() {

}

void EditableLabel::setText (const QString& text) {
    QGraphicsTextItem::setPlainText (text);
    emit textChanged (text);
}

void EditableLabel::mousePressEvent (QGraphicsSceneMouseEvent* event) {
    if (! _proxy) {
        _proxy = new QGraphicsProxyWidget (this);
    }
    if (! _textEdit) {
        _textEdit = new QLineEdit ();
        _proxy -> setWidget (_textEdit);
        connect (_textEdit, &QLineEdit::editingFinished, this, &EditableLabel::finishedEditing);
        connect (_textEdit, &QLineEdit::textChanged, this, [=] () { emit textChanged (toPlainText()); });

    }
    if (_textEdit) {
        _textEdit -> setText (toPlainText ());
        _textEdit -> selectAll ();
        _textEdit -> setFocus ();
        _textEdit -> show ();
        emit editingStateChanged (true);
    }
}

void EditableLabel::finishedEditing() {
    QString text = _textEdit -> text();
    setPlainText (text);
    _textEdit -> hide();
    emit editingStateChanged (false);
    emit textEdited (text);
}