//
// Created by ferret on 01/02/2021.
//

#include <QWidget>
#include <QStyleOptionViewItem>
#include <QSpinBox>
#include "SpinBoxDelegate.h"

using namespace calenhad::controls;


SpinBoxDelegate::SpinBoxDelegate (QObject* parent) : QStyledItemDelegate (parent) {

}

SpinBoxDelegate::~SpinBoxDelegate() {

}

QWidget* SpinBoxDelegate::createEditor (QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QSpinBox *editor = new QSpinBox(parent);
    editor -> setFrame (false);
    editor -> setMinimum (0);
    editor -> setMaximum (100);
    return editor;
}


void SpinBoxDelegate::setEditorData (QWidget *editor, const QModelIndex &index) const
{
    int value = index.model() -> data(index, Qt::EditRole).toInt();

    QSpinBox* spinBox = dynamic_cast<QSpinBox*> (editor);
    if (spinBox) {
        spinBox -> setValue (value);
    }
}


void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QSpinBox* spinBox = dynamic_cast<QSpinBox*> (editor);
    if (spinBox) {
        spinBox -> interpretText();
        int value = spinBox -> value();
        model -> setData (index, value, Qt::EditRole);
    }
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const {
    editor -> setGeometry (option.rect);
}
