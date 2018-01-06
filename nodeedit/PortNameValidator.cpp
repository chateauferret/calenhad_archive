//
// Created by martin on 20/09/17.
//

#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"
#include "exprtk/VariablesService.h"
#include "PortNameValidator.h"
#include "qneport.h"
#include "../qmodule/QNode.h"

using namespace calenhad::nodeedit;

PortNameValidator::PortNameValidator (QNEPort* port) : NodeNameValidator (port -> owner()), _port (port) {

}

PortNameValidator::~PortNameValidator() {

}

QValidator::State PortNameValidator::validate (QString& input, int& pos) const {
    QString errors = "";

    // make sure name is well-formed
    QValidator::State state = QRegularExpressionValidator::validate (input, pos);
    if (state != QValidator::Acceptable) {
        errors += ("Name must contain only letters, numbers and underscores ('_'), and must start with a letter\n");
        return state;
    }

    // check for length
    if (input.length() < CalenhadServices::preferences() -> calenhad_port_name_minLength) {
        errors += ("Name must contain at least " + QString::number (CalenhadServices::preferences() -> calenhad_port_name_minLength) + " characters\n");
        state = QValidator::Intermediate;
    }

    if (input.length() > CalenhadServices::preferences() -> calenhad_port_name_maxLength) {
        errors += ("Name cannot be more than " + QString::number (CalenhadServices::preferences() -> calenhad_port_name_maxLength) + " characters long\n");
        state = QValidator::Invalid;
    }

    // make sure name isn't a reserved word
    if (CalenhadServices::calculator() -> reservedWords.contains (input)) {
        errors += ("Name cannot be a reserved word\n");
        state = QValidator::Intermediate;
    }

    // make sure name isn't a duplicate (another port on the same node)
    foreach (QNEPort* p, _node -> ports()) {
        if (p -> portName() == input && p != _port) {
            errors += ("Name must be unique among all ports in a node\n");
            state = QValidator::Intermediate;
        }
    }

    if (state != QValidator::Acceptable) {
        emit message (errors);
    } else {
        emit success();
    }
    return state;
}
