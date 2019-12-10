//
// Created by martin on 18/10/18.
//

#include "SnapToGridCommand.h"
//
// Created by martin on 17/10/18.
//

#include <iostream>
#include "SnapToGridCommand.h"
#include "../nodeedit/CalenhadView.h"
#include "../preferences/PreferencesService.h"
#include "src/CalenhadServices.h"


using namespace calenhad::actions;
using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;


SnapToGridCommand::SnapToGridCommand (CalenhadView* view) {
    _view = view;
    setText (QObject::tr (_view -> gridVisible() ? "Hide grid" : "Show grid"));
}

SnapToGridCommand::~SnapToGridCommand () {

}

void SnapToGridCommand::undo () {
    redo();
}

void SnapToGridCommand::redo () {
    setText (QObject::tr (_view -> gridVisible() ? "Turn off snap to grid" : "Snap to grid"));
    _view -> setSnapToGrid (! _view -> snapToGrid());
    _view -> update();
}
