//
// Created by martin on 28/09/17.
//

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <src/CalenhadServices.h>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDialogButtonBox>
#include "CalenhadLegendDialog.h"
#include "../legend/LegendManager.h"
#include "../legend/LegendService.h"

using namespace calenhad::controls;
using namespace calenhad::legend;

CalenhadLegendDialog::CalenhadLegendDialog (QWidget* parent) {
    _legendManager = new LegendManager (this);
    setLayout (new  QVBoxLayout());
    layout() -> addWidget (_legendManager);

    QDialogButtonBox* buttonBox = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect (buttonBox, &QDialogButtonBox::accepted, this, &CalenhadLegendDialog::accept);
    connect (buttonBox, &QDialogButtonBox::rejected, this, &CalenhadLegendDialog::reject);
    layout() ->addWidget (buttonBox);
}

void CalenhadLegendDialog::showEvent (QShowEvent* event) {

}

void CalenhadLegendDialog::reject() {
    if (CalenhadServices::legends() -> isDirty()) {
        QMessageBox confirm;
        confirm.setText ("This will discard all changes to legends. Are you sure?");
        confirm.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        confirm.setDefaultButton (QMessageBox::Yes);
        confirm.setEscapeButton (QMessageBox::No);
        int result = confirm.exec ();
        if (result == QMessageBox::Yes) {
            CalenhadServices::legends() -> rollback();
            QDialog::reject();
        }
    } else {
        QDialog::reject();
    }
}

void CalenhadLegendDialog::accept() {
    CalenhadServices::legends() -> commit();
    emit legendChanged (_legendManager -> currentLegend());
    QDialog::accept();
}