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
#include "icosphereutils.h"
#include "dataset.h"



namespace icosphere {
    class Model {
    public:

        static constexpr double X = 0.525731112119133606;
        static constexpr double Z = 0.850650808352039932;
        static constexpr double PI = 3.141592653589793;
        static constexpr double TWO_PI = 6.283185307179586;
        static constexpr double HALF_PI = 1.570796326794896;

        Model (const char& depth = 8);

        virtual ~Model ();

        double distance (const geoutils::Geolocation& from, const geoutils::Geolocation& unto) const;

        double loxodrome (const geoutils::Geolocation& from, const geoutils::Geolocation& unto);

        bool addDataset (const std::string& key, noise::utils::Legend* legend, const char& depth = 0);

        bool removeDataset (const std::string& key);

        virtual int getDatum (const geoutils::Geolocation& g, const std::string& key) = 0;

        virtual bool getImage (QImage* image, const geoutils::GeoQuad& bounds, const std::string& key) = 0;

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
