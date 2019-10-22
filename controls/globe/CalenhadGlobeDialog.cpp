//
// Created by martin on 09/12/18.
//

#include "../../nodeedit/CalenhadToolBar.h"
#include "CalenhadGlobeDialog.h"
#include <QActionGroup>
#include <QtWidgets/QDockWidget>
#include "../../nodeedit/CalenhadToolBar.h"
#include <QDockWidget>
#include "../FlowLayout.h"


using namespace calenhad::controls;
using namespace calenhad::controls::globe;
using namespace calenhad::module;
using namespace calenhad::nodeedit;

CalenhadGlobeDialog::CalenhadGlobeDialog (QWidget* parent, Module* module) : QMainWindow (parent), _widget (nullptr), _module (module),
    _mainDock (nullptr), _mouseDock (nullptr), _widgetDock (nullptr) {
    connect (parent, &QWidget::destroyed, this, &QWidget::close);
}

CalenhadGlobeDialog::~CalenhadGlobeDialog() {
    delete _mainDock;
    delete _mouseDock;
    delete _widgetDock;
}

CalenhadGlobeWidget* CalenhadGlobeDialog::widget() {
    return _widget;
}

void CalenhadGlobeDialog::initialise() {
    _widget = new CalenhadGlobeWidget (nullptr, _module);
    _widget -> initialise();
    setDockNestingEnabled (true);

    _mainDock = new QDockWidget ("View");
    _mainDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    CalenhadToolBar* viewToolbar = _widget -> viewToolBar();
    _mainDock -> setWidget (viewToolbar);
    _mainDock -> layout() -> setAlignment (Qt::AlignLeft);

    connect (_mainDock, &QDockWidget::dockLocationChanged, viewToolbar, &CalenhadToolBar::arrange);
    connect (_mainDock, &QDockWidget::topLevelChanged, viewToolbar, &CalenhadToolBar::detached);

    _mouseDock = new QDockWidget ("Mouse");
    _mouseDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    CalenhadToolBar* mouseToolbar = _widget -> mouseToolBar();
    _mouseDock -> setWidget (mouseToolbar);
    _mouseDock -> layout() -> setAlignment (Qt::AlignLeft);
    connect (_mouseDock, &QDockWidget::dockLocationChanged, mouseToolbar, &CalenhadToolBar::arrange);
    connect (_mouseDock, &QDockWidget::topLevelChanged, mouseToolbar, &CalenhadToolBar::detached);

    _widgetDock = new QDockWidget ("Map");
    _widgetDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    CalenhadToolBar* mapWidgetsToolbar = _widget -> mapWidgetsToolBar();
    _widgetDock -> setWidget (mapWidgetsToolbar);
    _widgetDock -> layout() -> setAlignment (Qt::AlignLeft);
    connect (_widgetDock, &QDockWidget::dockLocationChanged, mapWidgetsToolbar, &CalenhadToolBar::arrange);
    connect (_widgetDock, &QDockWidget::topLevelChanged, mapWidgetsToolbar, &CalenhadToolBar::detached);

    addDockWidget (Qt::TopDockWidgetArea, _mainDock);
    addDockWidget (Qt::TopDockWidgetArea, _mouseDock);
    addDockWidget (Qt::TopDockWidgetArea, _widgetDock);
    //_widget -> setSizePolicy ()
    setCentralWidget (_widget);

}


void CalenhadGlobeDialog::closeEvent (QCloseEvent* e) {
    if (_mainDock) { _mainDock -> hide(); }
    if (_mouseDock) { _mouseDock -> hide(); }
    if (_widgetDock) { _widgetDock -> hide(); }
}


void CalenhadGlobeDialog::showEvent (QShowEvent* e) {
    if (_mainDock) { _mainDock -> show(); }
    if (_mouseDock) { _mouseDock -> show(); }
    if (_widgetDock) { _widgetDock -> show(); }
}
