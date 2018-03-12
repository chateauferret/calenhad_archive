//
// Created by martin on 16/12/16.
//
#include "Node.h"
#include <QDialog>
#include <QString>
#include <QCloseEvent>
#include <iostream>
#include "nodeedit/Connection.h"
#include <QMenu>
#include <nodeedit/NodeNameValidator.h>
#include <actions/CreateConnectionCommand.h>
#include "../nodeedit/CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../pipeline/ModuleFactory.h"
#include "nodeedit/Port.h"
#include "nodeedit/NodeBlock.h"
#include "../actions/ChangeModuleCommand.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::actions;

Node::Node (const QString& nodeType, QWidget* parent) : QWidget (parent),
    _model (nullptr),
    _dialog (nullptr),
    _handle (nullptr),
    _content (nullptr),
    _contentLayout (nullptr),
    _palette (nullptr),
    _validator (nullptr),
    _connectMenu (nullptr),
    _nodeType (nodeType) {

}

Node::~Node () {
    if (_dialog) { delete _dialog; }
    if (_validator) { delete _validator; }
    if (_palette) { delete _palette; }
    if (_connectMenu) { delete _connectMenu; }
}


void Node::initialise() {
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
    _palette = new QPalette();
    _nameEdit -> setPalette (*_palette);
    _validator = new NodeNameValidator (this);
    connect (_validator, &NodeNameValidator::message, this, [=] (const QString& message) {
        _nameEdit -> setToolTip (message);
        _palette -> setColor (QPalette::Text, CalenhadServices::preferences() -> calenhad_module_text_color_error);
        _nameEdit->setPalette (*_palette);
    });
    connect (_validator, &NodeNameValidator::success, this, [=] () {
        _nameEdit -> setToolTip (QString::null);
        _palette -> setColor (QPalette::Text, CalenhadServices::preferences() -> calenhad_module_text_color_normal);
        _nameEdit->setPalette (*_palette);
    });
    _nameEdit -> setValidator (_validator);

    layout -> addWidget (_nameEdit);

    connect (_nameEdit, &QLineEdit::editingFinished, this, [=] () {
        propertyChangeRequested ("name", _nameEdit -> text());
    });
    connect (this, &Node::nameChanged, this, [=] () { _nameEdit -> setText (_name); });

    _notesEdit = new QTextEdit (about);
    _notesEdit -> setFixedHeight (100);
    layout -> addWidget (_notesEdit);
    layout -> addStretch (0);

    connect (_notesEdit, &QTextEdit::textChanged, this, [=] () {
        propertyChangeRequested ("notes", _notesEdit -> document() -> toPlainText());
    });
    connect (this, &Node::notesChanged, this, [=] () { _notesEdit -> setText (_notes); });

    addPanel ("About", about);
    QLayout* l = new QVBoxLayout();
    l -> setSpacing (0);
    l -> setMargin (5);
    _dialog = new QDialog (this);
    _dialog -> setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint | Qt::WindowMinimizeButtonHint);
    _dialog -> setLayout (new QVBoxLayout());
    _dialog -> layout() -> addWidget (_expander);
    _dialog -> resize (300, 200);
    // when we change panels, move the focus to the newly-shown panel - this removes the focus from the parameter controls and causes their
    // values to be updated to the underlying noise owner data
    connect (_expander, &QToolBox::currentChanged, this, [=] () {
        QWidget* w = _expander -> currentWidget() -> findChild<QWidget*>();
        if (w) {
            w -> setFocus();
        }
    });
}

QString Node::name() {
    return _name;
}

// setName assumes the name has been validated using CalenhadModel::validateName - it is the caller's
// responsibility to call this first (since we don't know if the caller wants messages).
void Node::setName (const QString& name) {
    if (! (name.isNull()) && (name != _name)) {
        _name = name;
        _name = _name.replace (" ", "_");
        _nameEdit -> setText (_name);
        update();
        if (_dialog) {
            _dialog -> setWindowTitle (_name + " (" + nodeType () + ")");
        }
        emit (nameChanged (name));
    }
}

void Node::setNotes (const QString& notes) {
    if (! notes.isNull()) {
        if (! (notes == _notesEdit -> toPlainText())) {
            _notes = notes;
            _notesEdit -> setText (_notes);
        }
    }
    emit (notesChanged (notes));
}

QString Node::notes() {
    return _notes;
}

int Node::addPanel (const QString& title, QWidget* widget) {
    return _expander -> addItem (widget, title);
}

NodeBlock* Node::handle() {
    return _handle;
}

