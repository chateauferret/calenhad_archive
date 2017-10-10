//
// Created by martin on 08/10/17.
//

#include <QtGui/QClipboard>
#include <QtGui/QGuiApplication>
#include <QtXml/QDomDocument>
#include <nodeedit/Calenhad.h>
#include "../pipeline/CalenhadModel.h"
#include "PasteCommand.h"


using namespace calenhad::pipeline;
using namespace calenhad::nodeedit;
using namespace calenhad::qmodule;
using namespace calenhad::actions;

PasteCommand::PasteCommand (calenhad::pipeline::CalenhadModel* model) : QUndoCommand(), _model (model) {

}

PasteCommand::~PasteCommand () {

}

void PasteCommand::undo () {
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard -> setText (_oldXml);
}

void PasteCommand::redo () {
    QDomDocument doc;
    if (doc.setContent (QGuiApplication::clipboard() -> text())) {
        _model -> inflate (doc, CalenhadFileType::CalenhadModelFragment);
    }
}
