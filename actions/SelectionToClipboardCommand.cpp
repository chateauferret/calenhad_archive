//
// Created by martin on 08/10/17.
//

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QGuiApplication>
#include <QClipboard>
#include "SelectionToClipboardCommand.h"
#include "../pipeline/CalenhadModel.h"
#include "../qmodule/QModule.h"
#include "../nodeedit/qneport.h"
#include "../nodeedit/qneconnection.h"
#include "../nodeedit/QNodeBlock.h"

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
    for (QNEConnection* c : _model -> connections()) {
        QNode* n0 = c -> port1() -> owner();
        QNode* n1 = c -> port2() -> owner();
        if (n1 -> handle() -> isSelected() && n0 -> handle() -> isSelected()) {
            c -> setSelected (true);
        }
    }

    // serialise all selected items to the document
    for (QGraphicsItem* item : _model -> selectedItems()) {
        QNodeBlock* b = dynamic_cast<QNodeBlock*> (item);
        if (b) {
            QNode* n = b -> node();
            n -> serialize (doc);
        }
        QNEConnection* c = dynamic_cast<QNEConnection*> (item);
        if (c) {
            c -> serialise (doc);
        }
    }

    QString xml = doc.toString();

    QClipboard* clipboard = QGuiApplication::clipboard();
    _oldXml = clipboard -> text();
    clipboard -> setText (xml);
}
