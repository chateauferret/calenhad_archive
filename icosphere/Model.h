#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Rhumb.hpp>
#include <vector>
#include <map>
#include "../geoutils.h"
#include <QBuffer>
#include <QImage>
#include "../mapping/Legend.h"
#include "Bounds.h"

using namespace geoutils;
namespace icosphere {

class Model : public QObject {
Q_OBJECT
public:
    static constexpr double X = 0.525731112119133606;
    static constexpr double Z = 0.850650808352039932;
    static constexpr double PI = 3.141592653589793;
    static constexpr double TWO_PI = 6.283185307179586;
    static constexpr double HALF_PI = 1.570796326794896;

    Model (const unsigned& depth = 8);
    virtual ~Model();
    double distance (const Geolocation& from, const Geolocation& unto) const;
    double loxodrome (const Geolocation& from, const Geolocation& unto);
    virtual double getDatum (const Geolocation& g, const QString& key) = 0;
    virtual void setDatum (const Geolocation& g, const QString& key, double datum) = 0;
    virtual std::string getType() = 0;
    virtual void assemble (const Bounds& bounds) = 0;

    signals:
    void progress (const int&);
    void ready();

protected:
    unsigned int _depth;
    GeographicLib::Geocentric* _gc;
    GeographicLib::Rhumb* _rhumb;
    GeographicLib::Geodesic* _gd;
};

class DatasetNotFoundException : std::runtime_error {
public:
    DatasetNotFoundException (const std::string& msg) : std::runtime_error (msg) { }
};
} // namespace
#endif // MODEL_H
