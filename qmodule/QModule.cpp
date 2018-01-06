//
// Created by martin on 16/11/16.
//


#include "QModule.h"
#include "../nodeedit/QNodeBlock.h"
#include "../nodeedit/qneport.h"
#include "../nodeedit/Calenhad.h"
#include "../pipeline/CalenhadModel.h"
#include "../CalenhadServices.h"
#include "../legend/LegendService.h"
#include "../legend/Legend.h"
#include "../pipeline/ModuleFactory.h"
#include <QDialogButtonBox>
#include <controls/globe/CalenhadGlobeDialog.h>
#include <controls/globe/CalenhadStatsPanel.h>
#include <QtWidgets/QCheckBox>
#include "../nodeedit/qneconnection.h"

using namespace icosphere;
using namespace calenhad::qmodule;
using namespace calenhad::nodeedit;
using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::pipeline;
using namespace calenhad::legend;
using namespace calenhad::mapping;


int QModule::seed = 0;

QModule::QModule (const QString& nodeType, int inputs, QWidget* parent) : QNode (nodeType, inputs, parent),
    _globe (nullptr),
    _stats (nullptr),
    _scaleBiasPanel (nullptr),
    _normalizeCheck (nullptr) {
    _legend = CalenhadServices::legends() -> defaultLegend();
    initialise();
}

QModule::~QModule () {
    if (_globe) { delete _globe; }
    if (_stats) { delete _stats; }
}

/// Initialise a QModule ready for use. Creates the UI.
void QModule::initialise() {
    QNode::initialise();
    // all modules have an output
    QNEPort* output = new QNEPort (QNEPort::OutputPort, 0, "Output");
    addPort (output);
    setContextMenuPolicy(Qt::CustomContextMenu);
    _contextMenu = new QMenu (this);
    QAction* globeAction = new QAction (QIcon (":/appicons/controls/globe.png"), "Show globe");
    connect (globeAction, &QAction::triggered, this, &QModule::showGlobe);
    _contextMenu -> addAction (globeAction);

}

void QModule::showGlobe() {
    if (!_globe) {
        _globe = new CalenhadGlobeDialog (this, this);
        _globe -> initialise ();
        _globe -> resize (640, 320);
        connect (_globe -> globe(), &CalenhadMapWidget::rendered, this, &QModule::rendered);
    }
    _globe -> show();
}

void QModule::setupPreview() {
    _preview = new CalenhadMapView (this);
    _preview -> setSource (this);
    _previewIndex = addPanel (tr ("Preview"), _preview);
    _stats = new QDialog (this);
    _stats -> setLayout (new QVBoxLayout (_stats));
    CalenhadStatsPanel* statsPanel = new CalenhadStatsPanel (this);
    _stats -> layout() -> addWidget (statsPanel);
    QDialogButtonBox* box = new QDialogButtonBox (QDialogButtonBox::Ok);
    _stats -> layout() -> addWidget (box);
    connect (box, &QDialogButtonBox::accepted, _stats, &QDialog::accept);
    _stats -> setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::WindowContextHelpButtonHint);
    _stats -> setMinimumSize (400, 400);
    _stats -> move (_dialog -> pos().x() + 400, _dialog -> pos().y() + 300);
    connect (_preview, &CalenhadMapWidget::rendered, statsPanel, &CalenhadStatsPanel::refresh);
    QAction* statsAction = new QAction (QIcon (":/appicons/controls/statistics.png"), "Statistics");
    connect (statsAction, &QAction::triggered, _stats, &QWidget::show);
    connect (_preview, &QWidget::customContextMenuRequested, this, &QModule::showContextMenu);
    connect (_preview, &CalenhadMapWidget::rendered, this, &QModule::rendered);
    _contextMenu -> addAction (statsAction);
}

void QModule::showContextMenu (const QPoint& point) {
    _contextMenu -> exec (QCursor::pos());
}


void QModule::addInputPorts() {
    // Rules: if there are < 3 inputs none of them are controls,
    // if there are 3 inputs, 0 and 1 are data and 2 is control,
    // if there are > 3 inputs, 1 is data and the rest are controls
    int portType;
    QString name;

    for (int i = 0; i < _inputCount; i++) {
        if (_inputCount < 3 || i == 0 || (i == 1 && _inputCount == 3)) {
            name = "Input " + QString::number (i + 1);
            portType = QNEPort::InputPort;
        } else {
            name = "Control " + QString::number (i - 1);
            portType = QNEPort::ControlPort;
        }
        QNEPort* input = new QNEPort (portType, i, name);
        addPort (input);
    }
}

