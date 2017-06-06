//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_CALENHADSTATSTAB_H
#define CALENHAD_CALENHADSTATSTAB_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <qwt/qwt_plot.h>
#include <QtWidgets/QComboBox>

class QModule;
class CalenhadGlobe;

class CalenhadStatsTab : public QWidget {
public:
    CalenhadStatsTab (CalenhadGlobe* parent);
    virtual ~CalenhadStatsTab();
    void showEvent (QShowEvent* e) override;

protected:
    CalenhadGlobe* _globe;
    QLabel* _extremesMapLabel, * _extremesGlobeLabel;
    QLabel* _renderTimeLabel, * _renderTimePerPixelLabel;
    QLabel* _pixelsInMapLabel;
    QLabel* _whenGeneratedLabel;
    QwtPlot* _hypsography;
};


#endif //CALENHAD_CALENHADSTATSTAB_H
