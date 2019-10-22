//
// Created by martin on 02/08/17.
//

#ifndef CALENHAD_PARAMVALIDATOR_H
#define CALENHAD_PARAMVALIDATOR_H

#include <QtCore/QString>
#include <cfloat>
#include <cmath>
#include <iostream>
#include "geoutils.h"
namespace calenhad {
    namespace module {

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
            QString toString (const double& value) override { return "Enter any value"; }
        };

        class AcceptPositive : public ParamValidator {
            bool isInValidSet (const double& value) override { return value >= 0; }
            bool isInBestSet (const double& value) override { return isInValidSet (value);  }
            QString toString (const double& value) override { return "Enter any positive value or zero"; }
        };

        class AcceptNoiseValue : public ParamValidator {
            bool isInValidSet (const double& value) override { return value >= -1 && value <= 1; }
            bool isInBestSet (const double& value) override { return isInValidSet (value);  }
            QString toString (const double& value) override { return "Enter any value between -1 and 1 inclusive"; }
        };

        class PreferNoiseValue : public ParamValidator {
            bool isInValidSet (const double& value) override { return true; }
            bool isInBestSet (const double& value) override { return value >= -1 && value <= 1; }
            QString toString (const double& value) override { return "Preferred range is from -1 to 1 inclusive"; }
        };

        class AcceptAngleDegrees : public ParamValidator {
            bool isInValidSet (const double& value) override { return value >= -180 && value <= 180; }
            bool isInBestSet (const double& value) override { return isInValidSet (value);  }
            QString toString (const double& value) override { return "Enter any value between -180 and 180 inclusive"; }
        };

        class AcceptRange : public ParamValidator {
        public:
            double _epsilon;
            AcceptRange (const double& bestMin, const double& bestMax, const double& validMin = -DBL_MAX, const double& validMax = DBL_MAX) : ParamValidator(),
                _validMin (validMin), _validMax (validMax), _bestMin (bestMin), _bestMax (bestMax) {  _epsilon = 0.00000001; }
            bool isInValidSet (const double& value) override { return std::isgreaterequal (value + _epsilon, _validMin) && std::islessequal (value - _epsilon, _validMax);  }
            bool isInBestSet (const double& value) override {
                return std::isgreaterequal (value - _epsilon, _bestMin) && std::islessequal (value + _epsilon, _bestMax); }
            QString toString (const double& value) override {
                QString message = "Enter any value";
                if (_validMin != -DBL_MAX && _validMax != DBL_MAX) { message += " between " + QString::number (_validMin) + " and " + QString::number (_validMax); }
                if (_validMin == -DBL_MAX && _validMax != DBL_MAX) { message += " less than " + QString::number (_validMax); }
                if (_validMin != -DBL_MAX && _validMax == DBL_MAX) { message += " greater than " + QString::number (_validMin); }
                message += ". Preferred range is from " + QString::number (_bestMin) + " to " + QString::number (_bestMax) + ".";
                return message;
            }
        protected:
            double _validMin, _validMax, _bestMin, _bestMax;
        };

        class AcceptAngle : public AcceptRange {
        public:
            QString _minLetter, _maxLetter;
            explicit AcceptAngle (geoutils::AngleType type) : AcceptRange (0.0, type == geoutils::AngleType::Latitude ? 90.0 : 180.0, 0.0, type == geoutils::AngleType::Latitude ? 90.0 : 180.0),
                _minLetter (type == geoutils::AngleType::Latitude ? "S" : "W"),
                _maxLetter (type == geoutils::AngleType::Latitude ? "N" : "E") {
            }
            bool isInValidSet (const double& value) override {
                return AcceptRange::isInValidSet (std::abs (value));
            }
            bool isInBestSet (const double& value) override { return isInValidSet (value); }
            QString toString (const double& value) override {
                std::cout << "Failed validation on " << value << "\n";
                QString message = "Enter any value";
                if (_validMin != -DBL_MAX && _validMax != DBL_MAX) { message += " between " + QString::number (_validMax) + "° " +  _minLetter + " and " + QString::number (_validMax) + "° " + _maxLetter; }
                return message;
            }
        };

        class AcceptInteger : public ParamValidator {
            bool isInValidSet (const double& value) override { double intpart; return modf (value, &intpart) == 0.0; }
            bool isInBestSet (const double& value) override { return isInValidSet (value); }
            QString toString (const double& value) override { return "Only integer part of value will be used"; }
        };

        class PreferInteger : public ParamValidator {
            bool isInValidSet (const double& value) override { return true; }
            bool isInBestSet (const double& value) override { double intpart; return modf (value, &intpart) == 0.0; }
            QString toString (const double& value) override { return "Only integer part of value will be used"; }
        };
    }
}


#endif