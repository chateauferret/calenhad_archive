
#include "model.h"

using namespace icosphere;
using namespace noise::utils;

Model::Model (const char& depth) : _levels (depth) {

}

Model::~Model() {

}

bool Model::addDataset (const std::string& key, Legend* legend, const char& depth) {
    std::cout << "Adding dataset " << key << "\n";
    Dataset dataset = Dataset (key, legend, depth);
    if (hasDataset (key)) {
        removeDataset (key);
    }
    std::pair<std::string, Dataset> item = std::make_pair (key, dataset);
    _datasets.insert (item);
    return true;

}

bool Model::removeDataset (const std::string& key) {
    if (_datasets.find (key) != _datasets.end()) {
        // std::for_each (_vertices.begin(), _vertices.end(), [&key] (Vertex* pV) { pV -> erase (key); });
        _datasets.erase (key);
        return true;
    } else {
        return false;
    }
}

bool Model::hasDataset (const std::string& key) {
    return (_datasets.find (key) != _datasets.end());
}

double Model::distance (const geoutils::Geolocation& from, const geoutils::Geolocation& unto) const {
    double d;
    _gd -> Inverse (from.latitude, from.longitude, unto.latitude, unto.longitude, d);
    return d;
}

double Model::loxodrome (const geoutils::Geolocation& from, const geoutils::Geolocation& unto) {
    double azimuth;
    double length;
    _rhumb -> Inverse (from.latitude, from.longitude, unto.latitude, unto.longitude, azimuth, length);
    return azimuth;
}


std::string Model::getDatasetNames() {
    std::string names = "";
    std::map<std::string, Dataset>::iterator i;
    for (i = _datasets.begin(); i != _datasets.end(); ++i) {
        if (i != _datasets.begin()) { names += " "; }
        names += i -> first + " ";
    }
    return names;
}
