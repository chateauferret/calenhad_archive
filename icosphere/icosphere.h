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

namespace icosphere {

    class Vertex;
    class Triangle;
    class Legend;

    class Bounds {
    public:
        // in radians
        Bounds (double n = M_PI, double s = -M_PI, double w = -M_PI * 2, double e = M_PI * 2) : north (n), south (s), west (w), east (e) { }
        double north, south, west, east;
        int estimateVertexCount (int level) {

            int n = 10 * std::pow (4, level - 1) + 2;
            double latPortion = (north > 0 == south > 0 ? std::abs (std::sin (north)) - std::abs (std::sin (south)) : std::abs (std::sin (north)) + std::abs (std::sin (south))) / 2;
            double lonPortion = (east > west ? (east - west) : 2 * M_PI - (west - east)) / (2 * M_PI);
            double portion = latPortion * lonPortion;
            return (int) (n * portion);
        }
    };

    class Icosphere : public QObject {
        Q_OBJECT
    public:

        static Icosphere* instance();
        ~Icosphere ();
        const std::vector<Vertex*> vertices ();
        const std::vector<unsigned>& indices (const unsigned int& level);
        unsigned vertexCount();
        Vertex* operator[] (const unsigned& id);
        Vertex* nearest (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const geoutils::Cartesian& target, const unsigned int& depth = 0) const;
        void visit (Vertex* vertex);
        double distance (const geoutils::Geolocation& from, const geoutils::Geolocation& unto) const;
        double loxodrome (const geoutils::Geolocation& from, const geoutils::Geolocation& unto);
        std::experimental::optional<double> getDatum (const geoutils::Geolocation& g, const QUuid& key);
        bool setDatum (const geoutils::Geolocation& g, const QUuid& key, const double& datum);
        bool getImage (QImage* image, const icosphere::Bounds& bounds, const QUuid& key, Legend* legend);
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c, const Bounds& bounds) const;
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c) const;
        int getCountInBounds();
        int getCountMerged();

        public slots:
        //void update (QModule* module);

        signals:
        //void updated ();

    protected:
        Icosphere (const char& depth, const Bounds& bounds = Bounds());
        void subdivide (const unsigned int& level);

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
        static Icosphere* _instance;
        char _levels;
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
