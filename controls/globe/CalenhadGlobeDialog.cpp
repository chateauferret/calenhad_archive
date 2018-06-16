//
// Created by martin on 16/01/17.
//

#include "CalenhadGlobeDialog.h"
#include "CalenhadGlobeConfigDialog.h"
#include <QResizeEvent>
#include "../../legend/Legend.h"
#include "../../legend/LegendWidget.h"
#include <QMenu>
#include "CalenhadGlobeContextMenu.h"
#include <graph/graph.h>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtXml/QtXml>
#include "../../legend/LegendService.h"
#include "../../pipeline/CalenhadModel.h"
#include <QWindow>
#include <QtWidgets/QFileDialog>
#include "../../mapping/projection/Projection.h"
#include "qmodule/Module.h"
#include "../legend/LegendEditorScale.h"
#include "../../mapping/Graticule.h"

class QwtCompass;

using namespace GeographicLib;
using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls::legend;
using namespace calenhad::controls::globe;
using namespace calenhad::qmodule;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace calenhad::graph;
using namespace calenhad::qmodule;
using namespace geoutils;

CalenhadGlobeDialog::CalenhadGlobeDialog (QWidget* parent, Module* source) : QDialog (parent),
    _configDialog (nullptr),
    _contextMenu (nullptr),
    _moveFrom (QPoint (0, 0)),
    _globe (new CalenhadMapWidget (this)),
    _graph (nullptr),
    _geodesic (new Geodesic (1, 0)),
    _zoomSlider (nullptr),
    _navigator (nullptr) {

    // Turn on mouse tracking so that we can keep showing the mouse pointer coordinates.
    _globe -> setMouseTracking (true);
    connect (_globe, &QWidget::customContextMenuRequested, this, &CalenhadGlobeDialog::showContextMenu);
    _globe -> setSource (source);
    _globe -> setProjection ("Orthographic");
    _globe -> setInset (true);
}

void CalenhadGlobeDialog::initialise() {
    _zoomSlider = new QwtSlider (this);
    _zoomSlider -> setGroove (true);
    _zoomSlider -> setTrough (false);
    _zoomSlider -> setScalePosition (QwtSlider::NoScale);
    _zoomSlider -> setLowerBound (CalenhadServices::preferences() -> calenhad_globe_zoom_min);
    _zoomSlider -> setUpperBound (CalenhadServices::preferences() -> calenhad_globe_zoom_max);
    _zoomSlider -> move (width() - 20, height() - 20);
    _zoomSlider -> setFixedSize (40, 150);
    //_zoomSlider -> setScaleEngine (new QwtLogScaleEngine());
    connect (_globe, &CalenhadMapWidget::zoomRequested, _zoomSlider, &QwtSlider::setValue);
    connect (_zoomSlider, SIGNAL (valueChanged (const double&)), this, SLOT (setZoom (const double&)));
    _zoomSlider -> setValue (1.0);

    // navigator - our navigator replaces both the navigation buttons and the compass rose
    _navigator = new CalenhadNavigator (this);
    _navigator -> move (width() - 20, 20);
    _zoomSlider -> setFixedSize (100, 100);
    connect (_navigator, &CalenhadNavigator::navigationRequested, _globe, &CalenhadMapWidget::navigate);
    connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
    _positionLabel = new QLabel (this);

    // scale - distance measure based on scale and planet radius
    _scale = new GlobeScaleWidget (_globe, this);
    _scale -> resize (CalenhadServices::preferences() -> calenhad_globe_scale_width, CalenhadServices::preferences() -> calenhad_globe_scale_height);
    _scale -> move (20, height() - 20);
    _scale -> setMargin (5);
    _scale -> setVisible (true);
}

CalenhadGlobeDialog::~CalenhadGlobeDialog() {
    if (_configDialog) { delete _configDialog; }
    delete _geodesic;
    if (_graph) { delete _graph; }
}

void CalenhadGlobeDialog::invalidate () {
    _globe -> update();
}

void CalenhadGlobeDialog::resizeEvent (QResizeEvent* e) {
    int height = e -> size().height();
    int width = e -> size().width();

        // let the map fill the whole widget but keep the widget's aspect ratio at 2 : 1.
        if (width > 2 * height) {
            _globe -> setFixedSize (width, width / 2);
        } else {
            _globe -> setFixedSize (height * 2, height);
        }
        // update positions and sizes of the control widgets
        if (_zoomSlider) {
            _zoomSlider->move (std::max (20, width - 20 - _zoomSlider->width ()), std::max (20, height - 20 - _zoomSlider->height ()));
            _zoomSlider->setFixedSize (40, std::max (150, height - 200));
        }
        if (_navigator) {
            _navigator->setFixedSize (100, 100);
            _navigator->move (std::max (20, width - 20 - _navigator->height ()), 20);
        }
        invalidate();
        emit resized (QSize (e -> size().width(), height));
}

