//
// Created by martin on 20/09/17.
//

#include <CalenhadServices.h>
#include <QtWidgets/QGraphicsItem>
#include <iostream>
#include "NodeNameValidator.h"
#include "../preferences/PreferencesService.h"
#include "exprtk/Calculator.h"
#include "module/Node.h"
#include "../pipeline/CalenhadModel.h"
#include "NodeBlock.h"

using namespace calenhad::nodeedit;
using namespace calenhad::module;
using namespace calenhad::preferences;
using namespace calenhad::pipeline;
using namespace calenhad;

NodeNameValidator::NodeNameValidator (Node* node) :
    QRegularExpressionValidator (QRegularExpression (CalenhadServices::preferences() -> calenhad_node_name_validChars)),
    _node (node) {
}

NodeNameValidator::~NodeNameValidator () {

}

QValidator::State NodeNameValidator::validate (QString& input, int& pos) const {
    QString errors = "";
    // make sure name is well-formed
    QValidator::State state = QRegularExpressionValidator::validate (input, pos);
    if (state != QValidator::Acceptable) {
        errors += ("Name must contain only letters, numbers and underscores ('_'), and must start with a letter\n");
        return state;
    }

    // check for length
    if (input.length() < CalenhadServices::preferences() -> calenhad_node_name_minLength) {
        errors += ("Name must contain at least " + QString::number (CalenhadServices::preferences() -> calenhad_node_name_minLength) + " characters\n");
        state = QValidator::Intermediate;
    }

    if (input.length() > CalenhadServices::preferences() -> calenhad_node_name_maxLength) {
        errors += ("Name cannot be more than " + QString::number (CalenhadServices::preferences() -> calenhad_node_name_maxLength) + " characters long\n");
        state = QValidator::Invalid;
    }

    // make sure name isn't a reserved word
    if (CalenhadServices::calculator() -> reservedWords.contains (input)) {
       errors += ("Name cannot be a reserved word\n");
       state = QValidator::Intermediate;
    }

    // make sure name isn't a duplicate (another node)
    CalenhadModel* m = _node -> model();
    foreach (QGraphicsItem* item, m -> items()) {
        if (item -> type() == QGraphicsItem::UserType + 3) {  // is a NodeBlock
            NodeBlock* handle = (NodeBlock*) item;
            if (input == handle -> node() -> name() && handle -> node() != _node) {
                errors += ("Name must be unique among all objects in a model\n");
                state = QValidator::Intermediate;
            }
        }
    }

    if (state != QValidator::Acceptable) {
        emit message (errors);
    } else {
        emit success();
    }
    return state;
}
