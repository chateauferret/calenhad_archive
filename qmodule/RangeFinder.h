//
// Created by martin on 06/10/17.
//

#ifndef CALENHAD_RANGEFINDER_H
#define CALENHAD_RANGEFINDER_H

#include "QModule.h"
#include "../nodeedit/qneport.h"
#include "../nodeedit/qneconnection.h"
#include "../exprtk/CalculatorService.h"
#include "../CalenhadServices.h"

namespace calenhad {
    namespace qmodule {
        class RangeFinder {
        public:
            RangeFinder() {};
            virtual ~RangeFinder () {};
            virtual bool range (double& min, double& max) = 0;
            void setModule (QModule* module) { _module = module; }
        protected:
            QModule* _module;

            QModule* input (int i) {
                calenhad::nodeedit::QNEPort* p = _module -> ports ().at (i);
                if (p -> portType() == calenhad::nodeedit::QNEPort::InputPort) {
                    calenhad::nodeedit::QNEConnection* c = p -> connections ().first ();
                    return dynamic_cast<QModule*> (c -> otherEnd (p) -> owner ());
                }
                return nullptr;
            }

            bool inputRange (int i, double& min, double& max) {
                QModule* qm = input (i);
                if (qm) {
                    bool ok = qm -> range (min, max);
                    return ok;
                } else {
                    return false;
                };
            }

            double param (const QString& name) {
                calenhad::expressions::CalculatorService* calc = CalenhadServices::calculator();
                exprtk::expression<double>* exp = calc -> makeExpression (_module -> parameter (name));
                double v = exp -> value();
                delete exp;
                return v;
            }
        };

        //Finds a range from -1 to 1, being the output range of generator modules.
        class UnitRangeFinder : public RangeFinder {
        public:
            bool range (double& min, double& max) override {
                min = -1.0;
                max = 1.0;
                return true;
            };
        };

        // Range finder for the Exponent module.
        class ExponentRangeFinder : public RangeFinder {
        public:
            bool range (double& min, double& max) override {
                bool ok = inputRange (0, min, max);
                min = std::exp (min);
                max = std::exp (max);
                return ok;
            }
        };

        // Finds the range of the (single) upstream module.
        class PassThroughRangeFinder : public RangeFinder {
        public:
            bool range (double& min, double& max) override {
                bool ok = inputRange (0, min, max);
                return ok;
            }
        };

        // Finds the range of the (single) upstream module.
        class ScaleBiasRangeFinder : public RangeFinder {
        public:
            bool range (double& min, double& max) override {
                bool ok = inputRange (0, min, max);
                double scale = param ("scale");
                double bias = param ("bias");
                min *= scale;
                min += bias;
                max *= scale;
                max += bias;
                return ok;
            }
        };

        // Returns a range defined by two named parameters.
        class BoundsRangeFinder : public RangeFinder {
        public:
            BoundsRangeFinder (QString param1, QString param2) : RangeFinder(), _param1 (param1), _param2 (param2) { };
            virtual ~BoundsRangeFinder() { };
            bool range (double& min, double& max) override {
                double p1 = param (_param1);
                double p2 = param (_param2);
                min = std::min (p1, p2);
                max = std::max (p1, p2);
                return true;
            }
        protected:
            QString _param1, _param2;
        };

        // Sets the range to be the widest range of values in the ranges of all input modules
        class OrRangeFinder: public RangeFinder {

            bool range (double& min, double& max) {
                double _min = 0.0;
                double _max = 0.0;
                bool valid = false;
                for (int i = 0; i < _module->ports ().size (); i++) {
                    calenhad::nodeedit::QNEPort* p = _module->ports ().at (i);
                    if (p->portType () == calenhad::nodeedit::QNEPort::InputPort) {
                        calenhad::nodeedit::QNEConnection* c = p -> connections().first ();
                        QModule* qm = dynamic_cast<QModule*> (c -> otherEnd (p) -> owner ());
                        if (qm) {
                            bool ok = qm -> range (_min, _max);
                            if (ok) {
                                valid = true;
                                if (_min < min) { min = _min; }
                                if (_max > max) { max = _max; }
                            }
                        }
                    }
                }
                return valid;
            }
        };

