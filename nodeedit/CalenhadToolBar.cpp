//
// Created by martin on 29/09/17.
//

#include <QtWidgets/QDockWidget>
#include "CalenhadToolBar.h"

void calenhad::nodeedit::CalenhadToolBar::arrange (Qt::DockWidgetArea area) {
    QDockWidget* dock = dynamic_cast<QDockWidget*> (sender());
    if (dock) {
        if (area == Qt::BottomDockWidgetArea || area == Qt::TopDockWidgetArea) {
            setOrientation (Qt::Horizontal);
        }
        if (area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea) {
            setOrientation (Qt::Vertical);
        }
    }
}

calenhad::nodeedit::CalenhadToolBar::CalenhadToolBar (const QString& name, QWidget* parent) : QToolBar (name, parent) {

}

calenhad::nodeedit::CalenhadToolBar::~CalenhadToolBar () {

}
