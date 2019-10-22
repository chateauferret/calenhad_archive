//
// Created by martin on 16/01/17.
//

#include "CalenhadGlobeWidget.h"
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
#include "module/Module.h"
#include "../../mapping/Graticule.h"
#include "../../mapping/projection/ProjectionService.h"

class QwtCompass;

QMenu* makeGlobeContextMenu (const QPoint& pos);

using namespace GeographicLib;
using namespace icosphere;
using namespace calenhad;
using namespace calenhad::controls::legend;
using namespace calenhad::controls::globe;
using namespace calenhad::module;
using namespace calenhad::mapping;
using namespace calenhad::legend;
using namespace calenhad::graph;
using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace geoutils;



    CalenhadGlobeWidget::CalenhadGlobeWidget (QWidget* parent, Module* source) : QDialog (parent),
            _configDialog (nullptr),
            _contextMenu (nullptr),
            _moveFrom (QPoint (0, 0)),
            _globe (new CalenhadMapWidget (RenderMode::RenderModeGlobe, this)),
            _overview (new CalenhadMapWidget (RenderMode::RenderModeOverview, this)),
            _graph (nullptr),
            _geodesic (new Geodesic (1, 0)),
            _zoomSlider (nullptr),
            _navigator (nullptr) {
    // Turn on mouse tracking so that we can keep showing the mouse pointer coordinates.
    _globe -> setMouseTracking (true);
    connect (_globe, &QWidget::customContextMenuRequested, this, &CalenhadGlobeWidget::showContextMenu);
    _globe -> setSource (source);
    _globe -> setProjection ("Orthographic");

    _overview -> setSource (source);
    _overview -> setProjection ("Orthographic");
    _overview -> setMainMap (_globe);
}

