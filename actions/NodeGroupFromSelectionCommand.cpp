//
// Created by martin on 10/05/18.
//

#include "NodeGroupFromSelectionCommand.h"

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QGuiApplication>
#include "../pipeline/CalenhadModel.h"
#include "qmodule/NodeGroup.h"

using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::actions;

NodeGroupFromSelectionCommand::NodeGroupFromSelectionCommand (calenhad::pipeline::CalenhadModel* model, const QString& oldXml) : XmlCommand (model, oldXml) {

}

NodeGroupFromSelectionCommand::~NodeGroupFromSelectionCommand () {

}

void NodeGroupFromSelectionCommand::undo () {
    XmlCommand::undo();
}

void NodeGroupFromSelectionCommand::redo () {
    QPainterPath path = _model -> selectionArea();
    _model -> addNodeGroup (path);
    setNewXml (_model -> snapshot());
}
