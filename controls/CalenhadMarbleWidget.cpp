//
// Created by martin on 16/01/17.
//

#include "CalenhadMarbleWidget.h"
#include <marble/MarbleDirs.h>
#include <QResizeEvent>
#include "CalenhadOverviewMap.h"
#include "../qmodule/QNode.h"

using namespace Marble;

CalenhadMarbleWidget::CalenhadMarbleWidget (QWidget* parent) : MarbleWidget (parent) {
    setMapThemeId (QStringLiteral ("earth/calenhad/calenhad.dgml"));

    std::cout <<  "Marble data path '" << Marble::MarbleDirs::marbleDataPath ().toStdString () << "'\n";
    std::cout << "Map theme id '" << mapThemeId ().toStdString () << "'\n";
}

CalenhadMarbleWidget::~CalenhadMarbleWidget () {

}

void CalenhadMarbleWidget::resizeEvent (QResizeEvent* e) {
    MarbleWidget::resizeEvent (e);
    emit resized (e -> size());
}

void CalenhadMarbleWidget::setOverview (CalenhadPreview* overview) {

 /*   _overview = overview;
    _overview -> initialise();
    _overview -> setFixedSize (210, 105);
    _overview -> render(); */

}
