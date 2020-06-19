//
// Created by ferret on 18/06/2020.
//

#include "GisGlobeDialog.h"

calenhad::gis::GisGlobeDialog::GisGlobeDialog (QWidget *parent) : CalenhadGlobeDialog (parent, nullptr) {

}

calenhad::gis::GisGlobeDialog::~GisGlobeDialog() {

}

GisGlobeWidget *calenhad::gis::GisGlobeDialog::widget() {
    return reinterpret_cast<GisGlobeWidget*> (_widget);
}

void calenhad::gis::GisGlobeDialog::initialise() {
    CalenhadGlobeDialog::initialise();
}
