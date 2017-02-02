//
// Created by martin on 16/01/17.
//

#include "CalenhadMarbleWidget.h"
#include <marble/MarbleDirs.h>
#include <iostream>

CalenhadMarbleWidget::CalenhadMarbleWidget (QWidget* parent) : MarbleWidget (parent) {
    setMapThemeId (QStringLiteral ("earth/calenhad/calenhad.dgml"));
    setShowBackground (false);
    std::cout <<  "Marble data path '" << Marble::MarbleDirs::marbleDataPath ().toStdString () << "'\n";
    std::cout << "Map theme id '" << mapThemeId ().toStdString () << "'\n";
}

CalenhadMarbleWidget::~CalenhadMarbleWidget () {

}
