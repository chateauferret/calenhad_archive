//
// Created by martin on 16/12/16.
//
#include "QNode.h"
#include <QDialog>
#include <QString>
#include <QCloseEvent>
#include <iostream>
#include "../nodeedit/qneconnection.h"
#include <QMenu>
#include <nodeedit/NodeNameValidator.h>
#include "../nodeedit/CalenhadController.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../pipeline/ModuleFactory.h"
#include "../nodeedit/qneport.h"
#include "../messages/QNotificationService.h"
#include "../nodeedit/QNodeBlock.h"
#include "../actions/ChangeModuleCommand.h"
#include "../preferences/PreferencesService.h"

using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::pipeline;
using namespace calenhad::expressions;
using namespace calenhad::actions;

QNode::QNode (const QString& nodeType, int inputs, QWidget* parent) : QWidget (parent),
    _model (nullptr),
    _dialog (nullptr),
    _handle (nullptr),
    _content (nullptr),
    _contentLayout (nullptr),
    _palette (nullptr),
    _validator (nullptr),
    _inputCount (inputs),
    _nodeType (nodeType) {
}


QNode::~QNode () {
    if (_dialog) { delete _dialog; }
    if (_validator) { delete _validator; }
    if (_palette) { delete _palette; }
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
    connect (this, &QNode::nameChanged, this, [=] () { _nameEdit -> setText (_name); });

    _notesEdit = new QTextEdit (about);
    _notesEdit -> setFixedHeight (100);
    layout -> addWidget (_notesEdit);
    layout -> addStretch (0);

    connect (_notesEdit, &QTextEdit::textChanged, this, [=] () {
        propertyChangeRequested ("notes", _notesEdit -> document() -> toPlainText());
    });
    connect (this, &QNode::notesChanged, this, [=] () { _notesEdit -> setText (_notes); });

    addPanel ("About", about);
    QLayout* l = new QVBoxLayout();
    l -> setSpacing (0);
    l -> setMargin (5);
    _dialog = new QDialog (this);
    _dialog -> setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint | Qt::WindowMinimizeButtonHint);
    _dialog -> setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
    _dialog -> setLayout (new QVBoxLayout());
    _dialog -> layout() -> addWidget (_expander);

    // when we change panels, move the focus to the newly-shown panel - this removes the focus from the parameter controls and causes their
    // values to be updated to the underlying noise owner data
    connect (_expander, &QToolBox::currentChanged, this, [=] () {
        QWidget* w = _expander -> currentWidget() -> findChild<QWidget*>();
        if (w) {
            w -> setFocus();
        }
    });
}

QString QNode::name() {
    return _name;
}

// setName assumes the name has been validated using CalenhadModel::validateName - it is the caller's
// responsibility to call this first (since we don't know if the caller wants messages).
void QNode::setName (const QString& name) {
    if (! (name.isNull()) && (name != _name)) {
        _name = name;
        _nameEdit -> setText (_name);
        update();
        if (_dialog) {
            _dialog -> setWindowTitle (_name + " (" + nodeType () + ")");
        }
        emit (nameChanged (name));
    }
}

