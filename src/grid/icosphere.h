#ifndef ICOSPHERE_H
#define ICOSPHERE_H
#include <iostream>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Rhumb.hpp>
#include <vector>
#include <forward_list>
#include <list>
#include <unordered_map>
#include <src/module/Module.h>
#include "Vertex.h"


namespace calenhad {
namespace grid {




    struct Triangle {
    public:
        Vertex* vertices[3];
        std::forward_list<Triangle*> children;
        Triangle* parent;
        unsigned level;
    };

    class Icosphere {
    public:

        static constexpr double X = 0.525731112119133606;
        static constexpr double Z = 0.850650808352039932;

        explicit Icosphere (const unsigned int& depth);

        ~Icosphere ();

        unsigned long vertexCount ();

        Vertex* operator[] (const uint32_t& id);

        void visit (Vertex* vertex);

        Vertex* walkTowards (const geoutils::Geolocation& target, const unsigned int& depth = 0);

        Vertex* walkTowards (const geoutils::Cartesian& target, const unsigned int& depth) const;

        Vertex* nearest (const geoutils::Geolocation& target, const unsigned int& depth = 0);

        static double distSquared (const geoutils::Cartesian& a, const geoutils::Cartesian& b);

        inline void divideTriangle (Triangle* t);

        std::pair<std::vector<Triangle*>::iterator, std::vector<Triangle*>::iterator> triangles (const unsigned& level);

        std::pair<std::vector<Vertex*>::iterator, std::vector<Vertex*>::iterator> vertices ();

        float* vertexBuffer();

        bool makeRaster (QImage* image);


        static int verticesForLevel (int level);

    protected:
        unsigned _depth;
        uint32_t _count;

        inline void subdivide (const unsigned int& depth);

        std::vector<Vertex*> _vertices;
        std::vector<std::vector<Triangle*>> _triangles;
        mutable Vertex* _lastVisited;

        inline void makeTriangle (Vertex* a, Vertex* b, Vertex* c, Triangle* parent);

        inline void makeNeighbours (Vertex* p, Vertex* q) const;

        inline Vertex* addVertex (const geoutils::Cartesian& cartesian, const unsigned int& depth);

        inline void mid (const geoutils::Cartesian& c1, const geoutils::Cartesian& c2, geoutils::Cartesian& c);

        GeographicLib::Geocentric* _gc;

        // these are working variables for triangle subdivision
        uint32_t k;
        Vertex* e1;
        uint64_t edgeKey;
        bool _initial;
        std::unordered_map<uint64_t, Vertex*> edgeMap;
        std::unordered_map<uint64_t, Vertex*>::iterator it;

        Vertex* ids0[3];  // triangles of outer vertices
        Vertex* ids1[3];  // triangles of edge vertices

        geoutils::Cartesian c;
        unsigned _level;
        float* _vertexBuffer;
        static inline uint64_t makeKey (const uint32_t& v1, const uint32_t& v2);


        double mag;
        int _c1;

        virtual void setValue (Vertex* pVertex);

        int _lastVertex;
    };
}
} // namespace
#endif // ICOSPHERE_H