void Node::addPort (Port* port, const unsigned& index) {
    _ports.append (port);
    if (port -> portType () == Port::OutputPort) {
        _output = port;
    } else {
        _inputs.insert (index, port);
    }
}

QVector<Port*> Node::ports() {
    return _ports;
}

bool Node::isComplete() {
    bool complete = true;
    QList<ExpressionWidget *> widgets = findChildren<ExpressionWidget*>();
    if (! (widgets.isEmpty())) {
        for (ExpressionWidget* ew: widgets) {
            if (!ew->isValid ()) {
                complete = false;
                break;
            }
        }
    }
    if (complete) {
        for (Port* p : _ports) {
            if (p->portType () != Port::OutputPort) {
                if (!(p->hasConnection ())) {
                    complete = false;
                    break;
                }
            }
        }
    }
    return complete;
}

void Node::invalidate() {
    if (_handle) {
        _handle -> update ();
    }
    emit nodeChanged();
}

void Node::setModel (CalenhadModel* model) {
        _model = model;

}

void Node::showParameters (const bool& visible) {
    if (visible) {
        _dialog -> setWindowTitle (name () + " (" + nodeType () + ")");
        _dialog -> setAttribute (Qt::WA_DeleteOnClose, false);
        _dialog -> show();
    } else {
        _dialog -> hide();
    }
}

void Node::inflate (const QDomElement& element) {
    _element = element;
        QDomElement notesNode = element.firstChildElement ("name");
        QString name = notesNode.text ();
        QDomNodeList portNodes = element.elementsByTagName ("port");
        for (int i = 0; i < portNodes.count (); i++) {
            bool okIndex, okType;
            int portIndex = portNodes.at (i).attributes().namedItem ("index").nodeValue().toInt (&okIndex);
            int portType = portNodes.at (i).attributes().namedItem ("type").nodeValue().toInt (&okType);
            QDomElement portNameNode = portNodes.at (i).firstChildElement ("name");
            QString name = portNameNode.text ();
            if (okIndex && okType) {
                for (Port* p : _ports) {
                    if (p -> index () == portIndex && p -> portType() == portType) {
                        p -> setName (name);
                    }
                }
            } else {
                QString m = "Can't find " + portNodes.at (i).attributes ().namedItem ("type").nodeValue () + " port with index " +
                            portNodes.at (i).attributes ().namedItem ("index").nodeValue () + " in owner " + _name;
                CalenhadServices::messages() -> message ("warning", "Reverting to default port names. " + m);
            }
        }
        QDomNodeList paramNodes = element.elementsByTagName ("parameter");
        for (int i = 0; i < paramNodes.count (); i++) {
            QString paramName = paramNodes.at (i).attributes ().namedItem ("name").nodeValue ();
            QString paramValue = paramNodes.at (i).attributes ().namedItem ("value").nodeValue ();
            setParameter (paramName, paramValue);
        }
}

void Node::serialize (QDomDocument& doc) {
    _element = doc.createElement ("module");
    doc.documentElement().appendChild (_element);
    QDomElement nameElement = doc.createElement ("name");
    QDomText nameText = doc.createTextNode (_name);
    nameElement.appendChild (nameText);
    _element.appendChild (nameElement);

    if (! _notes.isEmpty()) {
        QDomElement notesElement = doc.createElement ("notes");
        _element.appendChild (notesElement);
        QDomText notesContent = doc.createTextNode (_notes);
        notesElement.appendChild (notesContent);
    }
    for (Port* p : _ports) {
        QDomElement portElement = doc.createElement ("port");
        _element.appendChild (portElement);
        portElement.setAttribute ("index", p -> index());
        portElement.setAttribute ("type", p -> portType());
        QDomElement portNameElement = doc.createElement ("name");
        QDomText portNameText = doc.createTextNode (p -> portName());
        portNameElement.appendChild (portNameText);
        portElement.appendChild (portNameElement);
    }
    if (! _parameters.isEmpty()) {
        for (QString key : _parameters.keys()) {
            QDomElement paramElement = doc.createElement ("parameter");
            paramElement.setAttribute ("name", key);
            paramElement.setAttribute ("value", _parameters.value (key) -> text ());
            _element.appendChild (paramElement);
        }
    }
}

void Node::propertyChangeRequested (const QString& p, const QVariant& value) {
    if (_model && (property (p.toStdString ().c_str ()) != value)) {
        ChangeModuleCommand* c = new ChangeModuleCommand (this, p, property (p.toStdString ().c_str ()), value);
        if (_model) {
            _model -> controller() -> doCommand (c);
        }
        _model -> update();
    }
}

void Node::showEvent (QShowEvent* event) {
    _nameEdit -> setText (_name);
    resize (300, 200);
    QWidget::showEvent (event);
}