void CalenhadGlobeWidget::initialise() {
    _zoomSlider = new QwtSlider (this);
    _zoomSlider -> setGroove (true);
    _zoomSlider -> setTrough (false);
    _zoomSlider -> setScalePosition (QwtSlider::NoScale);
    _zoomSlider -> setLowerBound (CalenhadServices::preferences() -> calenhad_globe_zoom_min);
    _zoomSlider -> setUpperBound (CalenhadServices::preferences() -> calenhad_globe_zoom_max);
    _zoomSlider -> move (width() - 20, height() - 20);
    _zoomSlider -> setFixedSize (40, 150);
    //_zoomSlider -> setScaleEngine (new QwtLogScaleEngine());
    connect (_globe, &AbstractMapWidget::zoomRequested, _zoomSlider, &QwtSlider::setValue);
    connect (_zoomSlider, SIGNAL (valueChanged (const double&)), this, SLOT (setZoom (const double&)));
    _zoomSlider -> setValue (1.0);

    // navigator - our navigator replaces both the navigation buttons and the compass rose
    _navigator = new CalenhadNavigator (this);
    _navigator -> move (width() - 20, 20);
    _zoomSlider -> setFixedSize (100, 100);
    connect (_navigator, &CalenhadNavigator::navigationRequested, _globe, &AbstractMapWidget::navigate);
    connect (this, SIGNAL (customContextMenuRequested (const QPoint&)), this, SLOT (showContextMenu (const QPoint&)));
    _positionLabel = new QLabel (this);

    // scale - distance measure based on scale and planet radius
    _scale = new GlobeScaleWidget (_globe, this);
    _scale -> resize (CalenhadServices::preferences() -> calenhad_globe_scale_width, CalenhadServices::preferences() -> calenhad_globe_scale_height);
    _scale -> move (20, 20);
    _scale -> setMargin (5);
    _scale -> setVisible (true);

    // graticule
    _showGraticuleAction = new QAction ("Graticule", this);
    _showGraticuleAction -> setIcon (QIcon (":/appicons/controls/graticule.png"));
    _showGraticuleAction -> setStatusTip ("Toggle the display of the graticule");
    _showGraticuleAction -> setCheckable (true);
    _showGraticuleAction -> setChecked (true);

    connect (_showGraticuleAction, &QAction::toggled, this, &CalenhadGlobeWidget::showGraticule);

    // toolbars and actions
    _viewToolbar = makeToolBar ("View");
    _mouseToolbar = makeToolBar ("Mouse");
    _mapWidgetsToolbar = makeToolBar ("Map");

    _propertiesAction = new QAction ("Properties");
    _propertiesAction -> setIcon (QIcon (":/appicons/controls/properties.png"));
    _propertiesAction -> setStatusTip ("Configure properties for the globe");
    connect (_propertiesAction, &QAction::triggered, this, [=] () { showConfigDialog(); });
    _viewToolbar -> addAction (_propertiesAction);
    _viewToolbar -> addAction (_showGraticuleAction);

    QActionGroup* mouseDragGroup = new QActionGroup (this);

    _mousePanAction = new QAction ("Pan");
    _mousePanAction -> setIcon (QIcon (":/appicons/controls/pan.png"));
    _mousePanAction -> setToolTip ("Pan the globe by dragging the mouse");
    mouseDragGroup -> addAction (_mousePanAction);
    _mousePanAction -> setCheckable (true);
    _mousePanAction -> setChecked (false);
    connect (_mousePanAction, &QAction::toggled, _globe, [=] (bool enabled) { _globe -> setMouseDragMode (enabled ? CalenhadGlobeDragMode::Pan : CalenhadGlobeDragMode::NoDrag); });
    _mouseToolbar -> addAction (_mousePanAction);
    _mouseZoomAction = new QAction ("Zoom");
    _mouseZoomAction -> setIcon (QIcon (":/appicons/controls/zoom.png"));
    _mouseZoomAction -> setToolTip ("Zoom the globe by dragging the mouse");
    _mouseZoomAction -> setCheckable (true);
    _mouseZoomAction -> setChecked (false);
    connect (_mouseZoomAction, &QAction::toggled, _globe, [=] (bool enabled) { _globe -> setMouseDragMode ( enabled ? CalenhadGlobeDragMode::Zoom : CalenhadGlobeDragMode::NoDrag); });
    mouseDragGroup -> addAction (_mouseZoomAction);
    _mouseToolbar -> addAction (_mouseZoomAction);


    _disableDragAction = new QAction ("Do nothing", this);
    _disableDragAction -> setToolTip ("Disable mouse dragging on the globe");
    _disableDragAction -> setCheckable (true);
    _disableDragAction -> setChecked (_globe -> mouseDragMode () == CalenhadGlobeDragMode::NoDrag);
    mouseDragGroup -> addAction (_disableDragAction);
    connect (_disableDragAction, &QAction::toggled, _globe, [=] () { _globe -> setMouseDragMode (CalenhadGlobeDragMode::NoDrag); });

    QActionGroup* mouseDoubleClickGroup = new QActionGroup (this);
    _mouseGotoAction = new QAction ("Goto");
    _mouseGotoAction -> setIcon (QIcon (":/appicons/controls/goto.png"));
    mouseDoubleClickGroup -> addAction (_mouseGotoAction);
    _mouseGotoAction -> setToolTip ("Go to a location by double-clicking the mouse");
    _mouseGotoAction -> setCheckable (true);
    _mouseGotoAction -> setChecked (false);
    connect (_mouseGotoAction, &QAction::toggled, _globe, [=] (bool enabled) { _globe -> setMouseDoubleClickMode (enabled ? CalenhadGlobeDoubleClickMode::Goto : CalenhadGlobeDoubleClickMode::NoDoubleClick); });

    _mouseToolbar -> addAction (_mouseGotoAction);
    _mousePlaceAction = new QAction ("Place");
    _mousePlaceAction -> setIcon (QIcon (":/appicons/controls/place.png"));
    mouseDoubleClickGroup -> addAction (_mousePlaceAction);
    _mousePlaceAction -> setToolTip ("Describe a place by double-clicking the mouse");
    _mousePlaceAction -> setCheckable (true);
    _mousePlaceAction -> setChecked (false);
    connect (_mousePlaceAction, &QAction::toggled, _globe, [=] (bool enabled) { _globe -> setMouseDoubleClickMode (enabled ? CalenhadGlobeDoubleClickMode::Place : CalenhadGlobeDoubleClickMode::NoDoubleClick); });
    _mouseToolbar -> addAction (_mousePlaceAction);

    _disableDoubleClickAction = new QAction ("Do nothing", this);
    _disableDoubleClickAction -> setToolTip ("Disable mouse double-clicking on the globe");
    _disableDoubleClickAction -> setCheckable (true);
    _disableDoubleClickAction -> setChecked (globe() -> mouseDoubleClickMode() == CalenhadGlobeDoubleClickMode::NoDoubleClick);
    mouseDoubleClickGroup -> addAction (_disableDoubleClickAction);
    connect (_disableDoubleClickAction, &QAction::toggled, _globe, [=] () { _globe -> setMouseDoubleClickMode (CalenhadGlobeDoubleClickMode::NoDoubleClick); });

    QActionGroup* mapWidgetsGroup = new QActionGroup (this);
    mapWidgetsGroup -> setExclusive (false);

    _showNavigatorAction = new QAction ("Navigator");
    _showNavigatorAction -> setIcon (QIcon (":/appicons/controls/navigator.png"));
    _showNavigatorAction -> setCheckable (true);
    _showNavigatorAction -> setChecked (true);
    connect (_showNavigatorAction, &QAction::toggled, this, &CalenhadGlobeWidget::showNavigator);
    mapWidgetsGroup -> addAction (_showNavigatorAction);

    _showScaleAction = new QAction ("Scale");
    _showScaleAction -> setIcon (QIcon (":/appicons/controls/scale.png"));
    _showScaleAction -> setStatusTip ("Toggle the display of the scale");
    _showScaleAction -> setCheckable (true);
    _showScaleAction -> setChecked (true);
    connect (_showScaleAction, &QAction::toggled, this, &CalenhadGlobeWidget::setScalebarVisible);
    mapWidgetsGroup -> addAction (_showScaleAction);

    _showOverviewAction = new QAction ("Overview");
    _showOverviewAction->setStatusTip ("Toggle the display of the overview map");
    _showOverviewAction -> setIcon (QIcon (":/appicons/controls/overview.png"));
    _showOverviewAction -> setCheckable (true);
    _showOverviewAction -> setChecked (true);
    connect (_showOverviewAction, &QAction::toggled, this, &CalenhadGlobeWidget::showOverviewMap);
    mapWidgetsGroup -> addAction (_showOverviewAction);

    _showZoomSliderAction = new QAction ("Zoom slider");
    _showZoomSliderAction->setStatusTip ("Toggle the display of the scale bar");
    _showZoomSliderAction -> setIcon (QIcon (":/appicons/controls/zoom.png"));
    _showZoomSliderAction -> setCheckable (true);
    _showZoomSliderAction -> setChecked (true);
    connect (_showZoomSliderAction, &QAction::toggled, this, &CalenhadGlobeWidget::showZoomSlider);
    mapWidgetsGroup -> addAction (_showZoomSliderAction);
    _mapWidgetsToolbar -> addActions (mapWidgetsGroup -> actions());


}