QNode* QModule::sourceModule (int portIndex) {
    QNEPort* p = _ports.at (portIndex);
    if (p -> portType () == QNEPort::OutputPort || p -> connections().isEmpty()) {
        return nullptr;
    }
    QNEConnection* c = p -> connections().first();
    return c -> otherEnd (p) -> owner ();
}

void QModule::inflate (const QDomElement& element) {
    QNode::inflate (element);
    QString legendName = element.attribute ("legend", "default");
    _legend = CalenhadServices::legends() -> find (legendName);
    // position is retrieved in CalenhadModel
}

void QModule::serialize (QDomDocument& doc) {
    QNode::serialize (doc);
    _element.setAttribute ("legend", _legend -> name());
    QDomElement positionElement = doc.createElement ("position");
    _element.appendChild (positionElement);
    positionElement.setAttribute ("y", handle() -> scenePos().y());
    positionElement.setAttribute ("x", handle() -> scenePos().x());
    _element.setAttribute ("type", nodeType ());
}

void QModule::rendered (const bool& success) {
        if (_scaleBiasPanel) {
            _normalizeCheck -> setEnabled (success);
            double min, max;
            if (success && range (min, max)) {
                _rangeLabel -> setText ("Values from " + QString::number (min) + " to " + QString::number (max));
            } else {
                _rangeLabel -> setText ("Select Preview to render and calculate range");
            }
        }
}

void QModule::setModel (CalenhadModel* model) {
    QNode::setModel (model);
    setupPreview();
}

void QModule::setLegend (Legend* legend) {
    _legend = legend;
    emit nodeChanged();
}

Legend* QModule::legend () {
    return _legend;
}

bool QModule::generateMap () {
    return true;
}

bool QModule::isComplete() {
    bool complete = QNode::isComplete();
    _expander->setItemEnabled (_previewIndex, complete);
    return complete;
}

void QModule::contextMenuEvent (QContextMenuEvent* e) {
    _contextMenu -> exec();
}

bool QModule::range (double& min, double& max) {
    if (_preview) {
        Statistics statistics = _preview -> statistics();
        min = statistics._min;
        max = statistics._max;
        return true;
    } else {
        _rangeLabel -> setText ("");
        return false;
    }
}

calenhad::controls::globe::CalenhadMapView* QModule::preview () {
    return _preview;
};


void QModule::addScaleAndBias() {
    if (! _scaleBiasPanel) {
        _scaleBiasPanel = new QWidget (this);
        _scaleBiasPanel -> setLayout (new QFormLayout());
        addPanel ("Scale and bias", _scaleBiasPanel);
        addParameter ("Scale", "scale", 1.0, new AcceptAnyRubbish(), _scaleBiasPanel);
        addParameter ("Bias", "bias", 0.0, new AcceptAnyRubbish(), _scaleBiasPanel);
        _normalizeCheck = new QCheckBox (_scaleBiasPanel);
        _rangeLabel = new QLabel (_scaleBiasPanel);
        ((QFormLayout*) _scaleBiasPanel -> layout ()) -> addRow (_rangeLabel, _normalizeCheck);
        _scaleBiasPanel -> layout() -> addWidget (_normalizeCheck);
        _normalizeCheck -> setEnabled (false);
        _normalizeCheck -> setChecked (false);
        _normalizeCheck -> setFixedWidth (100);
        _normalizeCheck -> setText ("Normalize");
        connect (_normalizeCheck, &QCheckBox::stateChanged, this, &QModule::normalize);
    }
}

void QModule::normalize (const int& state) {
    if (state == Qt::Checked) {
        double min, max;
        bool ok = range (min, max);
        if (ok) {
            double scale = 2 / (max - min);
            double bias = -(scale * (max - min) / 2) + 1;
            scale *= parameterValue ("scale");
            bias -= parameterValue ("bias");
            _parameters.find ("scale").value() -> blockSignals (true);
            _parameters.find ("bias").value() -> blockSignals (true);
            setParameter ("scale", scale);
            setParameter ("bias", bias);
            _parameters.find ("scale").value() -> blockSignals (false);
            _parameters.find ("bias").value() -> blockSignals (false);
            _rangeLabel->setText ("Select Preview to render and calculate range");
            _normalizeCheck -> setEnabled (false);
            invalidate();
        }
    }
}

void QModule::parameterChanged() {
    if (_normalizeCheck) {
        if (_normalizeCheck -> isChecked()) {
            CalenhadServices::messages() -> message ("Noise parameter changed", "Noise may no longer be normalized");
            _normalizeCheck -> setChecked (false);
            _normalizeCheck -> setEnabled (true);
        }
        _rangeLabel -> setText ("Select Preview to render and calculate range");
    }

    QNode::parameterChanged();
}