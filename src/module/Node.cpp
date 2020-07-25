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
#include <QtWidgets/QComboBox>
#include <utility>
#include "NodeGroup.h"
#include "../nodeedit/CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "src/CalenhadServices.h"
#include "../pipeline/ModuleFactory.h"
#include "nodeedit/Port.h"
#include "nodeedit/NodeBlock.h"
#include "../actions/ChangeModuleCommand.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::actions;
using namespace calenhad::notification;

Node::Node (QString  nodeType, QWidget* parent) : QWidget (parent),
                                                  _model (nullptr),
                                                  _propertiesWidget (nullptr),
                                                  _block (nullptr),
                                                  _content (nullptr),
                                                  _contentLayout (nullptr),
                                                  _palette (nullptr),
                                                  _validator (nullptr),
                                                  _group (nullptr),
                                                  _propertiesDock (nullptr),
                                                  _nodeType (std::move(nodeType)) {


    _nameEdit = new QLineEdit (this);
    _palette = new QPalette();
    _nameEdit -> setPalette (*_palette);
    _validator = new NodeNameValidator (this);
    connect (_validator, &NodeNameValidator::message, this, [=] (const QString& message) {
        _nameEdit -> setToolTip (message);
        _palette -> setColor (QPalette::Text, CalenhadServices::preferences() -> calenhad_module_text_color_error);
        _nameEdit -> setPalette (*_palette);
    });
    connect (_validator, &NodeNameValidator::success, this, [=] () {
        _nameEdit -> setToolTip (QString::null);
        _palette -> setColor (QPalette::Text, CalenhadServices::preferences() -> calenhad_module_text_color_normal);
        _nameEdit -> setPalette (*_palette);
    });
    _nameEdit -> setValidator (_validator);

    _expander = new QToolBox();
    QWidget* about = new QWidget (_expander);
    QVBoxLayout* layout = new QVBoxLayout();
    layout -> setSpacing (0);
    layout -> setMargin (5);
    about -> setLayout (layout);

    connect (_nameEdit, &QLineEdit::editingFinished, this, [=] () {
        propertyChangeRequested ("name", _nameEdit -> text());
    });
    connect (this, &Node::nameChanged, this, [=] () { _nameEdit -> setText (_name); invalidate(); });

    _notesEdit = new QTextEdit (about);
    _notesEdit -> setFixedHeight (100);
    layout -> addWidget (_notesEdit);
    layout -> addStretch (0);

    connect (_notesEdit, &QTextEdit::textChanged, this, [=] () {
        propertyChangeRequested ("notes", _notesEdit -> document() -> toPlainText());
    });
    connect (this, &Node::notesChanged, this, [=] () {
        if (_notesEdit -> toPlainText () != _notes) {
            _notesEdit -> setText (_notes);
        }
    });

    addPanel ("About", about);
    QLayout* l = new QVBoxLayout();
    l -> setSpacing (0);
    l -> setMargin (5);
    _propertiesWidget = new QWidget();
    _propertiesWidget -> setFixedSize (400, 300);
    _propertiesDock = new QDockWidget();
    _propertiesDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    _propertiesDock -> setWidget (_propertiesWidget);
    _propertiesDock -> layout() -> setAlignment (Qt::AlignLeft);
    _propertiesWidget -> setFixedWidth (240);
    //_propertiesWidget -> setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint | Qt::WindowMinimizeButtonHint);
    _propertiesWidget -> setLayout (new QVBoxLayout());
    _topPanel = new QWidget (_propertiesWidget);
    QHBoxLayout* topPanelLayout = new QHBoxLayout();
    _topPanel -> setLayout (topPanelLayout);
    topPanelLayout -> addWidget (_nameEdit);
    _propertiesWidget -> layout() -> addWidget (_topPanel);
    _propertiesWidget -> layout() -> addWidget (_expander);
    _propertiesWidget -> resize (300, 200);
    // when we change panels, move the focus to the newly-shown panel - this removes the focus from the parameter controls and causes their
    // values to be updated to the underlying noise owner data
    connect (_expander, &QToolBox::currentChanged, this, [=] () {
        QWidget* w = _expander -> currentWidget() -> findChild<QWidget*>();
        if (w) {
            w -> setFocus();
        }
    });

}

Node::~Node () {
    delete _validator;
    delete _palette;
}


QString Node::name() {
    return _name;
}

// setName assumes the name has been validated using CalenhadModel::validateName - it is the caller's
// responsibility to call this first (since we don't know if the caller wants messages).
void Node::setName (const QString& name) {
    if (! (name.isNull()) && (name != _name)) {
        _name = _model -> uniqueName (name);
        _name = _name.replace (" ", "_");
        _nameEdit -> setText (_name);
        update();
        if (_propertiesWidget) {
            _propertiesWidget -> setWindowTitle (_name + " (" + nodeType () + ")");
        }
        emit (nameChanged (name));
    }
}

