//
// Created by martin on 16/11/16.
//


#include "Module.h"
#include "nodeedit/NodeBlock.h"
#include "nodeedit/Port.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../legend/LegendService.h"
#include "../legend/Legend.h"
#include "../pipeline/ModuleFactory.h"
#include <QDialogButtonBox>
#include <controls/globe/CalenhadGlobeDialog.h>
#include <controls/globe/CalenhadStatsPanel.h>
#include "nodeedit/Connection.h"
#include "../nodeedit/CalenhadView.h"

using namespace icosphere;
using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::pipeline;
using namespace calenhad::legend;
using namespace calenhad::mapping;


int Module::seed = 0;

Module::Module (const QString& nodeType, const bool& suppressRender, QWidget* parent) : Node (nodeType, parent),
                                                            _globe (nullptr),
                                                            _shownParameter (QString::null),
                                                            _suppressRender (false),
                                                            _stats (nullptr)   {
    _legend = CalenhadServices::legends() -> defaultLegend();
    initialise();
}


Module::~Module () {
    if (_globe) { delete _globe; }
    if (_stats) { delete _stats; }
}

/// Initialise a QModule ready for use. Creates the UI.
void Module::initialise() {
    Node::initialise();
    // all modules have an output
    Port* output = new Port (Port::OutputPort, 0, "Output");
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
            _globe->initialise ();
            _globe->resize (640, 320);
            connect (_globe -> globe(), &CalenhadMapWidget::rendered, this, &Module::rendered);
        }
        _globe -> show();
    }
}

void Module::setupPreview() {
    if (!(_shownParameter.isNull () || _shownParameter.isEmpty ())) {
        _preview = new CalenhadMapView (this);
        _preview->setSource (this);
        _previewIndex = addPanel (tr ("Preview"), _preview);
        _stats = new QDialog (this);
        _stats -> setLayout (new QVBoxLayout (_stats));
        CalenhadStatsPanel* statsPanel = new CalenhadStatsPanel (this);
        _stats->layout ()->addWidget (statsPanel);
        QDialogButtonBox* box = new QDialogButtonBox (QDialogButtonBox::Ok);
        _stats -> layout ()->addWidget (box);
        connect (box, &QDialogButtonBox::accepted, _stats, &QDialog::accept);
        _stats -> setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint);
        _stats -> setMinimumSize (400, 400);
        _stats -> move (_dialog->pos().x() + 400, _dialog->pos().y() + 300);
        connect (_preview, &CalenhadMapWidget::rendered, statsPanel, &CalenhadStatsPanel::refresh);
        QAction* statsAction = new QAction (QIcon (":/appicons/controls/statistics.png"), "Statistics");
        connect (statsAction, &QAction::triggered, _stats, &QWidget::show);
        connect (_preview, &QWidget::customContextMenuRequested, this, &Module::showContextMenu);
        connect (_preview, &CalenhadMapWidget::rendered, this, &Module::rendered);
        _contextMenu -> addAction (statsAction);
    }
}

void Module::showContextMenu (const QPoint& point) {
    _contextMenu -> exec (QCursor::pos());
}

void Module::addInputPort (const unsigned& index, const int& portType, const QString& name) {
    Port* input = new Port (portType, index, name);
    addPort (input, index);
}

void Module::addInputPort (const unsigned& index, const int& portType, const QString& name, const double& defaultValue) {
    Port* input = new Port (portType, index, name, defaultValue);
    addPort (input, index);
}

void Module::addDependentNodes() {
    Node::addDependentNodes();
    for (const unsigned& index : inputs().keys()) {
        // if the port has a default value, create a constant value module to feed it
        Port* input = inputs().value (index);
        if (input -> hasDefaultValue()) {
            QString constName = _name + "_" + input -> portName();
            QPointF constPos = (QPointF (handle() -> pos().x() - 50 * (index + 2), handle() -> pos().y() + input -> pos().y() * index - 30));
            Module* constModule = _model -> addModule (constPos, "constant", constName);
            constModule -> setParameter ("value", input -> defaultValue());
            _model -> connectPorts (constModule -> output(), input);
        }
    }
}

void Module::showModuleDetail (const bool& visible) {
    Node::showModuleDetail (visible);
    if (visible && ! (_shownParameter.isNull() || _shownParameter.isEmpty())) {
        _expander -> setCurrentWidget (_content);
        _parameters.value (_shownParameter) -> setFocus();
    }
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

void Module::serialize (QDomDocument& doc) {
    Node::serialize (doc);
    _element.setAttribute ("legend", _legend -> name());
    QDomElement positionElement = doc.createElement ("position");
    _element.appendChild (positionElement);
    positionElement.setAttribute ("y", handle() -> scenePos().y());
    positionElement.setAttribute ("x", handle() -> scenePos().x());
    _element.setAttribute ("type", nodeType ());
}

void Module::rendered (const bool& success) {

}

void Module::setModel (CalenhadModel* model) {
    Node::setModel (model);
    if (! _suppressRender) {
        setupPreview ();
    }
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
    bool complete = Node::isComplete();
    _expander -> setItemEnabled (_previewIndex, complete);
    return complete;
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

calenhad::controls::globe::CalenhadMapView* Module::preview() {
    return _preview;
};

void Module::parameterChanged() {
    Node::parameterChanged();
    if (sender() == _parameters.find (_shownParameter).value()) {
        _handle -> setText (_parameters.find (_shownParameter).value() -> text());
    }
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

void Module::showParameter (QString paramName, bool editable) {
    _shownParameter = paramName;
    _editable = editable;
}
