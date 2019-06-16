//
// Created by martin on 10/05/17.
//

#include "CalenhadGlobeContextMenu.h"
#include "CalenhadGlobeWidget.h"
#include "CalenhadServices.h"
#include "../../mapping/projection/ProjectionService.h"
#include "../../mapping/projection/Projection.h"

using namespace calenhad::controls::globe;
using namespace calenhad::mapping;
using namespace calenhad::mapping::projection;

CalenhadGlobeContextMenu::CalenhadGlobeContextMenu (CalenhadGlobeWidget* parent) : QMenu(), _parent (parent) {

    _showOverviewMapAction = new QAction ("Overview map", this);
    _showOverviewMapAction->setStatusTip ("Toggle the display of the overview map");
    _showOverviewMapAction->setCheckable (true);
    addAction (_showOverviewMapAction);
    connect (_showOverviewMapAction, SIGNAL (toggled (bool)), this, SIGNAL (showOverviewMap (const bool&)));

    _showScaleAction = new QAction ("Scale", this);
    _showScaleAction->setStatusTip ("Toggle the display of the scale");
    _showScaleAction->setCheckable (true);
    _showScaleAction->setData ("scalebar");
    addAction (_showScaleAction);
    connect (_showScaleAction, SIGNAL (toggled (bool)), this, SLOT (setScalebarVisible (const bool&)));

    _showZoomSliderAction = new QAction ("Zoom bar", this);
    _showZoomSliderAction->setStatusTip ("Toggle the display of the scale bar");
    _showZoomSliderAction->setCheckable (true);
    addAction (_showZoomSliderAction);
    connect (_showZoomSliderAction, SIGNAL (toggled (bool)), this, SIGNAL (showZoomSlider (const bool&)));

    _showNavigatorAction = new QAction ("Navigator", this);
    _showNavigatorAction->setStatusTip ("Toggle the display of the compass / navigator dial");
    _showNavigatorAction->setCheckable (true);
    addAction (_showNavigatorAction);
    connect (_showNavigatorAction, SIGNAL (toggled (bool)), this, SIGNAL (showNavigator (const bool&)));

    _showGraticuleAction = new QAction ("Graticule", this);
    _showGraticuleAction->setStatusTip ("Toggle the display of the graticule");
    _showGraticuleAction->setCheckable (true);
    addAction (_showGraticuleAction);
    connect (_showGraticuleAction, SIGNAL (toggled (bool)), this, SIGNAL (showGraticule (const bool&)));

    // Configure actions for double-clicking with the mouse

    _mouseDoubleClickMenu = new QMenu ("Mouse double-click actions", this);
    _mouseDoubleClickMenu -> setStatusTip ("Go to or describe locations by double-clicking the mouse");
    addMenu (_mouseDoubleClickMenu);

    _gotoAction = new QAction ("Go to", this);
    _gotoAction -> setToolTip ("Go to a location by double-clicking the mouse");
    _gotoAction -> setCheckable (true);
    _gotoAction -> setChecked (_parent -> globe() -> mouseDoubleClickMode () == CalenhadGlobeDoubleClickMode::Goto);

    _placeAction = new QAction ("Describe place", this);
    _placeAction -> setToolTip ("Describe a place by double-clicking the mouse");
    _placeAction -> setCheckable (true);
    _placeAction -> setChecked (_parent -> globe() -> mouseDoubleClickMode () == CalenhadGlobeDoubleClickMode::Place);

    _disableDoubleClickAction = new QAction ("Do nothing", this);
    _disableDoubleClickAction -> setToolTip ("Disable mouse double-clicking on the globe");
    _disableDoubleClickAction -> setCheckable (true);
    _disableDoubleClickAction -> setChecked (_parent -> globe() -> mouseDoubleClickMode () == CalenhadGlobeDoubleClickMode::NoDoubleClick);

    QActionGroup* mouseDoubleClickModeGroup = new QActionGroup (this);
    mouseDoubleClickModeGroup -> addAction (_gotoAction);
    mouseDoubleClickModeGroup -> addAction (_placeAction);
    mouseDoubleClickModeGroup -> addAction (_disableDoubleClickAction);

    _mouseDoubleClickMenu -> addAction (_gotoAction);
    _mouseDoubleClickMenu -> addAction (_placeAction);
    _mouseDoubleClickMenu -> addAction (_disableDoubleClickAction);
    connect (_gotoAction, SIGNAL (toggled (bool)), this, SLOT (setDoubleClickMode (const bool&)));
    connect (_placeAction, SIGNAL (toggled (bool)), this, SLOT (setDoubleClickMode (const bool&)));
    connect (_disableDoubleClickAction, SIGNAL (toggled (bool)), this, SLOT (setDoubleClickMode (const bool&)));


    // Configure actions for dragging the mouse

    _mouseDragMenu = new QMenu ("Mouse drag actions", this);
    _mouseDragMenu -> setStatusTip ("Pan or scale the globe view using the mouse");
    addMenu (_mouseDragMenu);

    _panAction = new QAction ("Pan", this);
    _panAction -> setToolTip ("Pan the globe by dragging the mouse");
    _panAction -> setCheckable (true);
    _panAction -> setChecked (_parent -> globe() -> mouseDragMode () == CalenhadGlobeDragMode::Pan);

    _zoomAction = new QAction ("Zoom", this);
    _zoomAction -> setToolTip ("Zoom the globe by dragging the mouse");
    _zoomAction -> setCheckable (true);
    _zoomAction -> setChecked (_parent -> globe() -> mouseDragMode () == CalenhadGlobeDragMode::Zoom);

    _disableDragAction = new QAction ("Do nothing", this);
    _disableDragAction -> setToolTip ("Disable mouse dragging on the globe");
    _disableDragAction -> setCheckable (true);
    _disableDragAction -> setChecked (_parent -> globe() -> mouseDragMode () == CalenhadGlobeDragMode::NoDrag);

    QActionGroup* mouseDragModeGroup = new QActionGroup (this);
    mouseDragModeGroup -> addAction (_panAction);
    mouseDragModeGroup -> addAction (_zoomAction);
    mouseDragModeGroup -> addAction (_disableDragAction);

    _mouseDragMenu -> addAction (_disableDragAction);
    _mouseDragMenu -> addAction (_panAction);
    _mouseDragMenu -> addAction (_zoomAction);
    connect (_panAction, SIGNAL (toggled (bool)), this, SLOT (setDragMode (const bool&)));
    connect (_zoomAction, SIGNAL (toggled (bool)), this, SLOT (setDragMode (const bool&)));
    connect (_disableDragAction, SIGNAL (toggled (bool)), this, SLOT (setDragMode (const bool&)));

    // configure menu for selecting the projection

    _projectionMenu = new QMenu ("Projection", this);
    _projectionMenu -> setStatusTip ("Change the cartographic projection");
    addMenu (_projectionMenu);

    _projectionActions = new QActionGroup (this);
    QMap<QString, Projection*> m = CalenhadServices::projections() -> all ();
     for (QString key : m.keys()) {
         QAction* action = new QAction (key, this);
         action -> setToolTip ("Change projection to " + key);
         _projectionActions -> addAction (action);
         action -> setCheckable (true);
         _projectionMenu -> addAction (action);
         connect (action, SIGNAL (toggled (bool)), this, SLOT (projectionSelected (const bool&)));
     }

    _captureMenu = new QMenu ("Capture", this);
    QAction* _captureGreyscaleAction = new QAction ("Export heightmap", this);
    _captureGreyscaleAction -> setToolTip ("Generate a heightmap map and save");
    connect (_captureGreyscaleAction, &QAction::triggered, _parent, &CalenhadGlobeWidget::captureGreyscale);
    _captureMenu -> addAction (_captureGreyscaleAction);
    addMenu (_captureMenu);

    connect (_panAction, SIGNAL (toggled (bool)), this, SLOT (setDragMode (const bool&)));
    connect (_zoomAction, SIGNAL (toggled (bool)), this, SLOT (setDragMode (const bool&)));
    connect (_disableDragAction, SIGNAL (toggled (bool)), this, SLOT (setDragMode (const bool&)));


    // update the mouse mode actions on showing the menu to reflect the current state

    connect (this, SIGNAL (aboutToShow()), this, SLOT (initialise ()));

    addSeparator();

    QAction* configureAction = new QAction ("Properties...", this);
    configureAction -> setStatusTip ("Configure properties for the globe");
    configureAction -> setCheckable (false);
    addAction (configureAction);
    connect (configureAction, &QAction::triggered, parent, &CalenhadGlobeWidget::showConfigDialog);


}

