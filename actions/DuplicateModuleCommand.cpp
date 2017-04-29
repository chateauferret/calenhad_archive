//
// Created by martin on 28/04/17.
//

#include "DuplicateModuleCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/qneblock.h"
#include "../CalenhadServices.h"

DuplicateModuleCommand::DuplicateModuleCommand (QModule* module, CalenhadModel* model) : _module (module), _model (model), _copy (nullptr) {

}

DuplicateModuleCommand::~DuplicateModuleCommand () {

}

void DuplicateModuleCommand::undo () {
    _model -> deleteModule (_copy);
}

void DuplicateModuleCommand::redo () {
    _copy = _module -> clone();
    QString name = _module -> name();
    int i = 1;
    while (_model -> findModule (name)) {
        name = _module -> name() + " copy " + QString::number (i);
    }
    QPointF p = _module -> handle() -> scenePos();
    QPointF newPoint = QPointF (p.x() + CalenhadServices::preferences () -> calenhad_module_duplicate_offset_x, p.y() + CalenhadServices::preferences () -> calenhad_module_duplicate_offset_y);
    _model -> addModule (_copy, newPoint);
}
