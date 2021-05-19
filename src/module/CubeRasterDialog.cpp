//
// Created by ferret on 14/10/2020.
//

#include <QtWidgets/QVBoxLayout>
#include <QDialogButtonBox>
#include <QHeaderView>
#include "CubeRasterDialog.h"
#include "CubeRasterFile.h"
#include "CubeRasterModule.h"
#include "CubeRasterConfig.h"
#include "../controls/SpinBoxDelegate.h"
#include "../controls/CubeRasterWidget.h"

using namespace calenhad::module;
using namespace calenhad::controls;

CubeRasterDialog::CubeRasterDialog (CubeRasterModule* parent) : QDialog(), _widget (new CubeRasterWidget (parent)) {
    setLayout (new QVBoxLayout());
   // _widget -> horizontalHeader() -> show();
   // _widget -> verticalHeader() -> show();
   // _widget -> setModel (_config);
    layout() ->addWidget (_widget);
   // _widget -> setItemDelegateForColumn (5, new SpinBoxDelegate());


    QDialogButtonBox* buttons = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect (buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect (buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout() -> addWidget (buttons);

}

CubeRasterDialog::~CubeRasterDialog () {

}
