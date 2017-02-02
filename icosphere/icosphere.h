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
        Icosphere (const unsigned int& depth, geoutils::GeoQuad* bounds = nullptr);

        ~Icosphere ();

        const std::vector<Vertex*> vertices ();

        const std::vector<unsigned>& indices (const unsigned int& level);

        unsigned vertexCount ();

        Vertex* operator[] (const unsigned& id);

        Vertex* nearest (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;

        Vertex* walkTowards (const geoutils::Geolocation& target, const unsigned int& depth = 0) const;

        Vertex* walkTowards (const geoutils::Cartesian& target, const unsigned int& depth = 0) const;

        void visit (Vertex* vertex);

        std::set<Vertex*>* getVertices (const geoutils::GeoQuad& bounds, const unsigned& depth); // caller is responsible for deleting the returned pointer

        // override virtual methods
        int getDatum (const geoutils::Geolocation& g, const std::string& key);

        void setDatum (const geoutils::Geolocation& g, const std::string& key, int datum);

        bool getImage (QImage* image, const geoutils::GeoQuad& bounds, const std::string& key);

        std::string getType ();

    protected:
        void subdivide (const unsigned int& level);

        std::vector<Vertex*> _vertices;
        std::vector<std::vector<unsigned>*> _indices;
        std::vector<unsigned> _listIds;
        std::map<uint128_t, Triangle*> _triangles;
        geoutils::GeoQuad* _bounds;
        mutable Vertex* _vertex;
        mutable Vertex* _lastVisited;

        void
        makeTriangle (std::vector<unsigned>& refinedIndices, const unsigned& a, const unsigned& b, const unsigned& c, const unsigned int& level, Triangle* parent);

        void addTriangle (const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* nparent);

        uint128_t triangleKey (unsigned a, unsigned b, unsigned c);

        void makeNeighbours (const unsigned& p, const unsigned& q);

    };
}
#endif // ICOSPHERE_H
