//
// Created by martin on 01/11/17.
//

#include "Statistics.h"
using namespace calenhad::mapping;


Statistics::Statistics (const double& min, const double& max, const double& sum, const int& count) : _min (min), _max (max), _sum (sum), _count (count) {

}

double Statistics::range () {
    return _max - _min;
}

double Statistics::mean () {
    return _sum / _count;
}

bool Statistics::ok () {
    return _max > _min && _count > 0;
}
