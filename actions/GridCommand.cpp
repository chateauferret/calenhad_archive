//
// Created by martin on 17/10/18.
//

#include <iostream>
#include "GridCommand.h"
#include "../nodeedit/CalenhadView.h"
#include "../preferences/PreferencesService.h"
#include "../CalenhadServices.h"


using namespace calenhad::actions;
using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;


GridCommand::GridCommand (CalenhadView* view) {
    _view = view;
    setText (QObject::tr (_view -> gridVisible() ? "Hide grid" : "Show grid"));
}

GridCommand::~GridCommand () {

}

void GridCommand::undo () {
    redo();
}

void GridCommand::redo () {
    setText (QObject::tr (_view -> gridVisible() ? "Hide grid" : "Show grid"));
    std::cout << "Grid command " << text().toStdString () << "\n";
    _view -> setGridVisible (! _view -> gridVisible());
    _view -> update();
}
