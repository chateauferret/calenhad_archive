//
// Created by martin on 16/12/16.
//

#include "QNode.h"
#include <QLineEdit>
#include <iostream>
#include "../nodeedit/qneconnection.h"
#include <QMenu>
#include "../nodeedit/CalenhadController.h"

void QNode::initialise() {

    addInputPorts();

    // these widgets are relevant for all modules
    _expander = new QToolBox();
    QWidget* about = new QWidget (_expander);
    QVBoxLayout* layout = new QVBoxLayout();
    layout -> setSpacing (0);
    layout -> setMargin (5);
    about -> setLayout (layout);
    _nameEdit = new QLineEdit();
    connect (_nameEdit, &QLineEdit::textChanged, this, [=] () {
        setName (_nameEdit -> text());
    });

    _nameEdit -> selectAll();
    layout -> addWidget (_nameEdit);

    _notesEdit = new QTextEdit (about);
    _notesEdit -> setFixedHeight (100);
    layout -> addWidget (_notesEdit);
    connect (_notesEdit, &QTextEdit::textChanged, this, [=] () {
        setNotes (_notesEdit -> document() -> toPlainText());
    });
    addPanel ("About", about);
    QLayout* l = new QVBoxLayout();
    l -> setSpacing (0);
    l -> setMargin (5);
    l -> addWidget (_expander);
    setLayout (l);

    _contentLayout = new QFormLayout();
    _content = new QWidget (QNode::_expander);
    _content -> setLayout (_contentLayout);
    QNode::addPanel (tr ("Parameters"), _content);
}

QString QNode::name() {
    return _name;
}

void QNode::setName (const QString& name) {
    if (! (name.isNull ())) {
        _name = name;
        if (! (name == _nameEdit -> text())) {
            _nameEdit -> setText (name);
        }
    }
    emit (nodeChanged ("name", name));
    update();

}

void QNode::setNotes (const QString& notes) {
    _notes = notes;
    if (! notes.isNull()) {
        if (!(notes == _notesEdit -> toPlainText ())) {
            _notesEdit -> setText (_notes);
        }
    }
    emit (nodeChanged ("notes", notes));
}

QString QNode::notes() {
    return _notes;
}

int QNode::addPanel (const QString& title, QWidget* widget) {
    return _expander -> addItem (widget, title);
}

void QNode::addPort (QNEPort* port) {
    _ports.append (port);
}

QList<QNEPort*> QNode::ports() {
    return _ports;
}

bool QNode::isInitialised() {
    return _isInitialised;
}

bool QNode::isComplete() {
    for (QNEPort* p : _ports) {
        if (p -> portType() != QNEPort::OutputPort) {
            if (! (p -> hasConnection ())) {
                return false;
            }
        }
    }
    return true;
}

bool QNode::isRenderable() {
    return isComplete() && isEnabled() ;
    // other conditions to do
}

// Spin box which selects a libnoiseutils level value between -1 and 1
QDoubleSpinBox* QNode::noiseValueParamControl (const QString& text) {
    QDoubleSpinBox* spin = new QDoubleSpinBox (_content);
    spin -> setRange (-1.0, 1.0);
    spin -> setSingleStep (0.1);
    spin -> setToolTip (text);
    return spin;
}

// Spin box which selects an iteration or octave count between 1 and 12
QSpinBox* QNode::countParameterControl (const QString& text) {
    QSpinBox* spin = new QSpinBox (_content);
    spin -> setRange (1, 12);
    spin -> setToolTip (text);
    return spin;
}

// Spin box which selects an angle between + / - 180 degrees
QDoubleSpinBox* QNode::angleParameterControl (const QString& text) {
    QDoubleSpinBox* spin = new QDoubleSpinBox (_content);
    spin -> setRange (-179.0, 180.0);
    spin -> setSingleStep (1.0);
    spin -> setToolTip (text);
    return spin;
}

QLogSpinBox* QNode::logParameterControl (const QString& text) {
    return new QLogSpinBox (_content);
}

QNode::QNode (QWidget* widget) : QWidget (widget), _model (nullptr), _isInitialised (false) {

}

QNode::~QNode () {

}

void QNode::setModel (CalenhadModel* model) {
    if (! _model) {
        _model = model;
    } else {
        std::cout << "Can't reassign module to another model";
    }
}
