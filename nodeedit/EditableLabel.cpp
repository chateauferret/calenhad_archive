//
// Created by martin on 21/04/17.
//

#include "EditableLabel.h"
#include <QGraphicsScene>
#include <iostream>
#include <CalenhadServices.h>
#include "QNodeBlock.h"
#include "NodeNameValidator.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::nodeedit;

EditableLabel::EditableLabel (QGraphicsItem* parent) : QGraphicsTextItem (parent), _textEdit (nullptr), _proxy (nullptr) {
    setCursor (Qt::IBeamCursor);
    _palette = new QPalette();
}

EditableLabel::~EditableLabel() {
    delete _palette;
}

void EditableLabel::setValidator (NodeNameValidator* v) {
    _validator = v;
    connect (_validator, &NodeNameValidator::message, this, &EditableLabel::showMessage);
    connect (_validator, &NodeNameValidator::success, this, &EditableLabel::clearMessage);
    if (_textEdit) {
        _textEdit -> setValidator (v);
    }
}

void EditableLabel::setText (const QString& text) {
    QGraphicsTextItem::setPlainText (text);
    emit textChanged (text);
}

void EditableLabel::setTextColor (const QColor& color) {
    setDefaultTextColor (color);
    _palette -> setColor (QPalette::Text, color);
    if (_textEdit) {
        _textEdit->setPalette (*_palette);
    }

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
        _textEdit -> setValidator (_validator);
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

QString EditableLabel::proposedText () {
    return _textEdit -> text();
}


void EditableLabel::showMessage (const QString& message) {
    if (_textEdit) {
        _textEdit -> setToolTip (message);
        setTextColor (CalenhadServices::preferences() -> calenhad_module_text_color_error);
    }
}

void EditableLabel::clearMessage() {
    if (_textEdit) {
        _textEdit -> setToolTip (QString::null);
        setTextColor (CalenhadServices::preferences() -> calenhad_module_text_color_normal);
    }
}