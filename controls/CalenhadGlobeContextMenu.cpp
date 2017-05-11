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

    _mouseModeMenu = new QMenu ("Mouse behaviour", this);
    _mouseModeMenu -> setStatusTip ("Pan or zoom the globe view using the mouse");
    addMenu (_mouseModeMenu);

    _mouseModePanAction = new QAction ("Pan", this);
    _mouseModePanAction -> setToolTip ("Pan the globe using the mouse");
    _mouseModePanAction -> setCheckable (true);
    _mouseModePanAction -> setChecked (_parent -> mouseMode() == CalenhadGlobeMouseMode::Pan);

    _mouseModeZoomAction = new QAction ("Zoom", this);
    _mouseModeZoomAction -> setToolTip ("Zoom the globe using the mouse");
    _mouseModeZoomAction -> setCheckable (true);
    _mouseModeZoomAction -> setChecked (_parent -> mouseMode() == CalenhadGlobeMouseMode::Zoom);

    _mouseModeDisableAction = new QAction ("Disable", this);
    _mouseModeDisableAction -> setToolTip ("Disable mouse dragging on the globe");
    _mouseModeDisableAction -> setCheckable (true);
    _mouseModeDisableAction -> setChecked (_parent -> mouseMode() == CalenhadGlobeMouseMode::Disabled);

    QActionGroup* mouseModeGroup = new QActionGroup (this);
    mouseModeGroup -> addAction (_mouseModePanAction);
    mouseModeGroup -> addAction (_mouseModeZoomAction);
    mouseModeGroup -> addAction (_mouseModeDisableAction);

    connect (this, SIGNAL (aboutToShow()), this, SLOT (syncMouseMode ()));

    _mouseModeMenu -> addAction (_mouseModeDisableAction);
    _mouseModeMenu -> addAction (_mouseModePanAction);
    _mouseModeMenu -> addAction (_mouseModeZoomAction);
    connect (_mouseModePanAction, SIGNAL (toggled (bool)), this, SLOT (setMouseMode (const bool&)));
    connect (_mouseModeZoomAction, SIGNAL (toggled (bool)), this, SLOT (setMouseMode (const bool&)));
    connect (_mouseModeDisableAction, SIGNAL (toggled (bool)), this, SLOT (setMouseMode (const bool&)));

    addSeparator();

    QAction* configureAction = new QAction ("Properties...", this);
    configureAction -> setStatusTip ("Configure properties for the globe");
    configureAction -> setCheckable (false);
    addAction (configureAction);
    connect (configureAction, &QAction::triggered, parent, &CalenhadGlobe::showConfigDialog);


}

CalenhadGlobeContextMenu::~CalenhadGlobeContextMenu () {

}

void CalenhadGlobeContextMenu::setMouseMode (const bool& enable) {
    QAction* a = (QAction*) sender();
    if (a -> text() == "Pan") { emit mouseModeSelected (CalenhadGlobeMouseMode::Pan); }
    if (a -> text() == "Zoom") {emit mouseModeSelected (CalenhadGlobeMouseMode::Zoom); }
    if (a -> text() == "Disable") { emit mouseModeSelected (CalenhadGlobeMouseMode::Disabled); }
}

void CalenhadGlobeContextMenu::syncMouseMode () {
    blockSignals (true);
    _mouseModePanAction -> setChecked (_parent -> mouseMode() == CalenhadGlobeMouseMode::Pan);
    _mouseModeZoomAction -> setChecked (_parent -> mouseMode() == CalenhadGlobeMouseMode::Zoom);
    _mouseModeDisableAction -> setChecked (_parent -> mouseMode() == CalenhadGlobeMouseMode::Disabled);
    blockSignals (false);
}
