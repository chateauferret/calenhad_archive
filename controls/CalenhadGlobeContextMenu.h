//
// Created by martin on 10/05/17.
//

#ifndef CALENHAD_CALENHADGLOBECONTEXTMENU_H
#define CALENHAD_CALENHADGLOBECONTEXTMENU_H


#include <QtWidgets/QMenu>
#include "CalenhadGlobe.h"

class CalenhadGlobe;

class CalenhadGlobeContextMenu : public QMenu {
Q_OBJECT
public:
    CalenhadGlobeContextMenu (CalenhadGlobe* parent);
    virtual ~CalenhadGlobeContextMenu();

public slots:
    void setDragMode (const bool& enable);

signals:
    void showOverviewMap (const bool&);
    void showZoomSlider (const bool&);
    void setFloatItemVisible (const bool&);
    void showNavigator (const bool&);
    void dragModeSelected (const CalenhadGlobeDragMode&);
    void doubleClickModeSelected (const CalenhadGlobeDoubleClickMode&);

protected:
    CalenhadGlobe* _parent;
    QMenu* _mouseDragMenu;
    QAction* _panAction;
    QAction* _zoomAction;
    QAction* _disableDragAction;
    QMenu* _mouseDoubleClickMenu;
    QAction* _gotoAction;
    QAction* _placeAction;
    QAction* _disableDoubleClickAction;


protected slots:
    void syncMouseMode ();


    void setDoubleClickMode (const bool& enable);
};


#endif //CALENHAD_CALENHADGLOBECONTEXTMENU_H