        // Sets the range to be the narrowest range of values in the ranges of all input modules
        class AndRangeFinder: public RangeFinder {
            QPair<double, double> result = QPair<double, double> (0.0, 0.0);
            double min = 0.0;
            double max = 0.0;

            bool range (double& min, double& max) {
                double _min = 0.0;
                double _max = 0.0;
                bool valid = false;
                for (int i = 0; i < _module->ports ().size (); i++) {
                    calenhad::nodeedit::QNEPort* p = _module->ports ().at (i);
                    if (p->portType () == calenhad::nodeedit::QNEPort::InputPort) {
                        calenhad::nodeedit::QNEConnection* c = p -> connections().first ();
                        QModule* qm = dynamic_cast<QModule*> (c -> otherEnd (p) -> owner ());
                        if (qm) {
                            bool ok = qm -> range (_min, _max);
                            if (ok) {
                                valid = true;
                                if (_min > min) { min = _min; }
                                if (_max < max) { max = _max; }
                            }
                        }
                    }
                }
                return valid;
            }
        };

        // Range finder for the Abs module, which returns the absolute value of a single input.
        class AbsRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                QModule* qm = input (0);
                if (qm) {
                    double _min, _max;
                    bool ok = qm -> range (_min, _max);
                    if (_min < 0 && _max < 0) { max = std::abs (_min); min = std::abs (_max); return true; }
                    if (_min > 0 && _max > 0) { min = _min; max = _max; return true; }
                    min = 0;
                    max = std::max (std::abs (_min), max);
                    return true;
                } else {
                    return false;
                }
            }
        };

        // Range finder for the Invert module.
        class InvertRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                QModule* qm = input (0);
                if (qm) {
                    double _min, _max;
                    bool ok = qm -> range (_min, _max);
                    min = - _max; max = - _min;
                    return true;
                } else {
                    return false;
                }
            }
        };

        // Range finder for the Add module.
        class AddRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                double min0, max0, min1, max1;
                bool ok = inputRange (0, min0, max0) && inputRange (1, min1, max1);
                if (ok) {
                    min = min0 + min1;
                    max = max0 + max1;
                }
                return ok;
            }
        };

        // Range finder for the Diff module.
        class DiffRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                double min0, max0, min1, max1;
                bool ok = inputRange (0, min0, max0) && inputRange (1, min1, max1);
                if (ok) {
                    min = std::min (min0 - max1, min1 - max0);
                    max = std::max (max0 - min1, max1 - min0);
                }
                return ok;
            }
        };

        // Range finder for the Multiply module.
        class MultiplyRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                double min0, max0, min1, max1;
                bool ok = inputRange (0, min0, max0) && inputRange (1, min1, max1);
                if (ok) {
                    min = min0 * min1;
                    max = max0 * max1;
                    if (min0 * max1 < min) { min = min0 * max1; };
                    if (min0 * max1 > max) { max = min0 * max1; };
                    if (max0 * min1 < min) { min = max0 * min1; };
                    if (max0 * min1 > max) { max = max0 * min1; };
                }
                return ok;
            }
        };

        // Range finder for the Min module.
        class MinRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                double min0, max0, min1, max1;
                bool ok = inputRange (0, min0, max0) && inputRange (1, min1, max1);
                if (ok) {
                    min = std::min (min0, min1);
                    max = std::min (max0, max1);
                }
                return ok;
            }
        };

        // Range finder for the Max module.
        class MaxRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                double min0, max0, min1, max1;
                bool ok = inputRange (0, min0, max0) && inputRange (1, min1, max1);
                if (ok) {
                    min = std::max (min0, min1);
                    max = std::max (max0, max1);
                }
                return ok;
            }
        };

        // Range finder for the Power module.
        class PowerRangeFinder : public RangeFinder {
            bool range (double& min, double& max) override {
                double min0, max0, min1, max1;
                bool ok = inputRange (0, min0, max0) && inputRange (1, min1, max1);
                if (ok) {
                    min = std::pow (min0, min1);
                    max = std::pow (max0, max1);
                }
                return ok;
            }
        };


    }
}


#endif //CALENHAD_RANGEFINDER_H
