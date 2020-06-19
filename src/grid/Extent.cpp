//
// Created by ferret on 19/06/2020.
//

#include <QtWidgets/QFileDialog>
#include "Extent.h"
#include "../module/Module.h"
#include "../graph/ComputeService.h"

using namespace calenhad::grid;
using namespace calenhad::module;
using namespace calenhad::graph;

Extent::Extent (Module* module) : _buffer (nullptr), _module (module) {

}

Extent::~Extent() {
    delete [] _buffer;
}

float Extent::getValue(const geoutils::Geolocation &geolocation) {
    return 0;
}

void Extent::compute() {

    if (! _buffer) {
        _buffer = new float [rasterHeight() * rasterHeight() * 2];
    }
    if (! _module -> name().isNull()) {
        ComputeService* c = new ComputeService();
        c -> compute (_module);
        delete c;
    }
}

float* Extent::buffer () {
    return _buffer;
}

size_t Extent::rasterHeight () const {
    return std::pow (2, _resolution);
}

void Extent::save() {

    QString fileName = QFileDialog::getSaveFileName (nullptr, "Save heightmap image", QDir::homePath(), "Image Files (*.png *.jpg *.bmp)");
    QImage* image = new QImage (rasterHeight() * 2, rasterHeight(), QImage::Format_ARGB32);
    for (int i = 0; i < rasterHeight() * 2; i++) {
        for (int j = 0; j < rasterHeight(); j++) {
            int index = j * rasterHeight() * 2 + i;
            double h = _buffer [index] / 2 + 0.5;
            int value = (int) floor (h * 256);
            value = std::max (std::min (value, 255), 0);
            QColor c (value, value, value, 255);
            image -> setPixelColor (i, j, c);
        }
    }
    image -> save (fileName);
}

Extent::Extent (Extent *other) : _buffer (other -> buffer()),
                                 _module (other -> module()),
                                 _resolution (other -> resolution()) {

}

int Extent::resolution() const {
    return _resolution;
}

Module *Extent::module() {
    return _module;
}
