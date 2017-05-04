//
// Created by martin on 16/01/17.
//

#ifndef CALENHAD_CALENHADMARBLEWIDGET_H
#define CALENHAD_CALENHADMARBLEWIDGET_H


#include <marble/MarbleWidget.h>
#include <QtWidgets/QPushButton>
#include "QNoiseMapViewer.h"

class CalenhadOverviewMap;

class CalenhadMarbleWidget : public Marble::MarbleWidget {
Q_OBJECT
public:
    CalenhadMarbleWidget (QWidget* parent);
    virtual ~CalenhadMarbleWidget();
    void resizeEvent (QResizeEvent* e) override;
    void setOverview (CalenhadPreview* overview);

    signals:
    void resized (const QSize& size);

protected:
    CalenhadPreview* _overview;

};


#endif //CALENHAD_CALENHADMARBLEWIDGET_H
