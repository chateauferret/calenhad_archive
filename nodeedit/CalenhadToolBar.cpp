//
// Created by martin on 29/09/17.
//

#include <QtWidgets/QDockWidget>
#include "CalenhadToolBar.h"

using namespace calenhad::nodeedit;


calenhad::nodeedit::CalenhadToolBar::CalenhadToolBar (const QString& name, QWidget* parent) : QToolBar (name, parent) {
    setSizePolicy (QSizePolicy::Ignored, QSizePolicy::Maximum);
}

calenhad::nodeedit::CalenhadToolBar::~CalenhadToolBar () {

}


void CalenhadToolBar::arrange (Qt::DockWidgetArea area) {
        if (area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea) {
            setOrientation (Qt::Vertical);
        } else {
            setOrientation (Qt::Horizontal);
        }
}

void CalenhadToolBar::detached (const bool& floating) {
    if (floating) {
        setOrientation (Qt::Horizontal);
        adjustSize();
        QDockWidget* dock = dynamic_cast<QDockWidget*> (sender());
        if (dock) {
            dock -> adjustSize();
        }
    }
}
