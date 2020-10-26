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



namespace calenhad {
    namespace grid {
        namespace geometry {
            struct Cartesian {
            public:
                Cartesian (double x, double y, double z) : _x (x), _y (y), _z (z) {  }
                Cartesian () : _x (0.0), _y (0.0), _z (1.0) {  }
                double _x, _y, _z;
            };

            struct LatLon {
            public:
                LatLon () : lat (0.0), lon (0.0), height (0.0) {

                }

                LatLon (double y, double x) : lat (y), lon (x), height (0.0) {

                }

                double lat, lon, height;
            };


        }
    }

};



namespace calenhad {
namespace grid {


    struct Vertex {
    public:
        geometry::Cartesian cartesian;
        std::forward_list<Vertex*> neighbours;              // neighbouring vertices
        uint32_t id;
        double value = 0.5;
        unsigned level;
    };

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

        Vertex* walkTowards (const geometry::LatLon& target, const unsigned int& depth = 0);

        Vertex* walkTowards (const geometry::Cartesian& target, const unsigned int& depth) const;

        Vertex* nearest (const geometry::LatLon& target, const unsigned int& depth = 0);

        void toLatLon (const geometry::Cartesian& c, geometry::LatLon& g);

        geometry::Cartesian toCartesian (const geometry::LatLon& g, geometry::Cartesian& c);

        static double distSquared (const geometry::Cartesian& a, const geometry::Cartesian& b);

        inline void divideTriangle (Triangle* t);

        std::pair<std::vector<Triangle*>::iterator, std::vector<Triangle*>::iterator> triangles (const unsigned& level);

        std::pair<std::vector<Vertex*>::iterator, std::vector<Vertex*>::iterator> vertices ();

        float* vertexBuffer();
        int getDatum (const geometry::LatLon& g, const std::string& key);

        bool makeRaster (QImage* image);


    protected:
        unsigned _depth;
        uint32_t _count;

        inline void subdivide (const unsigned int& depth);

        std::vector<Vertex*> _vertices;
        std::vector<std::vector<Triangle*>> _triangles;
        mutable Vertex* _lastVisited;

        inline void makeTriangle (Vertex* a, Vertex* b, Vertex* c, Triangle* parent);

        inline void makeNeighbours (Vertex* p, Vertex* q);

        inline Vertex* addVertex (const geometry::Cartesian& cartesian, const unsigned int& depth);

        inline void mid (const geometry::Cartesian& c1, const geometry::Cartesian& c2, geometry::Cartesian& c);

        GeographicLib::Geocentric* _gc;

        // these are working variables for triangle subdivision
        uint32_t k{};
        Vertex* e1{};
        uint64_t edgeKey{};
        bool _initial;
        std::unordered_map<uint64_t, Vertex*> edgeMap;
        std::unordered_map<uint64_t, Vertex*>::iterator it;

        Vertex* ids0[3]{};  // triangles of outer vertices
        Vertex* ids1[3]{};  // triangles of edge vertices

        geometry::Cartesian c{};
        unsigned _level{};
        float* _vertexBuffer;
        static inline uint64_t makeKey (const uint32_t& v1, const uint32_t& v2);


        void setDatum (const geometry::LatLon& g, const std::string& key, float datum);
        double mag;
        int _c1{};

    };
}
} // namespace
#endif // ICOSPHERE_H
