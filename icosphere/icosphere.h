#ifndef ICOSPHERE_OLD_H
#define ICOSPHERE_OLD_H
#include <iostream>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Rhumb.hpp>
#include <vector>
#include <map>
#include "vertex.h"
#include "../geoutils.h"
#include <QBuffer>
#include <QImage>
#include "../mapping/Legend.h"
#include "triangle.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <QtCore/QMutex>
#include "Model.h"
#include "Bounds.h"

using namespace geoutils;

namespace noise {
    namespace module {
        class Module;
    }
}

namespace icosphere {

    class Icosphere : public Model {
    Q_OBJECT
    public:

        //--------------------------------------------------------------------------------
        // icosahedron data
        //--------------------------------------------------------------------------------


        static constexpr double vdata[12][3] = {
                {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
                {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
                {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
        };

        static constexpr unsigned tindices[20][3] = {
                {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
                {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
                {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
                {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

        Icosphere (const int& depth);
        ~Icosphere();
        const std::vector<Vertex*> vertices();
        const std::vector<unsigned>& indices (const unsigned int& level);
        unsigned vertexCount();
        Vertex* operator [] (const unsigned& id);
        Vertex* nearest (const Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const Geolocation& target, const unsigned int& depth = 0) const;
        Vertex* walkTowards (const Cartesian& target, const unsigned int& depth = 0) const;
        void visit (Vertex* vertex);
        bool getImage (QImage* image, Legend* legend, const Bounds& bounds);
        // override virtual methods
        void setDatum (const Geolocation& g, const QString& key, double datum) override;
        double getDatum (const Geolocation& g, const QString& key) override;
        std::string getType();
        const unsigned depth();
        const Bounds& bounds();
        void lock();
        void unlock();
        void assemble (const Bounds& bounds = Bounds());

    signals:
        void progress (const int&);

    protected:
        void subdivide (const unsigned int &level);
        std::vector<Vertex*> _vertices;
        std::vector<std::vector<unsigned>*> _indices;
        std::vector<unsigned> _listIds;
        std::map<boost::uint128_type, Triangle*> _triangles;
        mutable Vertex* _vertex;
        mutable Vertex* _lastVisited;
        void makeTriangle (std::vector<unsigned>& refinedIndices, const unsigned& a, const unsigned& b, const unsigned& c, const unsigned int& level, Triangle* parent);
        void addTriangle (const unsigned& a, const unsigned& b, const unsigned& c, const unsigned& level, Triangle* nparent);
        boost::uint128_type _triangleKey (unsigned a, unsigned b, unsigned c);
        void makeNeighbours (const unsigned& p, const unsigned& q);
        Bounds _bounds = Bounds();
        bool coversTriangle (const Geolocation& a, const Geolocation& b, const Geolocation& c, const Bounds& bounds) const;
        bool coversTriangle (const Geolocation& a, const Geolocation& b, const Geolocation& c) const;
        bool _locked;
        QMutex _lockMutex;



        void purge ();
        double _toDo;
    };
} // namespace
#endif // ICOSPHERE_OLD_H