CalenhadToolBar* CalenhadGlobeWidget::makeToolBar (const QString& name) {
    CalenhadToolBar* toolbar = new CalenhadToolBar (name, this);
    //toolbar -> setLayout (new FlowLayout());
    toolbar -> setAcceptDrops (false);
    toolbar -> setParent (this);
    toolbar -> setAttribute (Qt::WA_StyledBackground);
    toolbar -> setStyleSheet("background-color:lightyellow;");
    return toolbar;
}


CalenhadGlobeWidget::~CalenhadGlobeWidget() {
    if (_configDialog) { delete _configDialog; }
    delete _geodesic;
    if (_graph) { delete _graph; }
}

void CalenhadGlobeWidget::invalidate () {
    _globe -> update();
     if (_overview) {
         _overview -> update();
     }
}

void CalenhadGlobeWidget::resizeEvent (QResizeEvent* e) {
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
        if (_overview) {
            _overview -> resize (CalenhadServices::preferences() -> calenhad_globe_inset_height * 2, CalenhadServices::preferences() -> calenhad_globe_inset_height );
            _overview -> move (20, std::max (20, height - 20 - _overview -> height()));
        }
        invalidate();
        emit resized (QSize (e -> size().width(), height));
}

void CalenhadGlobeWidget::showOverviewMap (const bool& show) {
    _overview -> setVisible (show);
}