CalenhadGlobeContextMenu::~CalenhadGlobeContextMenu () {

}

void CalenhadGlobeContextMenu::setDragMode (const bool& enable) {
    QAction* a = (QAction*) sender();
    if (a -> text() == "Pan") { emit dragModeSelected (CalenhadGlobeDragMode::Pan); }
    if (a -> text() == "Zoom") {emit dragModeSelected (CalenhadGlobeDragMode::Zoom); }
    if (a -> text() == "Do nothing") { emit dragModeSelected (CalenhadGlobeDragMode::NoDrag); }
}

void CalenhadGlobeContextMenu::setDoubleClickMode (const bool& enable) {
    QAction* a = (QAction*) sender();
    if (a -> text() == "Go to") { emit doubleClickModeSelected (CalenhadGlobeDoubleClickMode::Goto); }
    if (a -> text() == "Describe place") {emit doubleClickModeSelected (CalenhadGlobeDoubleClickMode::Place); }
    if (a -> text() == "Do nothing") { emit doubleClickModeSelected (CalenhadGlobeDoubleClickMode::NoDoubleClick); }
}

void CalenhadGlobeContextMenu::setScalebarVisible (const bool& visible) {
    emit scaleVisibleSelected (visible);
}

void CalenhadGlobeContextMenu::initialise () {
    blockSignals (true);
    _showOverviewMapAction->setChecked (_parent -> isOverviewVisible());
    _showScaleAction->setChecked (_parent -> isScaleVisible());
    _showZoomSliderAction->setChecked (_parent -> isZoomBarVisible());
    _showNavigatorAction->setChecked (_parent->isNavigatorVisible ());
    _showGraticuleAction -> setChecked (_parent -> globe() -> isGraticuleVisible());
    _panAction -> setChecked (_parent -> globe() -> mouseDragMode () == CalenhadGlobeDragMode::Pan);
    _zoomAction -> setChecked (_parent -> globe() -> mouseDragMode () == CalenhadGlobeDragMode::Zoom);
    _disableDragAction -> setChecked (_parent -> globe() -> mouseDragMode () == CalenhadGlobeDragMode::NoDrag);
    _gotoAction -> setChecked (_parent -> globe() ->  mouseDoubleClickMode () == CalenhadGlobeDoubleClickMode::Goto);
    _placeAction -> setChecked (_parent -> globe() -> mouseDoubleClickMode () == CalenhadGlobeDoubleClickMode::Place);
    _disableDoubleClickAction -> setChecked (_parent -> globe() -> mouseDoubleClickMode () == CalenhadGlobeDoubleClickMode::NoDoubleClick);

   /* QMap<QString, Projection> m = CalenhadServices::projections() -> all();
    for (QAction* action : _projectionActions -> actions()) {
        Projection p = _parent -> projection();
        QString key =  action -> text();
        action -> setChecked (p == m.value (key));
    }
    */
    blockSignals (false);
}

void CalenhadGlobeContextMenu::projectionSelected (const bool& selected) {
    QAction* action = (QAction*) sender();
    if (selected) {
        emit projectionChangeRequested (action -> text());
    }
}

