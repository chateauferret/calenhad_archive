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
    void setMouseMode (const bool& enable);

signals:
    void showOverviewMap (const bool&);
    void showZoomSlider (const bool&);
    void setFloatItemVisible (const bool&);
    void showNavigator (const bool&);
    void mouseModeSelected (const CalenhadGlobeMouseMode&);

protected:
    CalenhadGlobe* _parent;
    QMenu* _mouseModeMenu;
    QAction* _mouseModePanAction;
    QAction* _mouseModeZoomAction;
    QAction* _mouseModeDisableAction;

protected slots:
    void syncMouseMode ();


};


#endif //CALENHAD_CALENHADGLOBECONTEXTMENU_H
