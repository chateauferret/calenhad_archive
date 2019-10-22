//
// Created by martin on 16/11/16.
//


#include "Module.h"
#include "nodeedit/NodeBlock.h"
#include "../module/NodeGroup.h"
#include "nodeedit/Port.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../legend/LegendService.h"
#include "../legend/Legend.h"
#include "../pipeline/ModuleFactory.h"
#include <QDialogButtonBox>
#include <controls/globe/CalenhadGlobeWidget.h>
#include <controls/globe/CalenhadStatsPanel.h>
#include <controls/globe/CalenhadGlobeDialog.h>
#include <mapping/CalenhadMapWidget.h>
#include "nodeedit/Connection.h"
#include "../nodeedit/CalenhadView.h"
#include "mapping/CalenhadMapWidget.h"
#include "../nodeedit/CalenhadController.h"
#include "../icosphere/icosphere.h"

using namespace calenhad::icosphere;
using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::pipeline;
using namespace calenhad::legend;
using namespace calenhad::mapping;
using namespace calenhad::notification;
using namespace calenhad::expressions;

int Module::seed = 0;

Module::Module (const QString& nodeType, QWidget* parent) : Node (nodeType, parent),
                                                            _globe (nullptr),
                                                            _shownParameter (QString::null),
                                                            _suppressRender (false),
                                                            _connectMenu (new QMenu()),
                                                            _preview (nullptr),
                                                            _vertexBuffer (nullptr),
                                                            _stats (nullptr)   {
    _legend = CalenhadServices::legends() -> defaultLegend();
    initialise();
}


Module::~Module () {
    _suppressRender = true;
    if (_preview) { delete _preview; }
    if (_globe) { delete _globe; }
    if (_stats) { delete _stats; }
    if (_connectMenu) { delete _connectMenu; }
}

/// Initialise a QModule ready for use. Creates the UI.
void Module::initialise() {
    Node::initialise();
    _ports.clear();
    addInputPorts();
    // all modules have an output
    Port* output = new Port (Port::OutputPort, 0, "Output", QString::null);
    addPort (output);
    setContextMenuPolicy(Qt::CustomContextMenu);
    _contextMenu = new QMenu (this);
    QAction* globeAction =  new QAction (QIcon (":/appicons/controls/globe.png"), "Show globe");
    connect (globeAction, &QAction::triggered, this, &Module::showGlobe);
    _contextMenu -> addAction (globeAction);
}

void Module::showGlobe() {
    if (! _suppressRender) {
        if (!_globe) {
            _globe = new CalenhadGlobeDialog (this, this);
            _globe -> initialise ();
            _globe -> resize (640, 320);
        }
        _globe -> show();
    }
}

void Module::setupPreview() {
    _preview = new CalenhadMapWidget (RenderMode::RenderModePreview, this);
    _preview -> setSource (this);
    _previewIndex = addPanel (tr ("Preview"), _preview);
    _stats = new QDialog (this);

    _stats -> setLayout (new QVBoxLayout (_stats));
    _statsPanel = new CalenhadStatsPanel (this);
    _stats->layout ()->addWidget (_statsPanel);
    QDialogButtonBox* box = new QDialogButtonBox (QDialogButtonBox::Ok);
    _stats -> layout ()->addWidget (box);
    connect (box, &QDialogButtonBox::accepted, _stats, &QDialog::accept);
    _stats -> setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint);
    _stats -> setMinimumSize (400, 400);
    _stats -> move (_dialog->pos().x() + 400, _dialog->pos().y() + 300);
    connect (_preview, &AbstractMapWidget::rendered, _statsPanel, &CalenhadStatsPanel::refresh);
    QAction* statsAction = new QAction (QIcon (":/appicons/controls/statistics.png"), "Statistics");
    connect (statsAction, &QAction::triggered, _stats, &QWidget::show);
    connect (_preview, &QWidget::customContextMenuRequested, this, &Module::showContextMenu);

    _contextMenu -> addAction (statsAction);
}

void Module::showContextMenu (const QPoint& point) {
    _contextMenu -> exec (QCursor::pos());
}

