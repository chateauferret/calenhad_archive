//
// Created by martin on 16/12/16.
//

#include "QNode.h"
#include <QLineEdit>
#include <QTextStream>
#include <QVariant>
#include <iostream>
#include "../nodeedit/qneconnection.h"
#include <QMenu>
#include "../nodeedit/CalenhadController.h"
#include "QModule.h"
#include "../nodeedit/Calenhad.h"
#include "../messagefactory.h"
#include "../pipeline/CalenhadModel.h"
#include "../actions/ChangeModuleCommand.h"


QNode::QNode (QWidget* widget) : QWidget (widget), _model (nullptr), _isInitialised (false) {
    connect (this, &QNode::initialised, this, [=] () { _isInitialised = true; });
}

QNode::~QNode () {
    if (_dialog) { delete _dialog; }
}

void QNode::initialise() {
    _ports.clear();
    addInputPorts();

    // these widgets are relevant for all modules
    _expander = new QToolBox();
    QWidget* about = new QWidget (_expander);
    QVBoxLayout* layout = new QVBoxLayout();
    layout -> setSpacing (0);
    layout -> setMargin (5);
    about -> setLayout (layout);
    _nameEdit = new QLineEdit();
    connect (_nameEdit, SIGNAL (textChanged (const QString&)), this, SLOT (setName (const QString&)));

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
    _dialog = new QDialog();
    _dialog -> setLayout (l);

    _contentLayout = new QFormLayout();
    _content = new QWidget (QNode::_expander);
    _content -> setLayout (_contentLayout);
    QNode::addPanel (tr ("Parameters"), _content);
}

QString QNode::name() {
    return _name;
}

void QNode::setName (const QString& name) {
    if (! (name.isNull()) && (name != _name)) {
        _name = name;
        _nameEdit -> setText (name);
        update();
        emit (nameChanged (name));
    }
}

void QNode::setNotes (const QString& notes) {
    if (! notes.isNull()) {
        if (! (notes == _notesEdit -> toPlainText())) {
            preserve();
            _notes = notes;
            _notesEdit -> setText (_notes);
        }
    }
    emit (notesChanged (notes));
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
    std::cout << "Is complete " << isComplete() << "\n";
    return isComplete();
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

void QNode::invalidate() {
    setEnabled (true);
    emit nodeChanged ("inputs", 0);
}

void QNode::setModel (CalenhadModel* model) {
    if (! _model) {
        _model = model;
        initialise();
        emit nodeChanged ("model", 0);
    } else {
        std::cout << "Can't reassign module to another model";
    }
}

void QNode::inflate (const QDomElement& element) {
    _element = element;
    QDomNodeList notesElements = element.elementsByTagName ("notes");
    _notes = notesElements.isEmpty() ? QString() : notesElements.item(0).nodeValue();
    QDomNodeList portNodes = element.elementsByTagName ("port");
    for (int i = 0; i < portNodes.count(); i++) {
        bool okIndex, okType;
        int portIndex = portNodes.at (i).attributes ().namedItem ("index").nodeValue ().toInt (&okIndex);
        int portType = portNodes.at (i).attributes ().namedItem ("type").nodeValue ().toInt (&okType);
        QString name = portNodes.at (i).attributes ().namedItem ("name").nodeValue ();
        if (okIndex && okType) {
            for (QNEPort* p : _ports) {
                if (p->index () == portIndex && p->type () == portType) {
                    p->setName (name);
                }
            }
        } else {
            QString m = "Can't find " + portNodes.at (i).attributes ().namedItem ("type").nodeValue() + " port with index " + portNodes.at (i).attributes ().namedItem ("index").nodeValue() + " in module " + _name;
            CalenhadServices::messages() -> message ("Reverting to default port names", m);
        }
    }
}

void QNode::serialise (QDomDocument& doc) {
    _element = doc.createElement ("module");

    doc.documentElement().appendChild (_element);
    QDomElement nameElement = doc.createElement ("name");
    QDomText nameText = doc.createTextNode (_name);
    nameElement.appendChild (nameText);
    _element.appendChild (nameElement);

    if (! _notes.isEmpty ()) {
        QDomElement notesElement = doc.createElement ("notes");
        _element.appendChild (notesElement);
        QDomText notesContent = doc.createTextNode (_notes);
        notesElement.appendChild (notesContent);
    }
    for (QNEPort* p : _ports) {
        QDomElement portElement = doc.createElement ("port");
        _element.appendChild (portElement);
        portElement.setAttribute ("index", p -> index());
        portElement.setAttribute ("name", p -> portName());
        portElement.setAttribute ("type", p -> portType());
    }
}

void QNode::showEvent (QShowEvent* event) {
    _nameEdit -> setText (_name);
    QWidget::showEvent (event);
}

QString QNode::preservedXml() {
    return _preservedXml;
}

void QNode::preserve() {
    if (_model) {
        QDomDocument doc;
        QDomElement root = doc.createElement ("model");
        doc.appendChild (root);
        serialise (doc);
        _preservedXml = doc.toString ();
    }
}
