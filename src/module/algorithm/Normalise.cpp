//
// Created by ferret on 17/09/2020.
//

#include <QtWidgets/QProgressDialog>
#include "Normalise.h"

using namespace calenhad::module::algorithm;
using namespace calenhad::module;
using namespace calenhad::controls::globe;

Normalise::Normalise () {

}

Normalise::~Normalise () {

}

bool Normalise::execute (calenhad::grid::CubicSphere* buffer) {
   /* QProgressDialog progress ("Computing algorithm", "Abandon", 0, (int) buffer -> count(), nullptr);
    progress.setWindowModality(Qt::WindowModal);

    CalenhadStatistics statistics = CalenhadServices::compute() -> statistics();
    float min = (float) statistics._minValue;
    float range = (float) statistics._range;
    float newMin = -1.0, newMax = 1.0;
    for (int i = 0; i < buffer -> count(); i++) {
        float v = buffer -> data() [i];
        v = (v - min) / range;
        v = v * (newMax - newMin) + newMin;
        buffer -> data() [i] = v;
        progress.setValue (i);
        if (progress.wasCanceled()) {
            return false;
        }
    }
    CalenhadServices::compute() -> computeStatistics (buffer); */
    return true;
}



