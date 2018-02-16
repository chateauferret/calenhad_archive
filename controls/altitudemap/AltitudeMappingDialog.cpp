//
// Created by martin on 07/02/18.
//

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include "AltitudeMappingDialog.h"
#include "../../exprtk/ExpressionWidget.h"

using namespace calenhad::controls::altitudemap;
using namespace calenhad::expressions;

AltitudeMappingDialog::AltitudeMappingDialog (QWidget* parent) : QDialog (parent), _index (-1) {
    QWidget* widget = new QWidget (this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect (buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    QFormLayout* layout = new QFormLayout (this);
    widget -> setLayout (layout);
    _from =  new ExpressionWidget (this);
    _to = new ExpressionWidget (this);
    layout -> addRow ("From", _from);
    layout -> addRow ("To", _to);
    widget -> layout() -> addWidget (buttonBox);
    widget -> adjustSize();
    _from -> setFocus();
    setTabOrder (_from, _to);
    adjustSize();
    //setWindowFlags (Qt::Window | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
    setContentsMargins (0, 0, 0, 0);
    adjustSize();

}

AltitudeMappingDialog::~AltitudeMappingDialog() {

}

int AltitudeMappingDialog::index() {
    return _index;
}

void AltitudeMappingDialog::setMapping (const AltitudeMapping& mapping) {
    _mapping = mapping;
    _from -> setText (_mapping.expressionX());
    _to -> setText (_mapping.expressionY());
}

AltitudeMapping AltitudeMappingDialog::mapping() {
    _mapping.setExpressionX (_from -> text());
    _mapping.setExpressionY (_to -> text());
    return _mapping;
}

bool AltitudeMappingDialog::hasErrors () {
    return _from -> hasErrors() || _to -> hasErrors();
}

void AltitudeMappingDialog::focusOutEvent (QFocusEvent* event) {
    reject();
}

void AltitudeMappingDialog::showEvent (QShowEvent* event) {

}

void AltitudeMappingDialog::setIndex (const int& index) {
    _index = index;
}
