//
// Created by martin on 16/12/16.
//

#include "QNode.h"
#include <iostream>
#include "../nodeedit/qneconnection.h"
#include <QMenu>
#include "../nodeedit/CalenhadController.h"
#include "QModule.h"
#include "../pipeline/CalenhadModel.h"
#include "../actions/ChangeModuleCommand.h"
#include "qwt/qwt_dial.h"
#include "qwt/qwt_dial_needle.h"
#include "qwt/qwt_counter.h"
#include "../CalenhadServices.h"
#include "../nodeedit/QNodeBlock.h"


QNode::QNode (QWidget* parent) : QWidget (parent), _model (nullptr), _isInitialised (false), _dialog (nullptr) {
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

    if (hasParameters()) {
        _contentLayout = new QFormLayout ();
        _contentLayout->setMargin (0);
        _contentLayout->setVerticalSpacing (0);
        _content = new QWidget (_expander);
        _content->setLayout (_contentLayout);
        QNode::addPanel (tr ("Parameters"), _content);
    }
    // when we change panels, move the focus to the newly-shown panel - this removes the focus from the parameter controls and causes their
    // values to be updated to the underlying noise owner data
    connect (_expander, &QToolBox::currentChanged, this, [=] () { _expander -> currentWidget() -> findChild<QWidget*>() -> setFocus(); });
}

QString QNode::name() {
    return _name;
}

void QNode::setName (const QString& name) {
    if (! (name.isNull()) && (name != _name)) {
        _name = name;
        _nameEdit -> setText (name);
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
    return isComplete();
    // other conditions to do
}

// Spin _box which selects a libnoiseutils level value between -1 and 1
QDoubleSpinBox* QNode::noiseValueParamControl (const QString& text, const QString& property) {
    if (property == QString::null) { return noiseValueParamControl (text, propertyName (text)); }
    QDoubleSpinBox* spin = new QDoubleSpinBox (_content);
    spin -> setRange (-1.0, 1.0);
    spin -> setSingleStep (0.1);
    spin -> setToolTip (text);

    connect (spin, &QDoubleSpinBox::editingFinished, this, [=] () { propertyChangeRequested (property, spin -> value()); });
    return spin;
}

// Spin _box which selects an iteration or octave count between 1 and 12
QSpinBox* QNode::countParameterControl (const QString& text, const QString& property) {
    if (property == QString::null) { return countParameterControl (text, propertyName (text)); }
    QSpinBox* spin = new QSpinBox (_content);
    spin -> setRange (1, 12);
    spin -> setToolTip (text);
    connect (spin, &QSpinBox::editingFinished, this, [=] () { propertyChangeRequested (property, spin -> value()); });
    return spin;
}

// Spin _box which selects an angle between + / - 180 degrees
QwtCounter* QNode::angleParameterControl (const QString& text, const QString& p) {
    if (p == QString::null) { return angleParameterControl (text, propertyName (text)); }
    QwtCounter *counter = new QwtCounter (this);
    counter -> setWrapping (true);
    counter -> setRange (-180.0, 180.0);
    counter -> setSingleStep (1.0);
    counter -> setNumButtons (3);
    counter -> setIncSteps (QwtCounter::Button1, 1);
    counter -> setIncSteps (QwtCounter::Button2, 15);
    counter -> setIncSteps (QwtCounter::Button3, 45);

    counter -> setToolTip (text);
    connect (counter, &QwtCounter::valueChanged, this, [=] () {
        double value = counter -> value();
        if (value == counter -> maximum()) { value = counter -> minimum(); }
        else if (value == counter -> minimum()) { value = counter -> maximum(); }
        if (! ((property (p.toStdString().c_str())  == counter -> minimum() && value == counter -> maximum()) ||
                property (p.toStdString().c_str()) == counter -> maximum () && value == counter -> minimum())) {
            propertyChangeRequested (p, value);
        }
    });

    return counter;
}

QLogSpinBox* QNode::logParameterControl (const QString& text, const QString& property) {
    if (property == QString::null) { return logParameterControl (text, propertyName (text)); }
    QLogSpinBox* spin = new QLogSpinBox (_content);
    connect (spin, &QDoubleSpinBox::editingFinished, this, [=] () { propertyChangeRequested (property, spin -> value()); });
    return spin;
}

void QNode::invalidate() {
    emit nodeChanged();
}

void QNode::setModel (CalenhadModel* model) {
    if (! _model) {
        _model = model;
        emit nodeChanged();
        //_model -> controller () -> addParamsWidget (_dialog, this);
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
    QString name = notesNode.text();
    QDomNodeList portNodes = element.elementsByTagName ("port");
    for (int i = 0; i < portNodes.count(); i++) {
        bool okIndex, okType;
        int portIndex = portNodes.at (i).attributes ().namedItem ("index").nodeValue ().toInt (&okIndex);
        int portType = portNodes.at (i).attributes ().namedItem ("type").nodeValue ().toInt (&okType);
        QDomElement portNameNode = portNodes.at (i).firstChildElement ("name");
        QString name = portNameNode.text();
        if (okIndex && okType) {
            for (QNEPort* p : _ports) {
                if (p -> index() == portIndex && p -> portType() == portType) {
                    p -> setName (name);
                }
            }
        } else {
            QString m = "Can't find " + portNodes.at (i).attributes ().namedItem ("type").nodeValue() + " port with index " + portNodes.at (i).attributes ().namedItem ("index").nodeValue() + " in owner " + _name;
            CalenhadServices::messages() -> message ("warning", "Reverting to default port names. " + m);
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
    return true;
}

void QNode::setGroup (QNodeGroup* group) {
    _group = group;
}

QNodeGroup* QNode::group () {
    return _group;
}