CalenhadModel* Node::model () {
    return _model;
}

QString Node::propertyName (const QString& name) {
    QString result;
    QString text = name.trimmed();
    for (int i = 0; i < text.size(); i++) {
        if (i == 0) {
            result = text.at (i).toLower();
        } else {
            if (text.at (i) != ' ') {
                result += text.at (i);
            } else {
                i++;
                result += text.at (i).toUpper();
            }
        }
    }
    return result;
}

void Node::closeEvent (QCloseEvent* event) {
    event -> ignore();
    showParameters (false);
}

bool Node::hasParameters () {
    return ! (_parameters.isEmpty());
}

void Node::setGroup (NodeGroup* group) {
    _group = group;
}

QString Node::nodeType() {
    return _nodeType;
}

NodeGroup* Node::group () {
    return _group;
}

ExpressionWidget* Node::addParameter (const QString& label, const QString& name, const double& initial, ParamValidator* validator, QWidget* _panel) {

    // create a panel to hold the parameter widgets, if we haven't done this already
    if (! _panel) {
        if (!(_content)) {
            addContentPanel ();
        }
        _panel = _content;
    }

    if (dynamic_cast<QFormLayout*> (_panel -> layout())) {
        ExpressionWidget* widget = new ExpressionWidget (this);
        connect (widget, &ExpressionWidget::compiled, this, &Node::nodeChanged);
        connect (widget, &ExpressionWidget::errorFound, this, &Node::nodeChanged);
        connect (widget, &ExpressionWidget::expressionChanged, this, &Node::nodeChanged);
        connect (widget, &ExpressionWidget::expressionChanged, this, &Node::parameterChanged);
        ((QFormLayout*) _panel->layout ()) -> addRow (label, widget);
        _parameters.insert (name, widget);
        widget->setValidator (validator);
        widget->setText (QString::number (initial));
        return widget;
    } else {
        return nullptr;
    }
}

void Node::parameterChanged() {
    invalidate();
}

void Node::setParameter (const QString& name, const QString& text) {
    _parameters.find (name).value() -> setText (text);
}

void Node::setParameter (const QString& name, const double& value) {
    _parameters.find (name).value() -> setText (QString::number (value));
}

QString Node::parameter (const QString& name) {
    return _parameters.value (name) -> text();
}


double Node::parameterValue (const QString& name) {
    return _parameters.value (name) -> value();
}

QStringList Node::parameters () {
    return _parameters.keys();
}

void Node::addContentPanel() {
    _contentLayout = new QFormLayout();
    _contentLayout -> setContentsMargins (5, 0, 5, 0);
    _contentLayout -> setVerticalSpacing (0);
    _content = new QWidget (_expander);
    _content -> setLayout (_contentLayout);
    addPanel (tr ("Parameters"), _content);
}

int Node::id () {
    return _id;
}

Port* Node::output () {
    return _output;
}

NodeBlock* Node::makeHandle() {
    _handle = new NodeBlock (this);
    _handle -> initialise();
    return _handle;
}

Node* Node::clone() {
    QDomDocument doc;
    QDomElement root = doc.createElement ("clone");
    doc.appendChild (root);
    serialize (doc);
    Node* _copy = CalenhadServices::modules() -> createModule (nodeType(), _model);
    _copy -> setModel (_model);
    _copy -> inflate (doc.documentElement().firstChildElement ("module"));
    _copy -> setName (_model -> uniqueName (_name));

    return _copy;
}

void Node::connectMenu (QMenu* menu, Port* p) {
    int portType = p -> portType ();

        if (portType == Port::OutputPort) {
            if (! _connectMenu) { _connectMenu = new QMenu(); }
            _connectMenu -> clear();
            _connectMenu -> setTitle (name());
            for (Port* port : _ports) {
                if (port -> portType() != Port::OutputPort) {
                    QAction* action = new QAction();
                    action -> setText (port -> portName());
                    _connectMenu -> addAction (action);
                    connect (action, &QAction::triggered, this, [=] () {
                        CreateConnectionCommand* command = new CreateConnectionCommand (p, port, _model);
                        _model -> controller() -> doCommand (command);
                    });
                }
            }
            menu -> addMenu (_connectMenu);
        } else {
            QAction* action = new QAction();
            action -> setText (name());
            connect (action, &QAction::triggered, this, [=] () {
                CreateConnectionCommand* command = new CreateConnectionCommand (output(), p, _model);
                _model -> controller() -> doCommand (command);
            });
            menu -> addAction (action);
        }
}

void Node::addInputPorts() {

}

void Node::addDependentNodes () {

}
