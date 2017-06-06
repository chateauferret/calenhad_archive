//
// Created by martin on 02/06/17.
//

#ifndef CALENHAD_LEGENDMANAGER_H
#define CALENHAD_LEGENDMANAGER_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include "../mapping/Legend.h"

class Legend;
class LegendChooser;
class LegendWidget;
class LegendService;

class LegendManager : public QWidget {

public:
    LegendManager (QWidget* parent);
    virtual ~LegendManager();
    Legend* currentLegend();
    void showEvent (QShowEvent* e);


public slots:
    void commit();
    void rollback();
    void showLegend ();
    void setCurrentLegend (Legend* legend);

protected:
    Legend* _legend;
    LegendService* _service;
    LegendChooser* _chooser;
    QWidget* _legendDetailArea;
    bool _legendsChanged;

    QPushButton* _deleteLegendButton;
    QPushButton* _newLegendButton;
    QWidget* _legendButtonPanel;

    void newLegend ();
    void deleteLegend ();


    void addLegendWidget (Legend* legend);
};


#endif //CALENHAD_LEGENDMANAGER_H