void QNode::setNotes (const QString& notes) {
    if (! notes.isNull()) {
        if (! (notes == _notesEdit -> toPlainText())) {
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


void QNode::setHandle (QNodeBlock* h) {
    _handle = h;
}

QNodeBlock* QNode::handle() {
    return _handle;
}

void QNode::addPort (QNEPort* port) {
    _ports.append (port);
    if (port -> portType () == QNEPort::OutputPort) {
        _output = port;
    }
}

QList<QNEPort*> QNode::ports() {
    return _ports;
}

bool QNode::isComplete() {
    bool complete = true;
    QList<ExpressionWidget *> widgets = findChildren<ExpressionWidget*>();
    for (ExpressionWidget* ew: widgets) {
        if (! ew -> isValid()) {
            complete = false;
            break;
        }
    }
    if (complete) {
        for (QNEPort* p : _ports) {
            if (p->portType () != QNEPort::OutputPort) {
                if (!(p->hasConnection ())) {
                    complete = false;
                    break;
                }
            }
        }
    }
    return complete;
}

QDoubleSpinBox* QNode::addParameter (const QString& text, const QString& property) {
    if (property == QString::null) { return addParameter (text, propertyName (text)); }
    QDoubleSpinBox* spin = new QDoubleSpinBox (_content);
    connect (spin, &QDoubleSpinBox::editingFinished, this, [=] () { propertyChangeRequested (property, spin -> value()); });
    return spin;
}

void QNode::invalidate() {
    emit nodeChanged();
}

void QNode::setModel (CalenhadModel* model) {
    if (! _model) {
        _model = model;
        for (ExpressionWidget* widget : _parameters.values ()) {
            connect (widget, &ExpressionWidget::expressionChanged, this, [=] () { if (_handle) { _handle -> update(); } });
            connect (widget, &ExpressionWidget::compiled, this, &QNode::nodeChanged);
            connect (widget, &ExpressionWidget::errorFound, this, &QNode::nodeChanged);
            connect (widget, &ExpressionWidget::expressionChanged, this, &QNode::nodeChanged);
        }

        emit nodeChanged();

    } else {
        std::cout << "Can't reassign owner to another model";
    }
}

void QNode::showParameters (const bool& visible) {
    if (visible) {
        _dialog -> setWindowTitle (name () + " (" + nodeType () + ")");
        _dialog -> setAttribute (Qt::WA_DeleteOnClose, false);
        _dialog -> show();
    } else {
        _dialog -> hide();
    }
}

void QNode::inflate (const QDomElement& element) {
    _element = element;
        QDomElement notesNode = element.firstChildElement ("name");
        QString name = notesNode.text ();
        QDomNodeList portNodes = element.elementsByTagName ("port");
        for (int i = 0; i < portNodes.count (); i++) {
            bool okIndex, okType;
            int portIndex = portNodes.at (i).attributes ().namedItem ("index").nodeValue ().toInt (&okIndex);
            int portType = portNodes.at (i).attributes ().namedItem ("type").nodeValue ().toInt (&okType);
            QDomElement portNameNode = portNodes.at (i).firstChildElement ("name");
            QString name = portNameNode.text ();
            if (okIndex && okType) {
                for (QNEPort* p : _ports) {
                    if (p->index () == portIndex && p->portType () == portType) {
                        p->setName (name);
                    }
                }
            } else {
                QString m = "Can't find " + portNodes.at (i).attributes ().namedItem ("type").nodeValue () + " port with index " +
                            portNodes.at (i).attributes ().namedItem ("index").nodeValue () + " in owner " + _name;
                CalenhadServices::messages ()->message ("warning", "Reverting to default port names. " + m);
            }
        }
        QDomNodeList paramNodes = element.elementsByTagName ("parameter");
        for (int i = 0; i < paramNodes.count (); i++) {
            QString paramName = paramNodes.at (i).attributes ().namedItem ("name").nodeValue ();
            QString paramValue = paramNodes.at (i).attributes ().namedItem ("value").nodeValue ();
            setParameter (paramName, paramValue);
        }

}

void QNode::serialize (QDomDocument& doc) {
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
    for (QNEPort* p : _ports) {
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

void QNode::propertyChangeRequested (const QString& p, const QVariant& value) {
    if (_model && (property (p.toStdString ().c_str ()) != value)) {
        ChangeModuleCommand* c = new ChangeModuleCommand (this, p, property (p.toStdString ().c_str ()), value);
        if (_model) {
            _model -> controller() -> doCommand (c);
        }
        _model -> update();
    }
}

void QNode::showEvent (QShowEvent* event) {
    _nameEdit -> setText (_name);
    QWidget::showEvent (event);
}

CalenhadModel* QNode::model () {
    return _model;
}

QString QNode::propertyName (const QString& name) {
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

void QNode::closeEvent (QCloseEvent* event) {
    event -> ignore();
    showParameters (false);
}

bool QNode::hasParameters () {
    return ! (_parameters.isEmpty());
}

void QNode::setGroup (QNodeGroup* group) {
    _group = group;
}

QString QNode::nodeType() {
    return _nodeType;
}

QNodeGroup* QNode::group () {
    return _group;
}

ExpressionWidget* QNode::addParameter (const QString& label, const QString& name, const double& initial, ParamValidator* validator) {

    // create a panel to hold the parameter widgets, if we haven't done this already
    if (! (_content)) {
        addContentPanel();
    }

    ExpressionWidget* widget = new ExpressionWidget (this);
    _contentLayout -> addRow (label, widget);

    _parameters.insert (name, widget);
    widget -> setValidator (validator);
    widget -> setText (QString::number (initial));
    return widget;
}

void QNode::setParameter (const QString& name, const QString& text) {
    _parameters.find (name).value() -> setText (text);
}

QString QNode::parameter (const QString& name) {
    return _parameters.value (name) -> text();
}


QStringList QNode::parameters () {
    return _parameters.keys();
}

QNode* QNode::clone () {
    return CalenhadServices::modules() -> clone (this);
}

void QNode::addContentPanel() {
    _contentLayout = new QFormLayout ();
    _contentLayout->setContentsMargins (5, 0, 5, 0);
    _contentLayout->setVerticalSpacing (0);
    _content = new QWidget (_expander);
    _content->setLayout (_contentLayout);
    addPanel (tr ("Parameters"), _content);
}

int QNode::id () {
    return _id;
}

calenhad::nodeedit::QNEPort* QNode::output () {
    return _output;
}
