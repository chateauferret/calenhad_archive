#ifndef ICOSPHERE_H
#define ICOSPHERE_H
#include <iostream>
#include <QImage>
#include <QUuid>
#include <set>
#include <experimental/optional>
#include <GeographicLib/Rhumb.hpp>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Geodesic.hpp>
#include "../geoutils.h"
#include "icosphereutils.h"
#include "../qmodule/QModule.h"
#include "Bounds.h"

namespace icosphere {

    class Vertex;
    class Triangle;
    class Legend;

    class Icosphere : public QObject {
        Q_OBJECT
    public:
        Icosphere();
        Icosphere (const char& depth, const Bounds& bounds = Bounds());
        ~Icosphere ();
        const std::vector<Vertex*> vertices ();
        const std::vector<unsigned>& indices (const unsigned int& level);
        unsigned vertexCount();
        Vertex* operator[] (const unsigned& id);
        Vertex* nearest (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* nearest (const geoutils::Cartesian& target, const unsigned int& depth = 0) const;
        void visit (Vertex* vertex);
        double distance (const geoutils::Geolocation& from, const geoutils::Geolocation& unto) const;
        double loxodrome (const geoutils::Geolocation& from, const geoutils::Geolocation& unto);
        std::experimental::optional<double> getDatum (const geoutils::Geolocation& g, const QString& key);
        std::experimental::optional<double> getDatum (const geoutils::Cartesian& c, const QString& key);
        bool setDatum (const geoutils::Geolocation& g, const QString& key, const double& datum);
        bool setDatum (const geoutils::Cartesian& c, const QString& key, const double& datum);
        bool getImage (QImage* image, const icosphere::Bounds& bounds, const QString& key, Legend* legend);
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c, const Bounds& bounds) const;
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c) const;
        int getCountInBounds();
        int getCountMerged();
        const char& depth();
        const Bounds& bounds();

    protected:

        void subdivide (const unsigned int& level);
        Vertex* walkTowards (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const geoutils::Cartesian& target, const unsigned int& depth = 0) const;
        // pointers for temporary use
        std::vector<Vertex*> _vertices;
        std::vector<std::vector<unsigned>*> _indices;
        std::vector<unsigned> _listIds;
        mutable Vertex* _vertex;
        mutable Vertex* _lastVisited;

        std::map<uint128_t, Triangle*> _triangles;
        Bounds _bounds;

        int _countInBounds, _merged;
        void makeTriangle (std::vector<unsigned>& refinedIndices, const unsigned& a, const unsigned& b, const unsigned& c, const unsigned int& level, Triangle* parent);
        void addTriangle (const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* parent);
        uint128_t triangleKey (unsigned a, unsigned b, unsigned c);
        void makeNeighbours (const unsigned& p, const unsigned& q);
        bool isInBounds (const geoutils::Geolocation& a, const Bounds& bounds) const;
        bool isInTriangle (const geoutils::Geolocation& p1, const geoutils::Geolocation& p2, const geoutils::Geolocation& p3, double lon, double lat) const;

        void addVertex (Vertex* v);
        void insertNewVertex (Vertex* v);
        void deleteTriangle (Triangle* t);
        char _depth;
        GeographicLib::Geocentric* _gc;
        GeographicLib::Rhumb* _rhumb;
        GeographicLib::Geodesic* _gd;
    };

    class IllegalIcosphereAccessException : std::runtime_error {
    public:
        IllegalIcosphereAccessException (const std::string& msg) : std::runtime_error (msg) {}
    };
}
#endif // ICOSPHERE_H
