//
// Created by martin on 05/06/17.
//

#include "StatisticsService.h"

StatisticsService::StatisticsService () {

}

CalenhadStatistics StatisticsService::statistics () {
    if (_statistics._minValueMap < _statistics._minValueGlobal) { _statistics._minValueGlobal = _statistics._minValueMap; }
    if (_statistics._maxValueMap > _statistics._maxValueGlobal) { _statistics._maxValueGlobal = _statistics._maxValueMap; }
    return _statistics;
}

StatisticsService::~StatisticsService () {

}

void StatisticsService::recordMapExtremes (double minimum, double maximum) {
    _statistics._minValueMap = minimum;
    _statistics._maxValueMap = maximum;
}

void StatisticsService::recordGlobalExtremes (double minimum, double maximum) {
    _statistics._minValueGlobal = minimum;
    _statistics._maxValueGlobal = maximum;
}

void StatisticsService::recordRenderMetrics (QDateTime timestamp, int pixels, int time) {
    _statistics._whenRendered = timestamp.toString();
    _statistics._pixelsInMap = pixels;
    _statistics._renderTime = time;
}