void CalenhadGlobeWidget::showZoomSlider (const bool& show) {
    if (_zoomSlider) {
        _zoomSlider->setVisible (show);
    }
}

void CalenhadGlobeWidget::showNavigator (const bool& show) {
    if (_navigator) {
        _navigator->setVisible (show);
    }
}

void CalenhadGlobeWidget::setZoom (const double& zoom) {
    if (zoom <= CalenhadServices::preferences() -> calenhad_globe_zoom_max && zoom >= CalenhadServices::preferences() -> calenhad_globe_zoom_min) {
        _globe -> setScale (std::pow (10, - zoom));
        invalidate();
    }
}

double CalenhadGlobeWidget::zoom() {
    return _globe -> scale ();
}

bool CalenhadGlobeWidget::isZoomBarVisible () {
    return _zoomSlider -> isVisible();
}

bool CalenhadGlobeWidget::isNavigatorVisible () {
    return _navigator -> isVisible();
}

void CalenhadGlobeWidget::showConfigDialog() {
    if (!_configDialog) {
        _configDialog = new CalenhadGlobeConfigDialog (this);
        connect (_configDialog, &QDialog::accepted, this, &CalenhadGlobeWidget::updateConfig);
    }
    _configDialog -> initialise();
    _configDialog -> exec();
}

void CalenhadGlobeWidget::updateConfig () {
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
    if (_configDialog) {
        _configDialog->update ();
    }
    if (_contextMenu) {
        _contextMenu -> update();
    }
    for (Legend* legend : CalenhadServices::legends() -> all()) {
        legend -> widget () -> updateLegend();
    }
     update();
    _globe -> setCoordinatesFormat (_configDialog -> coordinatesFormat());
    _globe -> setDatumFormat (_configDialog -> datumFormat());
    _globe -> source() -> setLegend (_configDialog -> selectedLegend());
}

bool CalenhadGlobeWidget::isScaleVisible () {
    if (_scale) {
        return _scale -> isVisible ();
    } else return false;
}

void CalenhadGlobeWidget::setScalebarVisible (const bool& visible) {
    if (_scale) {
        _scale -> setVisible (visible);
    }
}

CalenhadMapWidget* CalenhadGlobeWidget::globe () {
    return _globe;
}

