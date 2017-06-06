//
// Created by martin on 28/04/17.
//

#include "DuplicateNodeCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../nodeedit/qneblock.h"
#include "../CalenhadServices.h"

DuplicateNodeCommand::DuplicateNodeCommand (QNode* node, CalenhadModel* model) : _node (node), _model (model), _copy (nullptr) {

}

DuplicateNodeCommand::~DuplicateNodeCommand () {

}

void DuplicateNodeCommand::undo () {
    _model->deleteNode (_copy);
}

void DuplicateNodeCommand::redo () {
    _copy = _node -> clone();
    QString name = _node -> name();
    int i = 1;
    while (_model -> findModule (name)) {
        name = _node -> name() + " copy " + QString::number (i);
    }
    QPointF p = _node -> handle() -> scenePos();
    QPointF newPoint = QPointF (p.x() + CalenhadServices::preferences () -> calenhad_module_duplicate_offset_x, p.y() + CalenhadServices::preferences () -> calenhad_module_duplicate_offset_y);
    _model -> addNode (_copy, newPoint);
}
