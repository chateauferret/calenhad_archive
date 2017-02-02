#include "dataset.h"
#include "../libnoiseutils/legend.h"

using namespace icosphere;
using namespace noise::utils;

Dataset::Dataset() {

}

Dataset::Dataset (const std::string& n, Legend *l, const char& d) : name (n), legend (l), depth (d) {

}

Dataset::~Dataset () {

}

void Dataset::setLegend (Legend* l) {
    legend = l;
}

Legend* Dataset::getLegend() {
    return legend;
}

unsigned char Dataset::getDepth() {
    return depth;
}

