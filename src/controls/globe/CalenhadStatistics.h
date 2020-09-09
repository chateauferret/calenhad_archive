//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_CALENHADSTATISTICS_H
#define CALENHAD_CALENHADSTATISTICS_H


#include <QtCore/QDateTime>


namespace calenhad {
    namespace controls {
        namespace globe {
            class CalenhadStatistics {
            public:
                CalenhadStatistics() { }
                CalenhadStatistics (CalenhadStatistics const &other) :
                    _valueCount (other._valueCount), _renderTime (other._renderTime), _meanValue (other._meanValue),
                    _range (other._range), _minValue (other._minValue), _maxValue (other._maxValue) {
                    for (int i = 0; i < 1000; i++) {
                        _buckets [i] = other._buckets [i];
                    }
                }

                int _valueCount = 0;
                int _renderTime = 0;
                double _meanValue = 0.0;
                double _range = 0.0;
                double _minValue = 0.0, _maxValue = 0.0;
                unsigned _buckets [1000];
            };
        }
    }
}

#endif //CALENHAD_CALENHADSTATISTICS_H
