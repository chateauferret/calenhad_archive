//
// Created by martin on 10/05/17.
//

#include "CalenhadGlobeContextMenu.h"
#include "CalenhadGlobe.h"

CalenhadGlobeContextMenu::CalenhadGlobeContextMenu (CalenhadGlobe* parent) : QMenu(), _parent (parent) {

    QAction* showOverviewMapAction = new QAction ("Overview map", this);
    showOverviewMapAction->setStatusTip ("Toggle the display of the overview map");
    showOverviewMapAction->setCheckable (true);
    showOverviewMapAction->setChecked (parent -> isOverviewVisible());
    addAction (showOverviewMapAction);
    connect (showOverviewMapAction, SIGNAL (toggled (bool)), this, SIGNAL (showOverviewMap (const bool&)));

    QAction* showScaleAction = new QAction ("Scale", this);
    showScaleAction->setStatusTip ("Toggle the display of the scale");
    showScaleAction->setCheckable (true);
    showScaleAction->setChecked (parent -> isFloatItemVisible ("scalebar"));
    showScaleAction->setData ("scalebar");
    addAction (showScaleAction);
    connect (showScaleAction, SIGNAL (toggled (bool)), this, SIGNAL (setFloatItemVisible (const bool&)));

    QAction* showZoomSliderAction = new QAction ("Zoom bar", this);
    showZoomSliderAction->setStatusTip ("Toggle the display of the zoom bar");
    showZoomSliderAction->setCheckable (true);
    showZoomSliderAction->setChecked (parent -> isZoomBarVisible());
    addAction (showZoomSliderAction);
    connect (showZoomSliderAction, SIGNAL (toggled (bool)), this, SIGNAL (showZoomSlider (const bool&)));

    QAction* showNavigatorAction = new QAction ("Compass", this);
    showNavigatorAction->setStatusTip ("Toggle the display of the compass");
    showNavigatorAction->setCheckable (true);
    showNavigatorAction->setChecked (parent -> isCompassVisible());
    addAction (showNavigatorAction);
    connect (showNavigatorAction, SIGNAL (toggled (bool)), this, SIGNAL (showNavigator (const bool&)));

    // Configure actions for double-clicking with the mouse

    _mouseDoubleClickMenu = new QMenu ("Mouse double-click actions", this);
    _mouseDoubleClickMenu -> setStatusTip ("Go to or describe locations by double-clicking the mouse");
    addMenu (_mouseDoubleClickMenu);

    _gotoAction = new QAction ("Go to", this);
    _gotoAction -> setToolTip ("Go to a location by double-clicking the mouse");
    _gotoAction -> setCheckable (true);
    _gotoAction -> setChecked (_parent->doubleClickMode () == CalenhadGlobeDoubleClickMode::Goto);

    _placeAction = new QAction ("Describe place", this);
    _placeAction -> setToolTip ("Describe a place by double-clicking the mouse");
    _placeAction -> setCheckable (true);
    _placeAction -> setChecked (_parent->doubleClickMode () == CalenhadGlobeDoubleClickMode::Place);

    _disableDoubleClickAction = new QAction ("Do nothing", this);
    _disableDoubleClickAction -> setToolTip ("Disable mouse double-clicking on the globe");
    _disableDoubleClickAction -> setCheckable (true);
    _disableDoubleClickAction -> setChecked (_parent->doubleClickMode () == CalenhadGlobeDoubleClickMode::NoDoubleClick);

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
    _mouseDragMenu -> setStatusTip ("Pan or zoom the globe view using the mouse");
    addMenu (_mouseDragMenu);

    _panAction = new QAction ("Pan", this);
    _panAction -> setToolTip ("Pan the globe by dragging the mouse");
    _panAction -> setCheckable (true);
    _panAction -> setChecked (_parent->dragMode () == CalenhadGlobeDragMode::Pan);

    _zoomAction = new QAction ("Zoom", this);
    _zoomAction -> setToolTip ("Zoom the globe by dragging the mouse");
    _zoomAction -> setCheckable (true);
    _zoomAction -> setChecked (_parent->dragMode () == CalenhadGlobeDragMode::Zoom);

    _disableDragAction = new QAction ("Do nothing", this);
    _disableDragAction -> setToolTip ("Disable mouse dragging on the globe");
    _disableDragAction -> setCheckable (true);
    _disableDragAction -> setChecked (_parent->dragMode () == CalenhadGlobeDragMode::NoDrag);

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

    // update the mouse mode actions on showing the menu to reflect the current state

    connect (this, SIGNAL (aboutToShow()), this, SLOT (syncMouseMode ()));

    addSeparator();

    QAction* configureAction = new QAction ("Properties...", this);
    configureAction -> setStatusTip ("Configure properties for the globe");
    configureAction -> setCheckable (false);
    addAction (configureAction);
    connect (configureAction, &QAction::triggered, parent, &CalenhadGlobe::showConfigDialog);


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

void CalenhadGlobeContextMenu::syncMouseMode () {
    blockSignals (true);
    _panAction -> setChecked (_parent->dragMode () == CalenhadGlobeDragMode::Pan);
    _zoomAction -> setChecked (_parent->dragMode () == CalenhadGlobeDragMode::Zoom);
    _disableDragAction -> setChecked (_parent->dragMode () == CalenhadGlobeDragMode::NoDrag);
    _gotoAction -> setChecked (_parent->doubleClickMode () == CalenhadGlobeDoubleClickMode::Goto);
    _placeAction -> setChecked (_parent->doubleClickMode () == CalenhadGlobeDoubleClickMode::Place);
    _disableDoubleClickAction -> setChecked (_parent->doubleClickMode () == CalenhadGlobeDoubleClickMode::NoDoubleClick);
    blockSignals (false);
}
