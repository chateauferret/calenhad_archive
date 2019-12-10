//
// Created by martin on 21/04/17.
//

#include "EditableLabel.h"
#include <QGraphicsScene>
#include <iostream>
#include <src/CalenhadServices.h>
#include "NodeBlock.h"
#include "NodeNameValidator.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::nodeedit;

EditableLabel::EditableLabel (QGraphicsItem* parent) : QGraphicsTextItem (parent), _textEdit (nullptr), _proxy (nullptr), _alignment (Qt::AlignLeft) {
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
    if (textEdit()) {
        textEdit() -> setValidator (v);
    }
}

void EditableLabel::setText (const QString& text) {
    QGraphicsTextItem::setPlainText (text);
    setAlignment (_alignment);
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
    QLineEdit* te = textEdit();
    if (! te) {
        te = new QLineEdit ();
        _textEdit = te;
        te -> setAlignment (_alignment);
        _proxy -> setWidget (te);
        connect (te, &QLineEdit::editingFinished, this, &EditableLabel::finishedEditing);
        te -> setValidator (_validator);
        if (_alignment == Qt::AlignRight) {
            te -> move (- (te -> width() - boundingRect ().width () + 4) , 0);
        }
    }


    if (te) {
        te -> setText (toPlainText ());
        te -> selectAll ();
        te -> setFocus ();
        te -> show ();
        emit editingStateChanged (true);
    }
}

void EditableLabel::finishedEditing() {
    QString text = textEdit() -> text();
    setPlainText (text);
    _textEdit -> hide();

    emit editingStateChanged (false);
    emit textEdited (text);
    emit textChanged (text);
    setAlignment (_alignment);
}

QString EditableLabel::proposedText () {
    return textEdit() -> text();
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

Qt::AlignmentFlag EditableLabel::alignment() {
    return _alignment;
}

void EditableLabel::setAlignment (Qt::AlignmentFlag flag) {
    _alignment = flag;
    if (_textEdit) {
        textEdit() -> setAlignment (flag);
    }
}

QLineEdit* EditableLabel::textEdit () {
    return (QLineEdit*) _textEdit;
}
