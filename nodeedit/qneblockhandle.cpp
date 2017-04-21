//2
// Created by martin on 16/10/16.
//

#include "qneblockhandle.h"
#include "../qmodule/QModule.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>

QNEBlockHandle::QNEBlockHandle (QModule* w, QGraphicsItem* parent) : QGraphicsItem (parent), _module (w), _nameEdit (nullptr), _nameProxy (nullptr) {
    setFlag (QGraphicsItem::ItemIsMovable, true);
    setFlag (QGraphicsItem::ItemIsSelectable, true);
    setCursor (Qt::OpenHandCursor);
}

QNEBlockHandle::~QNEBlockHandle () {
    if (_nameEdit) { delete _nameEdit; }
}


void QNEBlockHandle::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED (option)
    QRectF rect = QRectF (0, 0, _module -> width(), 20);
    rect.setWidth (rect.width() - 2);
    rect.moveTo (1, 1);
    QPen pen = QPen();
    QBrush brush = QBrush (CalenhadServices::preferences() -> calenhad_handle_brush_color_bg);

    painter -> setPen (pen);
    painter -> setBrush (brush);
    painter -> drawRect (rect);

    rect.setWidth (rect.width() - 4);
    rect.setHeight (rect.height() - 6);
    rect.moveTo (3, 4);
    brush = QBrush (isSelected() ? CalenhadServices::preferences() -> calenhad_handle_brush_color_selected : CalenhadServices::preferences() -> calenhad_handle_brush_color_normal);
    painter -> setBrush (brush);
    QPainterPath path = QPainterPath();
    path.addRoundedRect (rect, 4, 4);
    painter -> drawPath (path);
    pen = QPen (isSelected() ? CalenhadServices::preferences() -> calenhad_handle_text_color_selected : CalenhadServices::preferences() -> calenhad_handle_text_color_normal);
    painter -> setBrush (brush);
    rect.setWidth (rect.width() - 4);
    rect.moveTo (5, 3);
    painter -> drawText (rect, Qt::AlignLeft, _module -> name());
    if (_nameProxy) {
        _nameProxy->setPos (pos ().x () + 2, pos ().y () + 2);
    }
}

QRectF QNEBlockHandle::boundingRect () const {
    return QRectF (0, 0, _module -> width(), 20);
}

void QNEBlockHandle::mouseDoubleClickEvent (QGraphicsSceneMouseEvent* event) {
    // double click = edit module name
    editModuleName();
}

void QNEBlockHandle::mousePressEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent (event);

}

void QNEBlockHandle::mouseReleaseEvent (QGraphicsSceneMouseEvent *event) {
    setCursor (Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent (event);
}

void QNEBlockHandle::refresh () {
    update();
}

QModule* QNEBlockHandle::module () {
    return _module;
}

void QNEBlockHandle::editModuleName() {
    if (! _nameEdit) {
        createNameEditor ();
        scene() -> addItem (_nameProxy);
        _nameEdit -> setFixedHeight (16);
    }
    _nameProxy -> setPos (pos ().x () + 2, pos ().y () + 2);
    _nameEdit -> setText (_module -> name ());
    _nameEdit -> selectAll ();
    _nameEdit -> setFocus();
    _nameEdit -> show();
}

void QNEBlockHandle::createNameEditor() {
    _nameEdit = new QLineEdit();
    _nameProxy = new QGraphicsProxyWidget ();
    _nameProxy -> setWidget (_nameEdit);
    QGraphicsScene* s = scene();
    connect (_nameEdit, &QLineEdit::editingFinished, this, &QNEBlockHandle::setName);
}

void QNEBlockHandle::setName() {
    _nameEdit -> hide();
    _module -> setName (_nameEdit -> text());
}

