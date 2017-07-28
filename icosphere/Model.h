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
#include "legend/Legend.h"
#include "Bounds.h"

using namespace geoutils;
namespace icosphere {

class Model : public QObject {
Q_OBJECT
public:


    Model (const unsigned& depth = 8);
    virtual ~Model();
    double distance (const Geolocation& from, const Geolocation& unto) const;
    double loxodrome (const Geolocation& from, const Geolocation& unto);
    virtual double getDatum (const Geolocation& g, const QString& key) = 0;
    virtual void setDatum (const Geolocation& g, const QString& key, double datum) = 0;
    virtual std::string getType() = 0;
    virtual void assembleAsync (const Bounds& bounds) = 0;
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
