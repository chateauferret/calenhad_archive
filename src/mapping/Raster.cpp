//
// Created by ferret on 16/01/2021.
//

#include <QImage>
#include "Raster.h"
#include "../geoutils.h"

using namespace calenhad::mapping;
using namespace geoutils;

Raster::Raster (QImage* image) : _image (image), _extent() {

}

Raster::Raster (QImage* image, const Extent& extent) : _image (image), _extent (extent) {

}

Raster::~Raster () {
    delete _image;
}

QImage* Raster::image () {
    return _image;
}

void Raster::setImage (QImage* image) {
    _image = image;
}

Extent Raster::extent () {
    return _extent;
}

void Raster::setExtent (const calenhad::mapping::Extent& extent) {
    _extent = extent;
}

Extent::Extent() : _projection (nullptr),
    _bounds { Geolocation (-M_PI, M_PI / 2), Geolocation (-M_PI, -M_PI / 2), Geolocation (M_PI, -M_PI / 2), Geolocation (-M_PI, M_PI / 2) } {

}

Extent::~Extent () {

}
