//
// Created by martin on 02/08/17.
//

#ifndef CALENHAD_PARAMVALIDATOR_H
#define CALENHAD_PARAMVALIDATOR_H

#include <QtCore/QString>

namespace calenhad {
    namespace qmodule {

        class ParamValidator {

        public:
            virtual bool isInValidSet (const double& value) = 0;
            virtual bool isInBestSet (const double& value) = 0;
            virtual QString toString (const double& value) = 0;
        };

        class AcceptAnyRubbish : public ParamValidator {
        public:
            bool isInValidSet (const double& value) override { return true; }
            bool isInBestSet (const double& value) override { return true; }
            QString toString (const double& value) { return "Enter any value"; }
        };

        class AcceptPositive : public ParamValidator {
            bool isInValidSet (const double& value) override { return value >= 0; }
            bool isInBestSet (const double& value) override { return value >= 0; }
            QString toString (const double& value) { return "Enter any positive value or zero"; }
        };

        class AcceptNoiseValue : public ParamValidator {
            bool isInValidSet (const double& value) override { return value >= -1 && value <= 1; }
            bool isInBestSet (const double& value) override { return value >= -1 && value <= 1; }
            QString toString (const double& value) { return "Enter any value between -1 and 1 inclusive"; }
        };

        class PreferNoiseValue : public ParamValidator {
            bool isInValidSet (const double& value) override { return true; }
            bool isInBestSet (const double& value) override { return value >= -1 && value <= 1; }
            QString toString (const double& value) { return "Preferred range is from -1 to 1 inclusive"; }
        };

        class AcceptAngleDegrees : public ParamValidator {
            bool isInValidSet (const double& value) override { return value >= -180 && value <= 180; }
            bool isInBestSet (const double& value) override { return value >= -180 && value <= 180; }
            QString toString (const double& value) { return "Enter any value between -1 and 1 inclusive"; }
        };

        class AcceptRange : public ParamValidator {
        public:
            AcceptRange (const double& bestMin, const double& bestMax, const double& validMin = -DBL_MAX, const double& validMax = DBL_MAX) : ParamValidator(),
                _validMin (validMin), _validMax (validMax), _bestMin (bestMin), _bestMax (bestMax) { }
            virtual bool isInValidSet (const double& value) override { return value >= _validMin && value <= _validMax; }
            bool isInBestSet (const double& value) override { return value >= _bestMin && value <= _bestMax; }
            QString toString (const double& value) {
                QString message = "Enter any value";
                if (_validMin != -DBL_MAX && _validMax != DBL_MAX) { message += " between \"" + QString::number (_validMin) + " and \" " + QString::number (_validMax); }
                if (_validMin == -DBL_MAX && _validMax != DBL_MAX) { message += " less than " + QString::number (_validMax); }
                if (_validMin != -DBL_MAX && _validMax == DBL_MAX) { message += " greater than " + QString::number (_validMin); }
                message += ". Preferred range is from " + QString::number (_bestMin) + " to " + QString::number (_bestMax) + ".";
                return message;
            }
        protected:
            double _validMin, _validMax, _bestMin, _bestMax;
        };

        class PreferInteger : public ParamValidator {
            bool isInValidSet (const double& value) override { return true; }
            bool isInBestSet (const double& value) override { double intpart; return modf (value, &intpart) == 0.0; }
            QString toString (const double& value) override { return "Only integer part of value will be used"; }
        };
    }
}
#endif //CALENHAD_PARAMVALIDATOR_H
