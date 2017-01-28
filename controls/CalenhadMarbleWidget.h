//
// Created by martin on 16/01/17.
//

#ifndef CALENHAD_CALENHADMARBLEWIDGET_H
#define CALENHAD_CALENHADMARBLEWIDGET_H


#include <marble/MarbleWidget.h>

class CalenhadMarbleWidget : public Marble::MarbleWidget {
Q_OBJECT
public:
    CalenhadMarbleWidget (QWidget* parent);
    virtual ~CalenhadMarbleWidget();

};


#endif //CALENHAD_CALENHADMARBLEWIDGET_H
