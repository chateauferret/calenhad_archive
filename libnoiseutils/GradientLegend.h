//
// Created by martin on 27/01/17.
//

#ifndef CALENHAD_GRADIENTLEGEND_H
#define CALENHAD_GRADIENTLEGEND_H

#include "intervallegend.h"

namespace noise {
    namespace utils {

        class GradientLegend : public IntervalLegend {
        public:
            QColor lookup (const int& value) override;

            QColor interpolate (const QColor& c1, const QColor& c2, const double& mix);
        };
    }
}

#endif //CALENHAD_GRADIENTLEGEND_H
