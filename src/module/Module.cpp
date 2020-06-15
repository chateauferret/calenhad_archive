//
// Created by martin on 16/11/16.
//


#include "Module.h"
#include "nodeedit/NodeBlock.h"
#include "../module/NodeGroup.h"
#include "nodeedit/Port.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "src/CalenhadServices.h"
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
#include "../graph/graph.h"
#include "../graph/ComputeService.h"
#include "../mapping/projection/ProjectionService.h"
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

int Module::seed = 0;

Module::Module (const QString& nodeType, QWidget* parent) : Node (nodeType, parent),
                                                            _globe (nullptr),
                                                            _shownParameter (QString::null),
                                                            _suppressRender (true),
                                                            _connectMenu (new QMenu()),
                                                            _preview (nullptr),
                                                            _vertexBuffer (nullptr), _buffer (nullptr),
                                                            _stats (nullptr),
                                                            _statsPanel (nullptr),
                                                            _colorMapBuffer (nullptr) {
                                                            _legend = CalenhadServices::legends() -> defaultLegend();
                                                            initialise();
}


Module::~Module () {
    _suppressRender = true;
    close();
    delete _preview;
    delete _globe;
    delete _stats;
    delete _connectMenu;
    delete [] _buffer;
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
    _suppressRender = false;
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
    setupPreview();
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
                        if (p -> portName () == ew -> objectName () && p -> portType () != Port::OutputPort) {
                            if (! (p->hasConnection ())) {
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
        compute();
        // if this node needs recalculating or rerendering, so do any nodes that depend on it -
        // that is any nodes with an input connected to this one's output
        for (Module* dependant : dependants ()) {
            dependant -> invalidate ();
        }
        if (_preview) {
            _preview -> update();
        }
        if (_globe && _globe -> isVisible ()) {
            _globe -> update ();
        }
        if (_statsPanel) {
            _statsPanel -> refresh ();
        }
    }
}

QSet<Module*> Module::dependants() {

    QSet<Module*> found;
    if (! _output -> connections(). isEmpty ()) {
        foreach (Connection* c, _output -> connections()) {
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
    QString code = CalenhadServices::modules() -> glsl (_nodeType);

    // replace the input module markers with their names referencing their member variables in glsl
    int i = 0;
    for (Port* port : inputs ()) {
        QString index = QString::number (i++);
        if (port -> connections ().isEmpty ()) {
            code.replace ("%" + index, QString::number (parameterValue (port->portName ())));
        } else {
            Node* other = port -> connections() [0]->otherEnd (port) -> owner ();
            QString source = other -> name ();
            code.replace ("%" + index, "in_" + index + " [index]");    // "%0" is shorthand for "$0 (c)"
        }
    }

    // fill in attribute values by looking for words beginning with % and replacing them with the parameter values from the XML
    for (QString param : CalenhadServices::modules() -> paramNames ()) {
        if (parameters().contains (param)) {
            code.replace ("%" + param, QString::number (parameterValue (param)));
        }
    }

    return code;
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

bool Module::renderSuppressed() const {
    return _suppressRender;
}

void Module::suppressRender (bool suppress) {
    _suppressRender = suppress;
}

float* Module::vertexBuffer() {
    if (! _vertexBuffer) {
        _vertexBuffer = CalenhadServices::icosphere() -> vertexBuffer();
    }

    //_preview -> computeVertices (_vertexBuffer);
    return _vertexBuffer;
}

void Module::compute () {
    if (! _buffer) {
        _buffer = new float [2048 * 4096]; // for now
    }
    if (! _name.isNull()) {
        ComputeService* c = new ComputeService();
        c -> compute (this);
        delete c;
    }
}

float* Module::buffer () {

    return _buffer;
}

float* Module::colorMapBuffer() {
    uint size = CalenhadServices::preferences () -> calenhad_colormap_buffersize;
    if (! _colorMapBuffer) {
        _colorMapBuffer = new float [size * 4];
    }

    QString legendName = _legend -> name();
    QVector<LegendEntry> legendElements = _legend -> entries();

    float dx = (1 / (float) size) * 2 ;
    for (int i = 0; i < size * 4; i+= 4)  {
        QColor c = _legend -> lookup (i * dx - 1);
        _colorMapBuffer [i + 0] = (float) c.redF();
        _colorMapBuffer [i + 1] = (float) c.greenF();
        _colorMapBuffer [i + 2] = (float) c.blueF();
        _colorMapBuffer [i + 3] = (float) c.alphaF();
    }
    return _colorMapBuffer;
}

size_t Module::rasterHeight () {
    return 1024;
}

void Module::save() {
    QString fileName = QFileDialog::getSaveFileName (this, tr("Save heightmap image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    QImage* image = new QImage (rasterHeight() * 2, rasterHeight(), QImage::Format_ARGB32);
    for (int i = 0; i < rasterHeight() * 2; i++) {
        for (int j = 0; j < rasterHeight(); j++) {
            int index = j * rasterHeight() * 2 + i;
            double h = _buffer [index] / 2 + 0.5;
            int value = (int) floor (h * 256);
            value = std::max (std::min (value, 255), 0);
            QColor c (value, value, value, 255);
            image -> setPixelColor (i, j, c);
        }
    }
    image -> save (fileName);
}