void CalenhadGlobeWidget::captureGreyscale() {
    QImage* image = _globe -> heightmap ();
    QString fileName = QFileDialog::getSaveFileName (this, tr("Save heightmap image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    image -> save (fileName);
}

bool CalenhadGlobeWidget::isOverviewVisible() {
    return _overview -> isVisible();
}


void CalenhadGlobeWidget::showGraticule (const bool& show) {
    _globe -> setGraticuleVisible (show);
}

void CalenhadGlobeWidget::showContextMenu (const QPoint& pos) {
    if (! _contextMenu) {
        _contextMenu = makeGlobeContextMenu (pos);
    }
    _contextMenu -> exec (mapToGlobal (pos));
}

QMenu* CalenhadGlobeWidget::makeGlobeContextMenu  (const QPoint& pos) {
    _contextMenu = new QMenu (this);
    _contextMenu -> addAction (_showOverviewAction);
    _contextMenu -> addAction (_showScaleAction);
    _contextMenu -> addAction (_showGraticuleAction);
    _showZoomSliderAction = new QAction ("Zoom bar", this);

    _showZoomSliderAction->setCheckable (true);
    addAction (_showZoomSliderAction);
    connect (_showZoomSliderAction, &QAction::toggled, this, &CalenhadGlobeWidget::showZoomSlider);

    _showNavigatorAction = new QAction ("Navigator", this);
    _showNavigatorAction->setStatusTip ("Toggle the display of the compass / navigator dial");
    _showNavigatorAction->setCheckable (true);
    addAction (_showNavigatorAction);
    connect (_showNavigatorAction, &QAction::toggled, this, &CalenhadGlobeWidget::showNavigator);

    // Configure actions for double-clicking with the mouse

    QMenu* mouseDoubleClickMenu = new QMenu ("Mouse double-click actions", this);
    mouseDoubleClickMenu -> setStatusTip ("Go to or describe locations by double-clicking the mouse");
    _contextMenu -> addMenu (mouseDoubleClickMenu);
    mouseDoubleClickMenu -> addAction (_mouseGotoAction);
    mouseDoubleClickMenu -> addAction (_mousePlaceAction);
    mouseDoubleClickMenu -> addAction (_disableDoubleClickAction);

    // Configure actions for dragging the mouse

    QMenu* mouseDragMenu = new QMenu ("Mouse drag actions", this);
    mouseDragMenu -> setStatusTip ("Pan or scale the globe view using the mouse");
    _contextMenu -> addMenu (mouseDragMenu);


    QActionGroup* mouseDragModeGroup = new QActionGroup (this);
    mouseDragModeGroup -> addAction (_mousePanAction);
    mouseDragModeGroup -> addAction (_mouseZoomAction);
    mouseDragModeGroup -> addAction (_disableDragAction);

    mouseDragMenu -> addAction (_disableDragAction);
    mouseDragMenu -> addAction (_mousePanAction);
    mouseDragMenu -> addAction (_mouseZoomAction);

    // configure menu for selecting the projection

    QMenu* projectionMenu = new QMenu ("Projection", this);
    projectionMenu -> setStatusTip ("Change the cartographic projection");


    QActionGroup* projectionActions = new QActionGroup (this);
    QMap<QString, Projection*> m = CalenhadServices::projections() -> all ();
    for (QString key : m.keys()) {
        QAction* action = new QAction (key, this);
        action -> setToolTip ("Change projection to " + key);
        action -> setData (key);
        projectionActions -> addAction (action);
        action -> setCheckable (true);
        projectionMenu -> addAction (action);
        connect (action, &QAction::toggled, this, &CalenhadGlobeWidget::projectionSelected);
    }
    _contextMenu -> addMenu (projectionMenu);
    QMenu* captureMenu = new QMenu ("Capture", this);
    QAction* captureGreyscaleAction = new QAction ("Export heightmap", this);
    captureGreyscaleAction -> setToolTip ("Generate a heightmap map and save");
    connect (captureGreyscaleAction, &QAction::triggered, this, &CalenhadGlobeWidget::captureGreyscale);
    captureMenu -> addAction (captureGreyscaleAction);
    _contextMenu -> addMenu (captureMenu);
    _contextMenu -> addSeparator();
    _contextMenu -> addAction (_propertiesAction);

    return _contextMenu;
}

void CalenhadGlobeWidget::projectionSelected (const bool& selected) {
    QAction* a = (QAction*) sender();
   _globe -> setProjection (a -> data().toString());
    invalidate();
}

CalenhadToolBar* CalenhadGlobeWidget::mouseToolBar() { return _mouseToolbar; }
CalenhadToolBar* CalenhadGlobeWidget::viewToolBar() { return _viewToolbar; }
CalenhadToolBar* CalenhadGlobeWidget::mapWidgetsToolBar() { return _mapWidgetsToolbar; }