void Module::addInputPort (const unsigned& index, const int& portType, const QString& name, const QString& label, const double& defaultValue, const bool& required) {
    Port* input = new Port (portType, index, name, label, defaultValue, required);

    // unless a port must be filled, provide an ExpressionWidget so that a constant value can be provided instead of a source module
    if (! required) {
        ExpressionWidget* param = addParameter (name, name, defaultValue, new AcceptAnyRubbish (), _content);

        // if a port is connected to an input, that input supersedes any value the user provides at the parameter's widget, so disable it
        connect (input, &Port::connected, this, [=] () { param->setEnabled (false); });
        connect (input, &Port::disconnected, this, [=] () { param->setEnabled (true); });
    }
    addPort (input, index);
}


Node* Module::sourceModule (int portIndex) {
    Port* p = _ports.at (portIndex);
    if (p -> portType () == Port::OutputPort || p -> connections().isEmpty()) {
        return nullptr;
    }
    Connection* c = p -> connections().first();
    return c -> otherEnd (p) -> owner();
}

void Module::inflate (const QDomElement& element) {
    Node::inflate (element);
    QString legendName = element.attribute ("legend", "default");
    _legend = CalenhadServices::legends() -> find (legendName);
    // position is retrieved in CalenhadModel
}

void Module::serialize (QDomElement& element) {
    Node::serialize (element);
    QDomDocument doc = element.ownerDocument();
    _element.setAttribute ("legend", _legend -> name());
}

void Module::rendered() {

}

void Module::setModel (CalenhadModel* model) {
    Node::setModel (model);
    setupPreview ();
}

void Module::setLegend (Legend* legend) {
    _legend = legend;
    emit nodeChanged();
}

Legend* Module::legend () {
    return _legend;
}

bool Module::generateMap () {
    return true;
}

bool Module::isComplete() {
    if (! _suppressRender) {
        for (Port* p : _ports) {
            if (p->isRequired () && !p->hasConnection ()) {
                return false;
            }
        }

        bool complete = true;
        QList<ExpressionWidget*> widgets = findChildren<ExpressionWidget*> ();
        if (!(widgets.isEmpty ())) {
            for (ExpressionWidget* ew: widgets) {
                if (!ew->isValid ()) {
                    for (Port* p : _ports) {
                        if (p->portName () == ew->objectName () && p->portType () != Port::OutputPort) {
                            if (!(p->hasConnection ())) {
                                complete = false;
                                break;
                            }
                        }
                    }
                }
            }
        }

        _expander->setItemEnabled (_previewIndex, complete);
        return complete;
    } else {
        return true;
    }
}

void Module::contextMenuEvent (QContextMenuEvent* e) {
    _contextMenu -> exec();
}

bool Module::range (double& min, double& max) {
    if (_preview) {
        Statistics statistics = _preview -> statistics ();
        min = statistics._min;
        max = statistics._max;
        return true;
    } else {
        return false;
    }
}

CalenhadMapWidget* Module::preview() {
    return _preview;
};

void Module::parameterChanged() {
    Node::parameterChanged();
    if (sender() == _parameters.find (_shownParameter).value()) {
        _block -> setText (_parameters.find (_shownParameter).value() -> text());
    }
}

void Module::invalidate() {
    if (! _suppressRender) {
        Node::invalidate ();

        // if this node needs recalculating or rerendering, so do any nodes that depend on it -
        // that is any nodes with an input connected to this one's output
        for (Module* dependant : dependants ()) {
            dependant->invalidate ();
        }
        if (_globe && _globe->isVisible ()) {
            _globe->update ();
        }
        if (_statsPanel) {
            _statsPanel->refresh ();
        }
    }
}

QSet<Module*> Module::dependants() {

    QSet<Module*> found;
    if (! _output -> connections(). isEmpty ()) {
        foreach (Connection* c, _output->connections()) {
            if (c) {
                Port* p = c->otherEnd (_output);
                if (p) {
                    found.insert (p->owner ());
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
    return CalenhadServices::modules() -> glsl (_nodeType);
}

QMap<unsigned, Port*> Module::inputs () {
    return _inputs;
}

void Module::addPort (Port* port, const unsigned& index) {
    _ports.append (port);
    if (port -> portType () == Port::OutputPort) {
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

void Module::addInputPorts () {

}

bool Module::renderSuppressed () {
    return _suppressRender;
}

void Module::suppressRender (bool suppress) {
    _suppressRender = suppress;
}

QColoredIcon* Module::icon () {
    return ((NodeBlock*) handle()) -> icon();
}

float* Module::vertexBuffer() {
    if (! _vertexBuffer) {
        _vertexBuffer = CalenhadServices::icosphere() -> vertexBuffer();
    }

    //_preview -> computeVertices (_vertexBuffer);
    return _vertexBuffer;
}