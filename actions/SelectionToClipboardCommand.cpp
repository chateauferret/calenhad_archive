//
// Created by martin on 08/10/17.
//

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QGuiApplication>
#include <QClipboard>
#include "SelectionToClipboardCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "qmodule/Module.h"
#include "nodeedit/Port.h"
#include "nodeedit/Connection.h"
#include "nodeedit/NodeBlock.h"

using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::actions;

SelectionToClipboardCommand::SelectionToClipboardCommand (calenhad::pipeline::CalenhadModel* model) : QUndoCommand(), _model (model) {

}

SelectionToClipboardCommand::~SelectionToClipboardCommand () {

}

void SelectionToClipboardCommand::undo () {
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard -> setText (_oldXml);
}

void SelectionToClipboardCommand::redo () {
    QDomDocument doc;
    QDomElement root = doc.createElement ("fragment");
    doc.appendChild (root);

    // copy any connections that are between nodes that are both in the selection
    for (Connection* c : _model -> connections()) {
        Node* n0 = c -> port1() -> owner();
        Node* n1 = c -> port2() -> owner();
        if (n1 -> handle() -> isSelected() && n0 -> handle() -> isSelected()) {
            c -> setSelected (true);
        }
    }

    // serialise all selected items to the document
    for (QGraphicsItem* item : _model -> selectedItems()) {
        NodeBlock* b = dynamic_cast<NodeBlock*> (item);
        if (b) {
            Node* n = b -> node();
            n -> serialize (doc);
        }
        Connection* c = dynamic_cast<Connection*> (item);
        if (c) {
            c -> serialise (doc);
        }
    }

    QString xml = doc.toString();

    QClipboard* clipboard = QGuiApplication::clipboard();
    _oldXml = clipboard -> text();
    clipboard -> setText (xml);
}
