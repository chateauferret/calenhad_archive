#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include "../geoutils.h"
#include <QBuffer>
#include <QImage>
#include <vector>
#include <map>
#include <GeographicLib/Rhumb.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <experimental/optional>
#include "icosphereutils.h"
#include "dataset.h"



namespace icosphere {

    class Bounds {
    public:
        // in radians
        Bounds (double n = M_PI_2, double s = -M_PI_2, double w = -M_PI, double e = M_PI) : north (n), south (s), west (w), east (e) { }
        double north, south, west, east;
    };

    class Model {
    public:

        Model (const char& depth = 8);
        virtual ~Model ();
        double distance (const geoutils::Geolocation& from, const geoutils::Geolocation& unto) const;
        double loxodrome (const geoutils::Geolocation& from, const geoutils::Geolocation& unto);
        bool addDataset (const std::string& key, noise::utils::Legend* legend, const char& depth = 0);
        bool removeDataset (const std::string& key);
        virtual bool setDatum (const geoutils::Geolocation& g, const std::string& key, const double& datum) = 0;
        virtual std::experimental::optional<double> getDatum (const geoutils::Geolocation& g, const std::string& key) = 0;
        virtual bool getImage (QImage* image, const Bounds& bounds, const std::string& key) = 0;
        virtual std::string getType () = 0;
        std::string getDatasetNames ();
        bool hasDataset (const std::string& key);

    protected:
        char _levels;
        std::map<std::string, Dataset> _datasets;
        GeographicLib::Geocentric* _gc;
        GeographicLib::Rhumb* _rhumb;
        GeographicLib::Geodesic* _gd;
    };

    class DatasetNotFoundException : std::runtime_error {
    public:
        DatasetNotFoundException (const std::string& msg) : std::runtime_error (msg) {}
    };
}
#endif // MODEL_H
