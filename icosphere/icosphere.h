#ifndef ICOSPHERE_H
#define ICOSPHERE_H
#include <iostream>
#include <QImage>
#include <set>
//#include <boost/multiprecision/cpp_int.hpp>
#include "model.h"
#include "../geoutils.h"
#include "icosphereutils.h"

namespace icosphere {

    class Vertex;
    class Triangle;

    class Icosphere : public Model {
    public:

        Icosphere (const char& depth, const Bounds& bounds = Bounds());
        ~Icosphere ();
        const std::vector<Vertex*> vertices ();
        const std::vector<unsigned>& indices (const unsigned int& level);
        unsigned vertexCount ();
        Vertex* operator[] (const unsigned& id);
        Vertex* nearest (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const geoutils::Cartesian& target, const unsigned int& depth = 0) const;
        void visit (Vertex* vertex);

        std::experimental::optional<double> getDatum (const geoutils::Geolocation& g, const std::string& key) override;
        bool setDatum (const geoutils::Geolocation& g, const std::string& key, const double& datum) override;
        bool getImage (QImage* image, const Bounds& bounds, const std::string& key) override;
        std::string getType() override;
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c, const Bounds& bounds) const;
        bool coversTriangle (const geoutils::Geolocation& a, const geoutils::Geolocation& b, const geoutils::Geolocation& c) const;

    protected:
        void subdivide (const unsigned int& level);

        std::vector<Vertex*> _vertices;
        std::vector<std::vector<unsigned>*> _indices;
        std::vector<unsigned> _listIds;
        std::map<uint128_t, Triangle*> _triangles;
        Bounds _bounds;
        mutable Vertex* _vertex;
        mutable Vertex* _lastVisited;

        void makeTriangle (std::vector<unsigned>& refinedIndices, const unsigned& a, const unsigned& b, const unsigned& c, const unsigned int& level, Triangle* parent);
        void addTriangle (const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* parent);
        uint128_t triangleKey (unsigned a, unsigned b, unsigned c);
        void makeNeighbours (const unsigned& p, const unsigned& q);
        bool isInBounds (const geoutils::Geolocation& a, const Bounds& bounds) const;
        bool isInTriangle (const geoutils::Geolocation& p1, const geoutils::Geolocation& p2, const geoutils::Geolocation& p3, double lon, double lat) const;
    };

    class IllegalIcosphereAccessException : std::runtime_error {
    public:
        IllegalIcosphereAccessException (const std::string& msg) : std::runtime_error (msg) {}
    };
}
#endif // ICOSPHERE_H
