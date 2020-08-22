//
// Created by ferret on 21/08/2020.
//

#include "Map.h"
#include "../nodeedit/Port.h"
#include "../nodeedit/Connection.h"

using namespace calenhad::module;
using namespace calenhad::nodeedit;
using namespace geoutils;

Map::Map (const QString& type) : Cache (type), _image (nullptr), _height (512), _width (512) {

}

Map::~Map () {

}

bool Map::isComplete() {
    return Module::isComplete ();
}

int Map::mapHeight() const {
    return _height;
}

int Map::mapWidth() const {
    return _width;
}



geoutils::Geolocation Map::centre () {
    double lat = _parameters.value ("latitude") -> value();
    double lon = _parameters.value ("longitude") -> value();
    return Geolocation (lat, lon, Degrees);
}

double Map::scale() const {
    return _parameters.value ("scale") -> value();
}

QImage* Map::image() const {
    return _image;
}

void Map::serialize (QDomElement& element) {
    Cache::serialize (element);
}

void Map::inflate (const QDomElement& element) {
    Cache::inflate (element);
}

void Map::generate() {

}