void CalenhadGlobeDialog::showContextMenu (const QPoint& pos) {
    if (! _contextMenu) {
        _contextMenu = new CalenhadGlobeContextMenu (this);
        connect (_contextMenu, &CalenhadGlobeContextMenu::showOverviewMap, this, &CalenhadGlobeDialog::showOverviewMap);
        connect (_contextMenu, &CalenhadGlobeContextMenu::showZoomSlider, this, &CalenhadGlobeDialog::showZoomSlider);
        connect (_contextMenu, &CalenhadGlobeContextMenu::scaleVisibleSelected, this, &CalenhadGlobeDialog::setScalebarVisible);
        connect (_contextMenu, &CalenhadGlobeContextMenu::showNavigator, this, &CalenhadGlobeDialog::showNavigator);
        connect (_contextMenu, &CalenhadGlobeContextMenu::showGraticule, _globe, &CalenhadMapWidget::setGraticuleVisible);
        connect (_contextMenu, &CalenhadGlobeContextMenu::dragModeSelected, _globe, &CalenhadMapWidget::setMouseDragMode);
        connect (_contextMenu, &CalenhadGlobeContextMenu::doubleClickModeSelected, _globe, &CalenhadMapWidget::setMouseDoubleClickMode);
        connect (_contextMenu, &CalenhadGlobeContextMenu::projectionChangeRequested,_globe, &CalenhadMapWidget::setProjection);
        connect (this, &CalenhadGlobeDialog::customContextMenuRequested, this,  &CalenhadGlobeDialog::showContextMenu);
    }
    _contextMenu -> exec (mapToGlobal (pos));
}

void CalenhadGlobeDialog::showOverviewMap (const bool& show) {
    _globe -> setInset (show);
}


void CalenhadGlobeDialog::showZoomSlider (const bool& show) {
    if (_zoomSlider) {
        _zoomSlider->setVisible (show);
    }
}

void CalenhadGlobeDialog::showNavigator (const bool& show) {
    if (_navigator) {
        _navigator->setVisible (show);
    }
}

void CalenhadGlobeDialog::setZoom (const double& zoom) {
    if (zoom <= CalenhadServices::preferences() -> calenhad_globe_zoom_max && zoom >= CalenhadServices::preferences() -> calenhad_globe_zoom_min) {
        _globe -> setScale (std::pow (10, - zoom));
        invalidate();
    }
}

double CalenhadGlobeDialog::zoom() {
    return _globe -> scale ();
}

bool CalenhadGlobeDialog::isZoomBarVisible () {
    return _zoomSlider -> isVisible();
}

bool CalenhadGlobeDialog::isNavigatorVisible () {
    return _navigator -> isVisible();
}

void CalenhadGlobeDialog::showConfigDialog() {
    if (!_configDialog) {
        _configDialog = new CalenhadGlobeConfigDialog (this);
        connect (_configDialog, &QDialog::accepted, this, &CalenhadGlobeDialog::updateConfig);
    }
    _configDialog -> initialise();
    _configDialog -> exec();
}

void CalenhadGlobeDialog::updateConfig () {
    setScalebarVisible (_configDialog -> scaleCheckState());
    showOverviewMap (_configDialog -> overviewCheckState ());
    showZoomSlider (_configDialog -> zoomBarCheckState ());
    showNavigator (_configDialog -> compassCheckState());
    Graticule* g = globe() -> graticule();
    if (g) {
        g -> setDensity (_configDialog -> graticuleDensity());
        g -> setPens (_configDialog -> graticuleMajorPen(), _configDialog -> graticuleMinorPen());
    }
    _globe -> setGraticuleVisible (_configDialog -> graticuleCheckState ());
    _globe -> setMouseDragMode (_configDialog -> dragMode ());
    _globe -> setMouseDoubleClickMode (_configDialog -> doubleClickMode());
    _globe -> setSensitivity (_configDialog -> mouseSensitivity());
    _globe -> setProjection (_configDialog -> selectedProjection() -> name ());
    _globe -> setRenderQuality (_configDialog -> selectedRenderQuality());
    _configDialog -> update();
    _contextMenu -> update();
    for (Legend* legend : CalenhadServices::legends() -> all()) {
        legend -> widget () -> updateLegend();
    }
     update();
    _globe -> setCoordinatesFormat (_configDialog -> coordinatesFormat());
    _globe -> setDatumFormat (_configDialog -> datumFormat());
    _globe -> source() -> setLegend (_configDialog -> selectedLegend());
}

bool CalenhadGlobeDialog::isScaleVisible () {
    if (_scale) {
        return ((QWidget*) _scale->parent ())->isVisible ();
    } else return false;
}

void CalenhadGlobeDialog::setScalebarVisible (const bool& visible) {
    if (_scale) {
        ((QWidget*) _scale -> parent()) -> setVisible (visible);
    }
}

CalenhadMapWidget* CalenhadGlobeDialog::globe () {
    return _globe;
}

bool CalenhadGlobeDialog::isOverviewVisible () {
    return _globe -> inset();
}

void CalenhadGlobeDialog::captureGreyscale() {
    QImage* image = _globe -> heightmap ();
    QString fileName = QFileDialog::getSaveFileName (this, tr("Save heightmap image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    image -> save (fileName);
}
