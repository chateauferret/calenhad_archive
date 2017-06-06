//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_STATISTICSSERVICE_H
#define CALENHAD_STATISTICSSERVICE_H


#include <QtCore/QDateTime>
#include "controls/CalenhadGlobe.h"


struct CalenhadStatistics {
public:
    int _pixelsInMap = 0;
    int _renderTime = 0;
    QString _whenRendered = "Not rendered";
    double _minValueGlobal = 0.0, _maxValueGlobal = 0.0;
    double _minValueMap = 0.0, _maxValueMap = 0.0;
};

class StatisticsService {

public:
    StatisticsService();
    virtual ~StatisticsService();
    CalenhadStatistics statistics();
    void recordMapExtremes (double minimum, double maximum);
    void recordGlobalExtremes (double minimum, double maximum);
    void recordRenderMetrics (QDateTime timestamp, int pixels, int time);

protected:
    CalenhadStatistics _statistics;

};


#endif //CALENHAD_STATISTICSSERVICE_H
