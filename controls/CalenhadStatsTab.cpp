//
// Created by martin on 05/06/17.
//

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include "CalenhadStatsTab.h"
#include "CalenhadGlobe.h"
#include "../CalenhadServices.h"

CalenhadStatsTab::CalenhadStatsTab (CalenhadGlobe* parent) : QWidget (parent),
    _globe (parent) {

    setLayout (new QFormLayout());
    _whenGeneratedLabel = new QLabel (this);
    ((QFormLayout*) layout()) -> addRow ("Map generated", _whenGeneratedLabel);
    _renderTimePerPixelLabel = new QLabel (this);
    ((QFormLayout*) layout()) -> addRow ("Render time per pixel", _renderTimePerPixelLabel);
    _renderTimeLabel = new QLabel (this);
    ((QFormLayout*) layout()) -> addRow ("Render time this map", _renderTimeLabel);
    _pixelsInMapLabel = new QLabel (this);
    ((QFormLayout*) layout()) -> addRow ("Pixels in map", _pixelsInMapLabel);
    _extremesMapLabel = new QLabel (this);
    ((QFormLayout*) layout()) -> addRow ("Minimum and maximum values in map", _extremesMapLabel);
    _extremesGlobeLabel = new QLabel (this);
    ((QFormLayout*) layout()) -> addRow ("Minimum and maximum values in world", _extremesGlobeLabel);

}

CalenhadStatsTab::~CalenhadStatsTab() {

}

void CalenhadStatsTab::showEvent (QShowEvent* e) {
    CalenhadStatistics stats = CalenhadServices::statistics() -> statistics();
    _whenGeneratedLabel -> setText (stats._pixelsInMap == 0 ? "Not rendered" : stats._whenRendered);
    _renderTimeLabel -> setText (stats._pixelsInMap == 0 ? "Not rendered" : QString::number (stats._renderTime) + " ms");
    double timePerPixel = ((double) stats._renderTime / (double) stats._pixelsInMap);
    _renderTimePerPixelLabel -> setText (stats._pixelsInMap == 0 ? "Not rendered" : QString::number (timePerPixel * 1000.0) + " μs");
    _pixelsInMapLabel -> setText (stats._pixelsInMap == 0 ? "Not rendered" : QString::number (stats._pixelsInMap));
    _extremesMapLabel -> setText (stats._pixelsInMap == 0 ? "Not rendered" : QString::number (stats._minValueMap) + " - " + QString::number (stats._maxValueMap));
    _extremesGlobeLabel -> setText (QString::number (stats._minValueGlobal) + " - " + QString::number (stats._maxValueGlobal));
}