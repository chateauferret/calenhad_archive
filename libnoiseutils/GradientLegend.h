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
            GradientLegend (const QString& name = "GradientLegend");
            virtual ~GradientLegend();
            QColor lookup (const double& value) override;

            QColor interpolateColors (std::map<double, QColor>::iterator lower, std::map<double, QColor>::iterator higher, const double& value);

            double interpolateValues (const double& p1, const double& v1, const double& p2, const double& v2, const double& value);
        };
    }
}

#endif //CALENHAD_GRADIENTLEGEND_H
