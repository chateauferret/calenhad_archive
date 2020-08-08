//
// Created by martin on 16/11/16.
//

#include "Module.h"
#include "nodeedit/NodeBlock.h"
#include "nodeedit/Port.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "src/CalenhadServices.h"
#include "../legend/LegendService.h"
#include "../pipeline/ModuleFactory.h"
#include <QDialogButtonBox>
#include "../mapping/projection/ProjectionService.h"
#include "../nodeedit/Connection.h"

#include <algorithm>

using namespace calenhad::grid;
using namespace calenhad::graph;
using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::pipeline;
using namespace calenhad::legend;
using namespace calenhad::mapping;
using namespace calenhad::notification;
using namespace calenhad::expressions;

Module::Module (const QString& nodeType, QWidget* parent) : Node (nodeType, parent),
                                                            _valid (false),
                                                            _output (nullptr),
                                                            _shownParameter (QString::null),
                                                            _connectMenu (new QMenu()) {
    _ports.clear();
    // all modules have an output
    Port* output = new Port (Port::OutputPort, 0, "Output", QString::null);
    addPort (output);
    QPushButton* globeButton = new QPushButton();
    QIcon globeIcon (":/appicons/controls/globe.png");
    globeButton -> setIcon (globeIcon);
    connect (globeButton, &QPushButton::pressed, this, [=] () { CalenhadServices::globe (this) -> show(); });
    connect (this, &Node::nodeChanged, this, [=] () { globeButton -> setEnabled (isComplete()); });
    _topPanel -> layout() -> addWidget (globeButton);
}


Module::~Module() {
    close();
    delete _connectMenu;
}

void Module::showGlobe() {
    CalenhadGlobeDialog* dialog = CalenhadServices::globe (this);
    dialog -> show();
}

void Module::addInputPort (const unsigned& index, const int& portType, const QString& name, const QString& label, const double& defaultValue, const bool& required) {
    Port* input = new Port (portType, index, name, label, defaultValue, required);

    // unless a port must be filled, provide an ExpressionWidget so that a constant value can be provided instead of a source module
    if (! required) {
        ExpressionWidget* param = addParameter (name, name, defaultValue, new AcceptAnyRubbish (), _content);

        // if a port is connected to an input, that input supersedes any value the user provides at the parameter's widget, so disable it
        connect (input, &Port::connected, this, [=] () { param -> setEnabled (false); });
        connect (input, &Port::disconnected, this, [=] () { param -> setEnabled (true); });
    }
    addPort (input, index);
}

void Module::setModel (CalenhadModel* model) {
    Node::setModel (model);
}

bool Module::isComplete() {
        for (Port* p : _ports) {
            if (p -> isRequired() && !p -> hasConnection ()) {
                return false;
            }
        }

        bool complete = true;
        QList<ExpressionWidget*> widgets = findChildren<ExpressionWidget*> ();
        if (!(widgets.isEmpty())) {
            for (ExpressionWidget* ew: widgets) {
                if (!ew->isValid ()) {
                    for (Port* p : _ports) {
                        if (p -> portName() == ew -> objectName() && p -> portType() != Port::OutputPort) {
                            if (! (p -> hasConnection())) {
                                complete = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
        return complete;
}


void Module::parameterChanged() {
    Node::parameterChanged();
    if (sender() == _parameters.find (_shownParameter).value()) {
        _block -> setText (_parameters.find (_shownParameter).value() -> text());
    }
}

void Module::invalidate() {
    Node::invalidate();
    _valid = false;
    for (Module* dependant : dependants()) {
        dependant -> invalidate();
    }
}

QSet<Module*> Module::dependants() {

    QSet<Module*> found;
    if (! _output -> connections(). isEmpty()) {
        for (Connection* c : _output -> connections()) {
            if (c) {
                Port* p = c->otherEnd (_output);
                if (p) {
                    found.insert (p -> owner());
                }
            }
        }
    }
    return found;
}

QString Module::label () {
    return CalenhadServices::modules() -> label (_nodeType);
}

QString Module::description () {
    return CalenhadServices::modules() -> description (_nodeType);
}

QString Module::glsl () {
    QString code = CalenhadServices::modules() -> glsl (_nodeType);

    // replace the input module markers with their names referencing their member variables in glsl
    int i = 0;
    for (Port* port : inputs ()) {
        QString index = QString::number (i++);
        if (port -> connections ().isEmpty ()) {
            code.replace ("%" + index, QString::number (parameterValue (port->portName ())));
        } else {
            Node* other = port -> connections() [0] -> otherEnd (port) -> owner ();
            QString source = other -> name();
        }
    }

    // fill in attribute values by looking for words beginning with % and replacing them with the parameter values from the XML
    for (const QString& param : CalenhadServices::modules() -> paramNames()) {
        if (parameters().contains (param)) {
            code.replace ("%" + param, QString::number (parameterValue (param)));
        }
    }
    return code;
}

QMap<unsigned, Port*> Module::inputs() {
    return _inputs;
}

void Module::addPort (Port* port, const unsigned& index) {
    _ports.append (port);
    if (port -> portType() == Port::OutputPort) {
        _output = port;
    } else {
        _inputs.insert (index, port);
    }
}

QVector<Port*> Module::ports() {
    return _ports;
}

void Module::connectMenu (QMenu* menu, Port* p) {
    int portType = p -> portType ();

    if (portType == Port::OutputPort) {
        _connectMenu -> clear();
        _connectMenu -> setTitle (name());
        for (Port* port : _ports) {
            if (port -> portType() != Port::OutputPort) {
                QAction* action = new QAction();
                action -> setText (port -> portName());
                _connectMenu -> addAction (action);
                connect (action, &QAction::triggered, this, [=] () {
                    _model -> createConnection (p, port);
                });
            }
        }
        menu -> addMenu (_connectMenu);
    } else {
        QAction* action = new QAction();
        action -> setText (name());
        connect (action, &QAction::triggered, this, [=] () {
            _model -> createConnection (output(), p);
        });
        menu -> addAction (action);
    }
}


Port* Module::output () {
    return _output;
}



