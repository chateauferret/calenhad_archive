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
#include "../exprtk/Calculator.h"

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
                                                            _valid (false), _minExpr (QString()), _maxExpr (QString()),
                                                            _output (nullptr),
                                                            _shownParameter (QString()),
                                                            _connectMenu (new QMenu()) {
    _ports.clear();
    // all modules have an output
    Port* output = new Port (Port::OutputPort, 0, "Output", QString());
    addPort (output);
    QPushButton* globeButton = new QPushButton();
    QIcon globeIcon (":/appicons/controls/globe.png");
    globeButton -> setIcon (globeIcon);
    connect (globeButton, &QPushButton::pressed, this, [=] () { CalenhadServices::globe (this) -> show(); });
    connect (this, &Node::nodeChanged, this, [=] () { globeButton -> setEnabled (isComplete()); });
    _topPanel -> layout() -> addWidget (globeButton);

    QWidget* minMax = new QWidget();
    QGridLayout* minMaxLayout = new QGridLayout();
    minMax -> setLayout (minMaxLayout);
    QLabel* minCaption = new QLabel ("Minimum value");
    minMaxLayout -> addWidget (minCaption, 0, 0);
    QLabel* maxCaption = new QLabel ("Maximum value");
    minMaxLayout -> addWidget (maxCaption, 1, 0);
    _minLabel = new QLabel();
    minMaxLayout -> addWidget (_minLabel, 0, 1);
    _maxLabel = new QLabel();
    minMaxLayout -> addWidget (_maxLabel, 1, 1);
    _about -> layout() -> addWidget (minMax);

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
            if (p -> isRequired() && !p -> hasConnection()) {
                return false;
            }
        }

        bool complete = true;
        QList<ExpressionWidget*> widgets = findChildren<ExpressionWidget*> ();
        if (!(widgets.isEmpty())) {
            for (ExpressionWidget* ew: widgets) {
                if (! ew -> isValid()) {
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
    QWidget* widget = _parameters.find (_shownParameter).value();
    if (sender() == widget) {
        ExpressionWidget* w = dynamic_cast<ExpressionWidget*> (widget);
        _block -> setText (w ? w -> text () : ((QComboBox*) widget) -> currentText ());
    }
}

void Module::invalidate() {
    Node::invalidate();
    updateMetrics();
    _valid = false;
    for (Module* dependant : dependants()) {
        dependant -> invalidate();
    }
}

void Module::updateMetrics() {
    QString minExpr = _minExpr;
    minExpr.detach();
    QString maxExpr = _maxExpr;
    maxExpr.detach();
    // replace the input module and parameter markers with their names to replace with values
    int i = 0;
    for (QString key : parameters()) {
        minExpr.replace ("%" + key, QString::number (parameterValue (key)));
        maxExpr.replace ("%" + key, QString::number (parameterValue (key)));
    }
    for (Port* port : inputs()) {
        QString index = QString::number (i++);
        if (port -> connections().isEmpty()) {
            minExpr.replace ("%" + index + ".min", QString::number (parameterValue (port -> portName())));
            minExpr.replace ("%" + index + ".max", QString::number (parameterValue (port -> portName())));
            maxExpr.replace ("%" + index + ".min", QString::number (parameterValue (port -> portName())));
            maxExpr.replace ("%" + index + ".max", QString::number (parameterValue (port -> portName())));
        } else {
            Node* other = port -> connections() [0] -> otherEnd (port) -> owner();
            Module* source = dynamic_cast<Module*> (other);
            minExpr.replace ("%" + index + ".min", QString::number (source -> min()));
            maxExpr.replace ("%" + index + ".min", QString::number (source -> min()));
            minExpr.replace ("%" + index + ".max", QString::number (source -> max()));
            maxExpr.replace ("%" + index + ".max", QString::number (source -> max()));

        }
    }
    if (!_minExpr.isNull() && !_minExpr.isEmpty()) {
        _min = (float) CalenhadServices::calculator() -> compute (minExpr);
        _minLabel -> setText (QString::number (_min));
    }
    if (!_maxExpr.isNull() && !_maxExpr.isEmpty()) {
        _max = (float) CalenhadServices::calculator() -> compute (maxExpr);
        _maxLabel -> setText (QString::number (_max));
    }
}

QSet<Module*> Module::dependants() {

    QSet<Module*> found;
    if (! _output -> connections(). isEmpty()) {
        for (Connection* c : _output -> connections()) {
            if (c) {
                Port* p = c -> otherEnd (_output);
                if (p) {
                    found.insert (p -> owner());
                }
            }
        }
    }
    return found;
}

QString Module::label() {
    return CalenhadServices::modules() -> label (_nodeType);
}

QString Module::description() {
    return CalenhadServices::modules() -> description (_nodeType);
}

void Module::inflate (const QDomElement& element) {
    Node::inflate (element);
}


QString Module::glsl () {
    QString code = CalenhadServices::modules() -> glsl (_nodeType);
    expandCode (code);
    return code;
}

void Module::expandCode (QString& code) {

    for (const QString& key : parameters()) {
        code.replace ("%" + key, QString::number (parameterValue (key)));
    }
    // replace the input module markers with their names referencing their member variables in glsl
    int i = 0;
    for (Port* port : inputs ()) {
        QString index = QString::number (i++);
        if (port -> connections().isEmpty ()) {
            code.replace ("%" + index, QString::number (parameterValue (port -> portName())));
            code.replace (".min", "");
            code.replace (".max", "");
        } else {
            Node* other = port -> connections() [0] -> otherEnd (port) -> owner();
            Module* m = dynamic_cast<Module*> (other);
            QString source = other -> name();
            if (m) {
                code.replace ("%" + index + ".min", QString::number (m -> min()));
                code.replace ("%" + index + ".max", QString::number (m -> max()));
            }
            code.replace ("%" + index, "_" + source + " (pos, c, g)");
            code.replace ("$" + index, "_" + source);
        }
    }

    // fill in attribute values by looking for words beginning with % and replacing them with the parameter values from the XML
    for (const QString& param : CalenhadServices::modules() -> paramNames()) {
        if (parameters().contains (param)) {
            code.replace ("%" + param, QString::number (parameterValue (param)));
        }
    }
}

void Module::fetch (CubicSphere* buffer) {
    std::cout << "Module :: fetch - " << name().toStdString() << " - " << buffer << "\n";
    CalenhadServices::compute() -> compute (this, buffer);
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
    if (p -> portType() == Port::OutputPort) {
        _connectMenu -> clear();
        _connectMenu -> setTitle (name());
        for (Port* port : _ports) {
            if (port -> portType() != Port::OutputPort) {
                if (_model -> canConnect (p, port)) {
                    QAction* action = new QAction();
                    action -> setText (port -> portName ());
                    _connectMenu -> addAction (action);
                    connect (action, &QAction::triggered, this, [=] () {
                        _model -> createConnection (p, port);
                    });
                }
            }
        }
        if (! _connectMenu -> isEmpty()) {
            menu -> addMenu (_connectMenu);
        }
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

bool Module::isComputed () {
    return false;
}

float Module::min() {
    return _min;
}


float Module::max() {
    return _max;
}

void Module::setMinExpr (const QString& expr) {
    _minExpr = expr;
}

void Module::setMaxExpr (const QString& expr) {
    _maxExpr = expr;
}

