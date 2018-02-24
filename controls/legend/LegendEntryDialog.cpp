//
// Created by martin on 19/02/18.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QColorDialog>
#include <legend/QColorButton.h>
#include <QtWidgets/QDialogButtonBox>
#include <CalenhadServices.h>
#include <exprtk/Calculator.h>
#include "LegendEntryDialog.h"
#include "../../exprtk/ExpressionWidget.h"

using namespace calenhad::controls::legend;
using namespace calenhad::expressions;

LegendEntryDialog::LegendEntryDialog (const bool& canDelete, QWidget* parent) {
    QWidget* widget = new QWidget (this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect (buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    QFormLayout* layout = new QFormLayout (this);
    widget -> setLayout (layout);
    _indexEdit = new ExpressionWidget (this);
    _colorButton = new QColorButton (this);
    _deleteCheck = new QCheckBox (this);
    layout -> addRow ("Index", _indexEdit);
    layout -> addRow ("Color", _colorButton);
    layout -> addRow ("Delete", _deleteCheck);
    connect (_deleteCheck, &QCheckBox::stateChanged, this, [=] () {
            _indexEdit->setEnabled (!(_deleteCheck->isChecked ()));
            _colorButton->setEnabled (!(_deleteCheck->isChecked ()));
    });

    widget -> layout() -> addWidget (buttonBox);
    widget -> adjustSize();
    _indexEdit -> setFocus();
    setTabOrder (_indexEdit, _colorButton);
    adjustSize();
    setContentsMargins (0, 0, 0, 0);

}


LegendEntryDialog::~LegendEntryDialog() {

}

void LegendEntryDialog::preventDelete (const bool& prevented) {
    _deleteCheck -> setChecked (false);
    _deleteCheck -> setEnabled (prevented);
}

void LegendEntryDialog::showEvent (QShowEvent* event) {
    _deleteCheck -> setChecked (false);
}

void LegendEntryDialog::setColor (const QColor& color) {
    _colorButton -> setColor (color);
}

void LegendEntryDialog::setIndex (const QString& index) const {
    _indexEdit -> setText (index);
}

QColor LegendEntryDialog::color () {
    return _colorButton -> color();
}

QString LegendEntryDialog::index () {
    return _indexEdit -> text();
}

bool LegendEntryDialog::isDelete () {
    return _deleteCheck -> isChecked();
}