void Node::setNotes (const QString& notes) {
    if (! notes.isNull()) {
        _notes = notes;
        if (! (notes == _notesEdit -> toPlainText())) {
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

QGraphicsItem* Node::handle() {
    return _block;
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
    return complete;
}

void Node::invalidate() {
    if (_block) {
        _block -> update ();
    }
    updateCache();
    std::cout << "Invalidate " << _name.toStdString() << "\n";
    emit nodeChanged();
}

void Node::setModel (CalenhadModel* model) {
    _model = model;
    //setParent (model -> views () [0]);

}

void Node::showModuleDetail (const bool& visible) {
    if (visible) {
        _propertiesDock -> setWindowTitle (name () + " (" + nodeType () + ")");
        _propertiesDock -> setAttribute (Qt::WA_DeleteOnClose, false);
        CalenhadServices::mainWindow() -> addDockWidget (Qt::LeftDockWidgetArea, _propertiesDock);
        _propertiesDock -> show();
    } else {
        _propertiesWidget -> hide();
    }
}

bool Node::isModuleDetailVisible() {
    return _propertiesWidget -> isVisible();
}

void Node::inflate (const QDomElement& element) {

    // position is retrieved in CalenhadModel

    _element = element;
        QDomElement notesNode = element.firstChildElement ("notes");
        setNotes (notesNode.text ());
        QDomNodeList paramNodes = element.elementsByTagName ("parameter");
        for (int i = 0; i < paramNodes.count (); i++) {
            QString paramName = paramNodes.at (i).attributes ().namedItem ("name").nodeValue ();
            QString paramValue = paramNodes.at (i).attributes ().namedItem ("value").nodeValue ();
            setParameter (paramName, paramValue);
        }
}

void Node::serialize (QDomElement& element) {
    QDomDocument doc = element . ownerDocument();
    _element = doc.createElement ("module");
    element.appendChild (_element);
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

    if (! _parameters.isEmpty()) {
        for (QString key : _parameters.keys()) {
            QDomElement paramElement = doc.createElement ("parameter");
            paramElement.setAttribute ("name", key);
            paramElement.setAttribute ("value", _parameters.value (key) -> text ());
            _element.appendChild (paramElement);
        }
    }

    QDomElement positionElement = doc.createElement ("position");
    _element.appendChild (positionElement);
    positionElement.setAttribute ("y", handle() -> pos().y());
    positionElement.setAttribute ("x", handle() -> pos().x());
    _element.setAttribute ("type", nodeType ());
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
    showModuleDetail (false);
}

bool Node::hasParameters () {
    return ! (_parameters.isEmpty());
}

void Node::setGroup (NodeGroup* g) {
    if (g) {
        _group = g;
    } else {
        _group = nullptr;
    }
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
            addContentPanel();
        }
        _panel = _content;
    }

    if (dynamic_cast<QFormLayout*> (_panel -> layout())) {
        ExpressionWidget* widget = new ExpressionWidget (this);
        widget -> setObjectName (name);
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

void Node::setParameter (const QString& key, const QString& text) {
    QMap<QString, ExpressionWidget*>::iterator item = _parameters.find (key);
    if (item == _parameters.end()) {
        CalenhadServices::messages() -> message ("No such parameter", "Parameter " + key + " not found for node " + name() + " of type " + nodeType(), NotificationStyle::WarningNotification);
    } else {
        item.value ()->setText (text);
    }
}

QString Node::parameter (const QString& name) {
    return _parameters.value (name) -> text();
}

double Node::parameterValue (const QString& name) {
    if (_parameters.keys().contains (name)) {
        return _parameters.value (name)->value ();
    } else {
        return 0.0;
    }
}

QStringList Node::parameters () {
    return _parameters.keys();
}

void Node::addContentPanel() {
    _contentLayout = new QFormLayout();
    _contentLayout -> setContentsMargins (5, 0, 5, 0);
    _contentLayout -> setVerticalSpacing (0);
    _content = new QWidget();
    _content -> setLayout (_contentLayout);
    addPanel (tr ("Parameters"), _content);
}


QGraphicsItem* Node::makeHandle() {
    _block = new NodeBlock (this, nullptr);
    _block -> initialise();
    return _block;
}

Node* Node::clone() {
    QDomDocument doc;
    QDomElement root = doc.createElement ("clone");
    doc.appendChild (root);
    QDomElement element = doc.documentElement();
    serialize (element);
    Node* _copy = CalenhadServices::modules() -> createModule (model() -> uniqueName (name()), nodeType(), _model);
    _copy -> setModel (_model);
    _copy -> inflate (doc.documentElement().firstChildElement ("module"));
    _copy -> setName (_model -> uniqueName (_name));

    return _copy;
}

void Node::updateCache () {

}


